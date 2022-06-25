/**
 * @file main.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief Program entry file.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mbed.h>
#include <EthernetInterface.h>
#include <TCPSocket.h>
#include <stdio.h>
#include <iostream>

#include "enums.h"
#include "variables.h"
#include "serial_output.h"
#include "pwm_generator.h"
#include "pwm_capturing.h"
#include "data_manager.h"

#define IP "192.168.0.31"
#define GATEWAY "192.168.0.1"
#define NETMASK "255.255.255.0"
#define PORT 80

EthernetInterface *net = NULL;
TCPSocket server;
TCPSocket *clientSocket = NULL;
TCPSocket *clientSocketOld = NULL;
SocketAddress clientAddress;
char rxBuf[512] = {0};
char txBuf[512] = {0};

DigitalOut internet_led(LED2);

void ethernet_loop();

int main()
{
    internet_led.write(1);
    serial_set_pins(USBTX, USBRX);
    serial_set_pins_esp_sd(D1, D0);
    data_manager_start_thread();
    pwm_generator_set_pins(LED1, D3);
    pwm_generator_start_thread();
    pwm_capturing_set_pin(D8);
    pwm_capturing_start_thread();
    /* ethernet_loop(); */
    ThisThread::sleep_for(osWaitForever);
}

void ethernet_loop()
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

    SocketAddress ip;
    SocketAddress netmask;
    SocketAddress gateway;
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

    while (true)
    {
        nsapi_error_t error = 0;

        clientSocket = server.accept(&error);
        if (error != 0)
        {
            serial_write("  ## Connection error.");
        }
        else
        {
            clientSocket->getpeername(&clientAddress);
            string client_ip(clientAddress.get_ip_address());
            serial_write("Connected. IP : " + client_ip);
            error = clientSocket->recv(rxBuf, sizeof(rxBuf));
            if (error > 0)
            {
                string received_data(rxBuf);
                string led_toggle_command = "led_toggle";
                serial_write("Received Data : " + received_data);
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
                    string web_button = "<button onclick=\"location.href='http://192.168.0.31/led_toggle'\" type=\"button\"> Click Me </button>";
                    const char *message = web_button.c_str();
                    clientSocket->send(message, strlen(message));
                }
            }
            clientSocket->close();
            serial_write("Closed the connection.");
        }
        ThisThread::yield();
    }
}