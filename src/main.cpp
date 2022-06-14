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
    Thread pwm_generator_thread;
    PwmGenerator pwm_generator;
    input.rise(&pwm_input_rise);
    pwm_generator_thread.start(PwmGenerator::pwm_generator_thread);
    ThisThread::yield();
    char *message = "  ## Software error.";
    serial.write(message, 20);
}

void pwm_input_rise()
{
    interrupted = true;
}