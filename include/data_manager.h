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

#define WRITE_RPM_HEADER "W-RPM-"
#define READ_RPM_HEADER "R-RPM-"
#define READ_WEBSITE_HEADER "R-WEB-"
#define ERROR_MESSAGE_HEADER "ERROR-"
#define MESSAGE_FOOTER "-H-END"
#define WEB_MESSAGE_FOOTER "WEB-END"
#define REFRESH_GRAPH_HEADER "REF-"

#define MIN_TIMEOUT_MS 100
#define MAX_TIMEOUT_COUNT 2
#define MAX_MESSAGE_TRY 10

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
 * @brief Reads a web file from ESP32 SD controller.
 * 
 * @param file_name File to be read
 * @param starting_index File reading starting index
 * @param read_size File read char size
 * @return std::string 
 */
std::string web_read_esp_sd(std::string website_name, int starting_index, int read_size);

/**
 * @brief Writes the message type that goes to ESP32 SD readerç
 *
 * @param message_type Type of the message
 * @return std::string Message type header
 */
std::string write_message_type_header(MESAGE_TYPE message_type);

/**
 * @brief Strips the message from header and footer.
 * 
 * @param main_string Incoming message
 * @param header Search string
 * @param footer Search string
 * @return std::string Stripped message
 */
std::string strip_the_message(std::string main_string, std::string header, std::string footer);

/**
 * @brief Checks if mesage is correct.
 * 
 * @param main_string Incoming message
 * @param header Search string
 * @param footer Search string
 * @return true Message is correct
 * @return false Message is not correct
 */
bool check_message_integrity(std::string main_string, std::string header, std::string footer);

/**
 * @brief Waits until timeout. Use this after sendinga message.
 * 
 * @return true Message is timed out
 * @return false Message is not timed out
 */
bool is_message_timed_out();

#endif