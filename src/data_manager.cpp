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
    std::string incoming_message;
    while (true)
    {
        data_manager_mutex.lock();
        for (int i = 0; i < 500000; i += 100)
        {
            incoming_message = web_read_esp_sd("index.html", i, 100);
            serial_write("STM32 received a message : " + incoming_message);
            ThisThread::sleep_for(1000);
        }
        data_manager_mutex.unlock();
        ThisThread::sleep_for(1000);
    }
}

std::string web_read_esp_sd(std::string file_name, int starting_index, int read_size)
{
    std::string incoming_message;
    std::string sent_message;
    sent_message += READ_WEBSITE_HEADER;
    sent_message += file_name + "-";
    sent_message += to_string(starting_index) + "-";
    sent_message += to_string(read_size) + MESSAGE_FOOTER;

    int message_sent_count = 0;
    bool message_integrity = false;
    while (true)
    {
        serial_write_esp_sd(sent_message);
        serial_write("STM32 sent a message : " + sent_message);
        message_sent_count++;
        if (message_sent_count > MAX_MESSAGE_TRY)
        {
            serial_write("  ## Serial com maximum try error.");
            message_integrity = false;
            break;
        }
        if (is_message_timed_out() == true)
        {
            serial_write("  ## Serial com timeout error.");
            ThisThread::sleep_for(100);
            continue;
        }

        incoming_message = serial_read_esp_sd();
        message_integrity = check_message_integrity(incoming_message, READ_WEBSITE_HEADER, MESSAGE_FOOTER);
        if (message_integrity == false)
        {
            serial_write("  ## Serial com message integrity error. : " + incoming_message);
            ThisThread::sleep_for(100);
            continue;
        }
        else
        {
            break;
        }
    }
    if (message_integrity == true)
    {
        /* Delete footer & header */
    }
    return incoming_message;
}

bool check_message_integrity(std::string main_string, std::string sub_string_1, std::string sub_string_2)
{
    bool ret_val;
    int sub_string_1_pos = main_string.find(sub_string_1);
    int sub_string_2_pos = main_string.find(sub_string_2);
    if ((sub_string_1_pos == 0) && (sub_string_2_pos == (int)(main_string.length() - sub_string_2.length())))
    {
        ret_val = true;
    }
    else
    {
        ret_val = false;
    }
    return ret_val;
}

bool is_message_timed_out()
{
    ThisThread::sleep_for(MIN_TIMEOUT_MS);
    int wait_count = 0;
    while (serial_readable_esp_sd() == false)
    {
        ThisThread::sleep_for(MIN_TIMEOUT_MS);
        wait_count++;
        if (wait_count >= MAX_TIMEOUT_MS)
        {
            return true;
        }
    }
    return false;
}