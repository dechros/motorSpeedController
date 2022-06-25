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

#include "variables.h"
#include "serial_output.h"

#define WRITE_RPM_HEADER "W-RPM-"
#define READ_RPM_HEADER "R-RPM-"
#define READ_WEBSITE_HEADER "R-WEB-"
#define ERROR_MESSAGE_HEADER "ERROR-"
#define MESSAGE_FOOTER "-H"

#define MIN_TIMEOUT_MS 1
#define MAX_TIMEOUT_MS 250
#define MAX_MESSAGE_TRY 10

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
 * @brief Checks if mesage is correct.
 * 
 * @param main_string Incoming message
 * @param sub_string_1 Search string
 * @param sub_string_2 Search string
 * @return true Message is correct
 * @return false Message is not correct
 */
bool check_message_integrity(std::string main_string, std::string sub_string_1, std::string sub_string_2);

/**
 * @brief Waits until timeout. Use this after sendinga message.
 * 
 * @return true Message is timed out
 * @return false Message is not timed out
 */
bool is_message_timed_out();

#endif