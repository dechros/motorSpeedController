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

DigitalOut internet_led(LED2);
Timer ip_timer;

Thread data_manager_thread;
Mutex data_manager_mutex;

void ethernet_loop();

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
        string led_toggle_command = "led_toggle";
        /* serial_write("Received Data : " + received_data); */
        if (received_data.find(led_toggle_command) != std::string::npos)
        {
            internet_led.write(!internet_led.read());

            string web_string = "HTTP/1.1 200 OK\n...\nAccess-Control-Allow-Origin: http://192.168.0.31\n";
            const char *message = web_string.c_str();
            clientSocket->send(message, strlen(message));

            web_string = "Halit";
            const char *message2 = web_string.c_str();
            clientSocket->send(message2, strlen(message2));
        }
        else
        {
            serial_write("File transfer started.");
            for (int i = 0; i < 200; i += 200)
            {
                incoming_message = web_read_esp_sd("simple.txt", i, 200);
                /* serial_write("STM32 received a message : " + incoming_message); */
                serial_write(to_string(i));
                const char *message = incoming_message.c_str();
                clientSocket->send(message, strlen(message));
            }
            serial_write("File transfer completed.");
        }
        clientSocket->close();
        serial_write("Closed the connection.");
        ip_timer.reset();
        ip_timer.start();
        data_manager_mutex.unlock();
    }
}

std::string web_read_esp_sd(std::string file_name, int starting_index, int read_size)
{
    std::string incoming_message;
    std::string sent_message;
    sent_message += READ_WEBSITE_HEADER;
    sent_message += file_name + "-";
    sent_message += to_string(starting_index) + "-";
    sent_message += to_string(read_size) + MESSAGE_FOOTER;

    int message_sent_count = 0;
    bool message_integrity = false;
    while (true)
    {
        serial_write_esp_sd(sent_message);
        /* serial_write("STM32 sent a message."); */
        /* serial_write("STM32 sent a message : " + sent_message); */
        message_sent_count++;
        if (message_sent_count > MAX_MESSAGE_TRY)
        {
            serial_write("  ## Serial com maximum try error.");
            message_integrity = false;
            break;
        }
        if (is_message_timed_out() == true)
        {
            serial_write("  ## Serial com timeout error.");
            ThisThread::sleep_for(100);
            continue;
        }
        incoming_message = serial_read_esp_sd(std::string(READ_WEBSITE_HEADER).size());
        while (true)
        {
            if (incoming_message.compare(READ_WEBSITE_HEADER) == 0)
            {
                incoming_message += serial_read_esp_sd();
                break;
            }
            else
            {
                incoming_message.erase(incoming_message.begin());
                int size_old = incoming_message.size();
                incoming_message += serial_read_esp_sd(1);
                if (size_old == incoming_message.size())
                {
                    break;
                }
            }
        }
        message_integrity = check_message_integrity(incoming_message, READ_WEBSITE_HEADER, MESSAGE_FOOTER);
        if (message_integrity == false)
        {
            serial_write("  ## Serial com message integrity error. : " + incoming_message);
            ThisThread::sleep_for(100);
            continue;
        }
        else
        {
            incoming_message = strip_the_message(incoming_message, READ_WEBSITE_HEADER, MESSAGE_FOOTER);
            break;
        }
    }
    return incoming_message;
}

std::string strip_the_message(std::string main_string, std::string header, std::string footer)
{
    main_string = main_string.erase(0, header.size());
    main_string = main_string.erase(main_string.length() - footer.length(), footer.size());
    return main_string;
}

bool check_message_integrity(std::string main_string, std::string header, std::string footer)
{
    bool ret_val;
    int header_pos = main_string.find(header);
    int footer_pos = main_string.find(footer);
    if ((header_pos == 0) && (footer_pos == (int)(main_string.length() - footer.length())))
    {
        ret_val = true;
    }
    else
    {
        ret_val = false;
    }
    return ret_val;
}

bool is_message_timed_out()
{
    ThisThread::sleep_for(MIN_TIMEOUT_MS);
    int wait_count = 0;
    while (serial_readable_esp_sd() == false)
    {
        ThisThread::sleep_for(MIN_TIMEOUT_MS);
        wait_count++;
        if (wait_count >= MAX_TIMEOUT_COUNT)
        {
            return true;
        }
    }
    return false;
}

void ethernet_loop()
{

    while (true)
    {

        ThisThread::yield();
    }
}