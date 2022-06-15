/**
 * @file serial_output.h
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes serial output class related definitions.
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
 * @brief Used for controlling console output via uart.
 * 
 */
class SerialOutput
{
private:
    Thread thread;
    static Queue<std::string, 100> queue;
    static UnbufferedSerial *serial_hardware;

    /**
     * @brief Serial console output thread.
     * 
     * @details This thread is responsible from writing console output.
     */
    static void serial_output_thread();

public:
    /**
     * @brief Construct a new Serial Output object.
     * 
     */
    SerialOutput();

    /**
     * @brief Destroy the Serial Output object.
     * 
     */
    ~SerialOutput();

    /**
     * @brief Set the UART com pins.
     * 
     * @param tx_pin Transfer pin
     * @param rx_pin Receive pin
     */
    void set_pins(PinName tx_pin, PinName rx_pin);

    /**
     * @brief Starts the related thread.
     * 
     */
    void start_thread();

    /**
     * @brief Writes console message to the queue.
     * 
     * @param message 
     */
    void write(std::string message);
};

#endif