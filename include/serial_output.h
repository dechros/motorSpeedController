/**
 * @file serial_output.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes serial output related definitions.
 * @version 0.1
 * @date 2022-06-15
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SERIAL_OUTPUT_H
#define SERIAL_OUTPUT_H

#include <mbed.h>
#include <string>
#include <iostream>
#include <sstream>
    
/**
 * @brief Set the UART com pins.
 * 
 * @param tx_pin Transfer pin
 * @param rx_pin Receive pin
 */
void serial_set_pins(PinName tx_pin, PinName rx_pin);

/**
 * @brief Set the ESP32 UART com pins.
 * 
 * @param tx_pin Transfer pin
 * @param rx_pin Receive pin
 */
void serial_set_pins_esp(PinName tx_pin, PinName rx_pin);

/**
 * @brief Writes console message to the serial buffer.
 * 
 * @param message_p The message to be written into the console
 */
void serial_write(std::string message_p);

/**
 * @brief Writes ESP32 message to the serial buffer.
 * 
 * @param message_p The message to be written tho ESP32
 */
void serial_write_esp(std::string message_p);

/**
 * @brief Checks if ESP32 has sent any data.
 * 
 * @return true Data available
 * @return false No data
 */
bool serial_readable_esp();

/**
 * @brief Reads serial data from ESP32.
 * 
 * @return std::string Incoming message
 */
std::string serial_read_esp();

#endif