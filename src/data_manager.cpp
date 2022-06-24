/**
 * @file data_manager.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes data manager related declerations.
 * @version 0.1
 * @date 2022-06-24
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "data_manager.h"

Thread data_manager_thread;
Mutex data_manager_mutex;

void data_manager_start_thread()
{
    data_manager_thread.start(data_managing_thread);
    serial_write("Data manager thread is started.");
}

void data_managing_thread()
{
    std::string message;
    while (true)
    {
        data_manager_mutex.lock();

        message = write_message_type(ERROR_MESSAGE);
        serial_write_esp(message);
        serial_write("STM32 sent a message : " + message);

        while (serial_readable_esp() == false)
        {
            ThisThread::sleep_for(1);
        }
        serial_write("STM32 received a message.");
        serial_write("Message is : " + serial_read_esp());

        message = write_message_type(WRITE_RPM);
        serial_write_esp(message);
        serial_write("STM32 sent a message : " + message);

        while (serial_readable_esp() == false)
        {
            ThisThread::sleep_for(1);
        }
        serial_write("STM32 received a message.");
        serial_write("Message is : " + serial_read_esp());

        message = write_message_type(READ_RPM);
        serial_write_esp(message);
        serial_write("STM32 sent a message : " + message);

        while (serial_readable_esp() == false)
        {
            ThisThread::sleep_for(1);
        }
        serial_write("STM32 received a message.");
        serial_write("Message is : " + serial_read_esp());

        message = write_message_type(READ_WEBSITE);
        serial_write_esp(message);
        serial_write("STM32 sent a message : " + message);

        while (serial_readable_esp() == false)
        {
            ThisThread::sleep_for(1);
        }
        serial_write("STM32 received a message.");
        serial_write("Message is : " + serial_read_esp());

        data_manager_mutex.unlock();
        ThisThread::sleep_for(100);
    }
}

std::string write_message_type(MESAGE_TYPE message_type)
{
    std::string ret_val;
    if (message_type == WRITE_RPM)
    {
        ret_val = "W-RPM-";
    }
    else if (message_type == READ_RPM)
    {
        ret_val = "R-RPM-";
    }
    else if (message_type == READ_WEBSITE)
    {
        ret_val = "R-WEB-";
    }
    else
    {
        ret_val = "ERROR";
    }
    return ret_val;
}