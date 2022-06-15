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

UnbufferedSerial *SerialOutput::serial_hardware;
Queue<std::string, 100> SerialOutput::queue;

SerialOutput::SerialOutput()
{
    serial_hardware = NULL;
}

SerialOutput::~SerialOutput()
{
}

void SerialOutput::set_pins(PinName tx_pin, PinName rx_pin)
{
    serial_hardware = new UnbufferedSerial(tx_pin, rx_pin);
}

void SerialOutput::start_thread()
{
    if (serial_hardware == NULL)
    {
        char *message = "  ## Null serial_hardware output object error.";
        serial_hardware->write(message, 37);
        while (true)
        {
        }
    }
    else
    {
        thread.start(SerialOutput::serial_output_thread);
    }
}

void SerialOutput::serial_output_thread()
{
    while (true)
    {
        std::string *message[1];
        bool get_result = queue.try_get(message);
        if (get_result == true)
        {
            const char *cstr = message[0]->c_str();
            for (int i = 0; i < message[0]->size(); i++)
            {
                serial_hardware->write((cstr + i), 1);
            }
        }
        else
        {
            ThisThread::yield();
        }
    }
}

void SerialOutput::write(std::string message)
{
    bool result = queue.try_put(&message);

    if (result == false)
    {
        char *message = "  ## Serial queue is full.";
        serial_hardware->write(message, 26);
    }
}