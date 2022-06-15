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
#include <string>

#include "globals/enums.h"
#include "globals/variables.h"
#include "pwm_generator/pwm_generator.h"

void pwm_input_rise();

int main()
{
    BufferedSerial serial(USBTX, USBRX);
    PwmGenerator pwm_generator;
    pwm_generator.set_pwm_pins(LED1, D3);
    pwm_generator.set_serial_output(&serial);
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