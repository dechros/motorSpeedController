/**
 * @file sd_manager.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes SD card manager related descriptions.
 * @version 0.1
 * @date 2022-07-04
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#ifndef SD_MANAGER_H
#define SD_MANAGER_H

#include <mbed.h>
#include <vector>
#include <SDBlockDevice.h>
#include <FATFileSystem.h>

#include "variables.h"
#include "serial_output.h"

#define READ_WRITE "r+"
#define WRITE_CREATE "w"
#define APPEND_CREATE "a"

/**
 * @brief Sets SD card SPI interface
 *
 * @param mosi Pin
 * @param miso Pin
 * @param sclk Pin
 * @param cs Pin
 */
void set_sd_card(PinName mosi, PinName miso, PinName sclk, PinName cs);

/**
 * @brief Reads file from SD card.
 *
 * @param file_name File to be read
 * @param starting_index File reading starting index
 * @param read_size File read char size
 * @return std::string
 */
std::string file_read(std::string website_name, int starting_index, int read_size);

/**
 * @brief Writes the data to the file.
 * 
 * @param data Data to be written
 * @param file_name File to be written
 * @param starting_index File writing starting index
 */
void file_write(std::string data, std::string file_name, int starting_index);

/**
 * @brief Writes data to the end of the file.
 * 
 * @param data Data to be written
 * @param file_name File to be written
 */
void file_append(std::string data, std::string file_name);
/**
 * @brief Gets the file list
 * 
 * @param dir_name Directory name to be listed all of its files 
 * @return std::vector<std::string> File list
 */
std::vector<std::string> get_file_list(std::string dir_name);

#endif