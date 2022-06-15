/**
 * @file main.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief Program entry file.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include <mbed.h>

#include "globals/enums.h"
#include "globals/variables.h"
#include "pwm_generator/pwm_generator.h"
#include "serial_output/serial_output.h"

void pwm_input_rise();

int main()
{
    SerialOutput serial_output;
    serial_output.set_pins(USBTX, USBRX);
    serial_output.start_thread();

    PwmGenerator pwm_generator;
    pwm_generator.set_pins(LED1, D3);
    pwm_generator.set_serial_output(&serial_output);
    pwm_generator.start_thread();

    input.rise(&pwm_input_rise);
    while (true)
    {
        ThisThread::yield();
    }
}

void pwm_input_rise()
{
    interrupted = true;
}