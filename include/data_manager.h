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
#include <SDBlockDevice.h>
#include <FATFileSystem.h>

#include "variables.h"
#include "serial_output.h"

#define REFRESH_GRAPH_HEADER "REF-"
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
 * @brief Sets the ethernet interface,
 * 
 */
void set_ethernet_interface();

/**
 * @brief Sets SD card SPI interface
 * 
 * @param mosi Pin
 * @param miso Pin
 * @param sclk Pin
 * @param cs Pin
 */
void set_sd_card(PinName mosi, PinName miso, PinName sclk, PinName cs);

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
 * @brief Sends file read request to ESP32 SD reader.
 *
 * @param website_name Website name to be read
 */

/**
 * @brief Reads file from SD card.
 * 
 * @param file_name File to be read
 * @param starting_index File reading starting index
 * @param read_size File read char size
 * @return std::string 
 */
std::string file_read(std::string website_name, int starting_index, int read_size);

#endif