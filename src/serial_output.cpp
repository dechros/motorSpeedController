/**
 * @file serial_output.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes serial output related declerations.
 * @version 0.1
 * @date 2022-06-15
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "serial_output.h"

Mutex serial_mutex;
Mutex serial_mutex_esp_sd;
UnbufferedSerial *serial_hardware;
BufferedSerial *serial_hardware_esp_sd;

void serial_set_pins(PinName tx_pin, PinName rx_pin)
{
    serial_mutex.lock();
    serial_hardware = new UnbufferedSerial(tx_pin, rx_pin);
    serial_mutex.unlock();
    serial_write("Serial pins are initialized.");
}

void serial_set_pins_esp_sd(PinName tx_pin, PinName rx_pin)
{
    serial_mutex_esp_sd.lock();
    serial_hardware_esp_sd = new BufferedSerial(tx_pin, rx_pin, 115200);
    serial_mutex_esp_sd.unlock();
    serial_write("ES32 SD Serial pins are initialized.");
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

void serial_write_esp_sd(std::string message_p)
{
    serial_mutex_esp_sd.lock();
    if (serial_hardware_esp_sd != NULL)
    {
        for (int i = 0; i < message_p.size(); i++)
        {
            serial_hardware_esp_sd->write((char *)&message_p[i], 1);
        }
    }
    serial_mutex_esp_sd.unlock();
}

bool serial_readable_esp_sd()
{
    serial_mutex_esp_sd.lock();
    bool ret_val = serial_hardware_esp_sd->readable();
    serial_mutex_esp_sd.unlock();
    return ret_val;
}

std::string serial_read_esp_sd()
{
    serial_mutex_esp_sd.lock();
    std::string ret_str = "  ## Null serial_hardware_esp_sd error.";
    if (serial_hardware_esp_sd != NULL)
    {
        ret_str = "";
        while (serial_hardware_esp_sd->readable() == true)
        {
            char incoming_char;
            serial_hardware_esp_sd->read(&incoming_char, 1);
            ret_str += incoming_char;
        }
    }
    serial_mutex_esp_sd.unlock();
    return ret_str;
}