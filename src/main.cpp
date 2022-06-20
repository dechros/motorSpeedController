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

#include "enums.h"
#include "variables.h"
#include "serial_output.h"
#include "pwm_generator.h"

void pwm_input_rise();

int main()
{
    serial_set_pins(USBTX, USBRX);
    pwm_generator_set_pins(LED1, D3);
    pwm_generator_start_thread();
    input.rise(&pwm_input_rise);
    ThisThread::sleep_for(osWaitForever);
}

void pwm_input_rise()
{
    interrupted = true;
}