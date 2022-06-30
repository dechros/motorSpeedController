/**
 * @file data_manager.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes data manager related declerations.
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "data_manager.h"

EthernetInterface *net = NULL;
TCPSocket server;
TCPSocket *clientSocket = NULL;
TCPSocket *clientSocketOld = NULL;
SocketAddress clientAddress;
SocketAddress ip;
SocketAddress netmask;
SocketAddress gateway;

char rxBuf[512] = {0};
char txBuf[512] = {0};

BlockDevice *device;
FATFileSystem fs("fs");

DigitalOut internet_led(LED2);
Timer ip_timer;

Thread data_manager_thread;
Mutex data_manager_mutex;

void set_ethernet_interface()
{
    net = new EthernetInterface;
    if (net == NULL)
    {
        serial_write("  ## Ethernet memory allocation error.");
        ThisThread::sleep_for(osWaitForever);
    }
    net->set_network(IP, NETMASK, GATEWAY); /* Can be deleted for dynamic IP adress. */
    nsapi_size_or_error_t error = net->connect();
    if (error != 0)
    {
        serial_write("  ## Ethernet connection error.");
        ThisThread::sleep_for(osWaitForever);
    }
    net->get_ip_address(&ip);
    net->get_netmask(&netmask);
    net->get_gateway(&gateway);
    ip.set_port(PORT);
    const char *ipAddr = ip.get_ip_address();
    const char *netmaskAddr = netmask.get_ip_address();
    const char *gatewayAddr = gateway.get_ip_address();
    string ip_str(ipAddr);
    string netmask_str(netmaskAddr);
    string gateway_str(gatewayAddr);
    serial_write("IP address: " + ip_str);
    serial_write("Netmask: " + netmask_str);
    serial_write("Gateway: " + gateway_str);
    server.open(net);
    server.bind(ip);
    server.listen(5);
}

void set_sd_card(PinName mosi, PinName miso, PinName sclk, PinName cs)
{
    device = new SDBlockDevice(mosi, miso, sclk, cs);
    int err = fs.mount(device);
    if (err == 0)
    {
        serial_write("SD card init OK.");
    }
    else
    {
        serial_write("  ## SD card init error!");
    }
}

void data_manager_start_thread()
{
    data_manager_thread.start(data_managing_thread);
    serial_write("Data manager thread is started.");
}

void data_managing_thread()
{
    std::string incoming_message;
    while (true)
    {
        data_manager_mutex.lock();
        ThisThread::sleep_for(1);
        nsapi_error_t error = 0;
        clientSocket = server.accept(&error);
        if (error != 0)
        {
            serial_write("  ## Connection error.");
            clientSocket->close();
            serial_write("Closed the connection.");
            data_manager_mutex.unlock();
            continue;
        }
        clientSocket->getpeername(&clientAddress);
        string client_ip(clientAddress.get_ip_address());
        serial_write("Connected. IP : " + client_ip);
        clientSocket->set_timeout(1000);
        error = clientSocket->recv(rxBuf, sizeof(rxBuf));
        if (error <= 0)
        {
            serial_write("  ## Data receive error.");
            clientSocket->close();
            serial_write("Closed the connection.");
            data_manager_mutex.unlock();
            continue;
        }
        string received_data(rxBuf);
        /* serial_write("Received Data : " + received_data); */
        if (received_data.find("led_toggle") != std::string::npos)
        {
            serial_write("Message : led_toggle");
            internet_led.write(!internet_led.read());
            string web_string = "Halit";
            const char *message2 = web_string.c_str();
            clientSocket->send(message2, strlen(message2));
        }
        else if (received_data.find("refresh_graph") != std::string::npos)
        {
            string web_string = REFRESH_GRAPH_HEADER;
            serial_write("Message : refresh_graph");
            bool rpm_string_has_data = false;
            rpm_mutex.lock();
            if (rpm_string.length() > 0)
            {
                rpm_string_has_data = true;
            }
            web_string += rpm_string;
            rpm_string = "";
            rpm_mutex.unlock();
            web_string += WEB_MESSAGE_FOOTER;
            serial_write(web_string);
            if (rpm_string_has_data == true)
            {
                const char *message3 = web_string.c_str();
                clientSocket->send(message3, strlen(message3));
            }
        }
        else
        {
            int eclapsed_time_us = ip_timer.read_us();
            serial_write("Eclapsed time since last connection : " + to_string(eclapsed_time_us));
            if (eclapsed_time_us < 3000000 && eclapsed_time_us > 0)
            {
                serial_write("  ## To fast connection.");
                clientSocket->close();
                serial_write("Closed the connection.");
                data_manager_mutex.unlock();
                continue;
            }
            serial_write("File transfer started.");
            for (int i = 0; i < 180000; i += 200)
            {
                incoming_message = file_read("index.html", i, 200);
                /* serial_write("File : " + incoming_message); */
                /* serial_write(to_string(i)); */
                const char *message = incoming_message.c_str();
                clientSocket->send(message, strlen(message));
            }
            serial_write("File transfer completed.");
            ip_timer.reset();
            ip_timer.start();
        }
        clientSocket->close();
        serial_write("Closed the connection.");
        data_manager_mutex.unlock();
    }
}

std::string file_read(std::string file_name, int starting_index, int read_size)
{
    std::string ret_str;
    file_name = "/fs/" + file_name;
    FILE *file = fopen(file_name.c_str(), "r+");
    fseek(file, starting_index, SEEK_SET);
    while (ret_str.length() < read_size)
    {
        ret_str += fgetc(file);
    }
    fclose(file);
    fflush(file);
    return ret_str;
}