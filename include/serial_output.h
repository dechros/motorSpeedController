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
    
/**
 * @brief Set the UART com pins.
 * 
 * @param tx_pin Transfer pin
 * @param rx_pin Receive pin
 */
void serial_set_pins(PinName tx_pin, PinName rx_pin);

/**
 * @brief Writes console message to the serial buffer.
 * 
 * @param message_p The message to be written into the console.
 */
void serial_write(std::string message_p);

#endif