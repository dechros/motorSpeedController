/**
 * @file serial_output.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes serial_hardware output class related declerations.
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "serial_output/serial_output.h"

Mutex serial_mutex;
UnbufferedSerial *serial_hardware;

void serial_set_pins(PinName tx_pin, PinName rx_pin)
{
    serial_mutex.lock();
    serial_hardware = new UnbufferedSerial(tx_pin, rx_pin);
    serial_mutex.unlock();
    serial_write("Serial pins are initialized.");
    
}

void serial_write(std::string message_p)
{
    serial_mutex.lock();
    if (serial_hardware != NULL)
    {
        message_p += "\r\n";
        const char *cstr = message_p.c_str();
        serial_hardware->write(cstr, strlen(cstr));
    }
    serial_mutex.unlock();
}