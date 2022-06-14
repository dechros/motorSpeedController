/**
 * @file pwm_generator.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM Generator class.
 * @version 0.1
 * @date 2022-06-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "pwm_generator/pwm_generator.h"

PwmGenerator::PwmGenerator()
{
}

PwmGenerator::~PwmGenerator()
{
}

void PwmGenerator::pwm_generator_thread()
{
    while (1)
    {
        if (led_dir == RISING_DIR)
        {
            if (period_ms < 100)
            {
                period_ms++;
            }
            else
            {
                led_dir = FALLING_DIR;
            }
        }
        else if (led_dir == FALLING_DIR)
        {
            if (period_ms > 1)
            {
                period_ms--;
            }
            else
            {
                led_dir = RISING_DIR;
            }
        }
        else
        {
            led_dir = ERROR_DIR;
            char *message = "  ## Led direction error.";
            serial.write(message, 25);
        }

        led.period_ms(period_ms);
        led.write(0.50f);
        pwm.period_ms(period_ms);
        pwm.write(0.50f);
        ThisThread::sleep_for(period_ms);

        if (interrupted == true)
        {
            interrupted = false;
            char *message = "Rise\r\n";
            serial.write(message, 6);
        }
    }
}