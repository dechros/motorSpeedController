/**
 * @file sd_manager.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes SD card manager related declerations.
 * @version 0.1
 * @date 2022-07-04
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "sd_manager.h"

BlockDevice *device;
FATFileSystem fs("fs");

Mutex sd_card;

void set_sd_card(PinName mosi, PinName miso, PinName sclk, PinName cs)
{
    sd_card.lock();
    device = new SDBlockDevice(mosi, miso, sclk, cs);
    int err = fs.mount(device);
    if (err == 0)
    {
        serial_write("SD card init OK.");
    }
    else
    {
        serial_write("  ## SD card init error!");
    }
    sd_card.unlock();
}

std::string file_read(std::string file_name, int starting_index, int read_size)
{
    sd_card.lock();
    std::string ret_str;
    file_name = "/fs/" + file_name;
    FILE *file = fopen(file_name.c_str(), READ_WRITE);
    if (file != NULL)
    {
        fseek(file, starting_index, SEEK_SET);
        while (ret_str.length() < read_size)
        {
            char temp_char = fgetc(file);
            if (temp_char < 255 && temp_char > 0)
            {
                ret_str += temp_char;
            }
            else
            {
                break;
            }
        }
        fclose(file);
    }
    else
    {
        serial_write("  ## File opening error. 3");
    }
    fflush(file);
    sd_card.unlock();
    return ret_str;
}

void file_write(std::string data, std::string file_name, int starting_index)
{
    sd_card.lock();
    file_name = "/fs/" + file_name;
    FILE *file = fopen(file_name.c_str(), WRITE_CREATE);
    if (file != NULL)
    {
        fseek(file, starting_index, SEEK_SET);
        for (int i = 0; i < data.length(); i++)
        {
            fputc(data[i], file);
        }
        fclose(file);
    }
    else
    {
        serial_write("  ## File opening error. 2");
    }
    fflush(file);
    sd_card.unlock();
}

void file_append(std::string data, std::string file_name)
{
    sd_card.lock();
    file_name = "/fs/" + file_name;
    FILE *file = fopen(file_name.c_str(), APPEND_CREATE);
    if (file != NULL)
    {
        for (int i = 0; i < data.length(); i++)
        {
            fputc(data[i], file);
        }
        fclose(file);
    }
    else
    {
        serial_write("  ## File opening error. 1");
    }
    fflush(file);
    sd_card.unlock();
}

std::vector<std::string> get_file_list(std::string dir_name)
{
    sd_card.lock();
    std::vector<std::string> ret_list;
    dir_name = "/fs/" + dir_name;
    DIR *d;
    struct dirent *dir;
    d = opendir(dir_name.c_str());
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            ret_list.push_back(std::string(dir->d_name));
        }
        closedir(d);
    }
    else
    {
        serial_write("  ## Dir opening error.");
    }
    sd_card.unlock();
    return ret_list;
}