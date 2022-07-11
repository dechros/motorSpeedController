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
Timer website_timer;
Timer socket_timer;

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
        nsapi_error_t error = 0;
        clientSocket = server.accept(&error);
        int eclapsed_time_us = socket_timer.read_us();
        serial_write("Eclapsed time since last connection : " + to_string(eclapsed_time_us));
        if (eclapsed_time_us < 100000 && eclapsed_time_us > 0)
        {
            serial_write("  ## To fast connection.");
        }
        else if (error != 0)
        {
            serial_write("  ## Connection error.");
        }
        else
        {
            clientSocket->getpeername(&clientAddress);
            string client_ip(clientAddress.get_ip_address());
            serial_write("Connected. IP : " + client_ip);
            clientSocket->set_timeout(1000);
            error = clientSocket->recv(rxBuf, sizeof(rxBuf));
            if (error <= 0)
            {
                serial_write("  ## Data receive error.");
            }
            else
            {
                string received_data(rxBuf);
                /* serial_write("Received Data : " + received_data); */
                if (received_data.find("refresh_graph") != std::string::npos)
                {
                    string web_string = REFRESH_GRAPH_HEADER;
                    int time_out = 0;
                    while (true)
                    {
                        time_out++;
                        rpm_mutex.lock();
                        if (rpm_string.length() > 0)
                        {
                            web_string += rpm_string;
                            rpm_string = "";
                            rpm_mutex.unlock();
                            break;
                        }
                        else if (time_out >= 100)
                        {
                            web_string = "0-";
                            rpm_mutex.unlock();
                            break;
                        }
                        else
                        {
                            ThisThread::sleep_for(1);
                        }
                        rpm_mutex.unlock();
                    }
                    web_string += WEB_MESSAGE_FOOTER;
                    const char *message = web_string.c_str();
                    clientSocket->send(message, strlen(message));
                    serial_write("Web Message = " + web_string);
                }
                else if (received_data.find("file_list") != std::string::npos)
                {
                    string web_string = FILE_LIST_HEADER;
                    std::vector<std::string> file_list = get_file_list("");
                    for (int i = 0; i < file_list.size(); i++)
                    {
                        web_string += file_list[i] + "-";
                    }
                    web_string += WEB_MESSAGE_FOOTER;
                    const char *message = web_string.c_str();
                    clientSocket->send(message, strlen(message));
                    serial_write("Web Message = " + web_string);
                }
                else if (received_data.find("get_file") != std::string::npos)
                {
                    string file_name = get_file_name(received_data);
                    string web_string = FILE_LIST_HEADER;
                    web_string += file_read(file_name, 0, 4000);
                    web_string += WEB_MESSAGE_FOOTER;
                    const char *message = web_string.c_str();
                    clientSocket->send(message, strlen(message));
                    serial_write("Web Message = " + web_string);
                }
                else
                {
                    int eclapsed_time_us = website_timer.read_us();
                    serial_write("Eclapsed time since last connection : " + to_string(eclapsed_time_us));
                    if (eclapsed_time_us < 3000000 && eclapsed_time_us > 0)
                    {
                        serial_write("  ## To fast connection.");
                    }
                    else
                    {
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
                        website_timer.reset();
                        website_timer.start();
                    }
                }
            }
        }
        clientSocket->close();
        serial_write("Closed the connection.");
        socket_timer.reset();
        socket_timer.start();
        data_manager_mutex.unlock();
        ThisThread::sleep_for(10);
    }
}

std::string get_file_name(std::string message)
{
    int file_name_index = message.find("get_file") + string("get_file").size();
    string file_name = message.substr(file_name_index, 20);
    while (file_name.length() > 0)
    {
        if (file_name[file_name.length() - 1] != 't')
        {
            file_name.erase(file_name.length() - 1, 1);
        }
        else
        {
            break;
        }
    }
    return file_name;
}