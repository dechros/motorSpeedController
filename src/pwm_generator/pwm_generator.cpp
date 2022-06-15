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

PwmOut *PwmGenerator::led_out;
PwmOut *PwmGenerator::pwm_out;

PwmGenerator::PwmGenerator()
{
    led_out = NULL;
    pwm_out = NULL;
}

PwmGenerator::~PwmGenerator()
{
}

void PwmGenerator::set_pwm_pins(PinName led_pin, PinName output_pin)
{
    led_out = new PwmOut(led_pin);
    pwm_out = new PwmOut(output_pin);
}

void PwmGenerator::start_thread()
{
    if (led_out == NULL || pwm_out == NULL)
    {
        char *message = "  ## Null PWM output object error.";
        serial.write(message, 34);
        while(true);
    }
    else
    {
        thread.start(PwmGenerator::pwm_generator_thread);
    }
}

void PwmGenerator::pwm_generator_thread()
{
    LED_DIRECTION led_dir = RISING_DIR;
    int period_ms = 0;

    while (true)
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
            while(true);
        }

        led_out->period_ms(period_ms);
        led_out->write(0.50f);
        pwm_out->period_ms(period_ms);
        pwm_out->write(0.50f);
        ThisThread::sleep_for(period_ms);
    }
}