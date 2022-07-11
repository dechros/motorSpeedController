/**
 * @file data_manager.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes data manager related descriptions.
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <mbed.h>
#include <EthernetInterface.h>
#include <TCPSocket.h>
#include <stdio.h>

#include "variables.h"
#include "serial_output.h"
#include "sd_manager.h"

#define REFRESH_GRAPH_HEADER "REF-"
#define FILE_LIST_HEADER "FILE-"
#define WEB_MESSAGE_FOOTER "WEB-END"

#define IP "192.168.0.31"
#define GATEWAY "192.168.0.1"
#define NETMASK "255.255.255.0"
#define PORT 80

/**
 * @brief Message type that goes to ESP32
 *
 */
enum MESAGE_TYPE
{
    ERROR_MESSAGE,
    WRITE_RPM,
    READ_RPM,
    READ_WEBSITE
};

/**
 * @brief Sets the ethernet interface.
 *
 */
void set_ethernet_interface();

/**
 * @brief Starts related data manager thread.
 *
 */
void data_manager_start_thread();

/**
 * @brief Data manager thread.
 *
 * @details This task is responsible from reading serial commands
 * from STM32 and giving ansvers.
 */
void data_managing_thread();

/**
 * @brief Strips the file name from received message.
 * 
 * @param message Received message.
 * @return std::string File name
 */
std::string get_file_name(std::string message);

#endif