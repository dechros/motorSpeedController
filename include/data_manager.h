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
 * @brief Writes the message type that goes to ESP32
 * 
 * @param message_type Type of the message 
 * @return std::string Message type header
 */
std::string write_message_type(MESAGE_TYPE message_type);

#endif