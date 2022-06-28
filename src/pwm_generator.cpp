/**
 * @file pwm_generator.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM generator related declerations.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pwm_generator.h"

#define PWM_PERIOD_MAX 8000
#define PWM_PERIOD_MIN 2000

Thread pwm_generate_thread;
Mutex pwm_generate_mutex;
PwmOut *led_pwm_out = NULL;
PwmOut *cable_pwm_out = NULL;

void pwm_generator_set_pins(PinName led_pin, PinName cable_pin)
{
    pwm_generate_mutex.lock();
    led_pwm_out = new PwmOut(led_pin);
    cable_pwm_out = new PwmOut(cable_pin);
    pwm_generate_mutex.unlock();
    serial_write("PWM generator pins are initialized.");
}

void pwm_generator_start_thread()
{
    if (led_pwm_out == NULL || cable_pwm_out == NULL)
    {
        serial_write("  ## Null pin error.");
    }
    else
    {
        pwm_generate_thread.start(pwm_generator_thread);
        serial_write("PWM generator thread is started.");
    }
}

void pwm_generator_thread()
{
    PWM_PERIOD_DIR pwm_dir = RISING_DIR;
    int pwm_period_us = PWM_PERIOD_MIN;

    while (true)
    {
        pwm_generate_mutex.lock();
        if (pwm_dir == RISING_DIR)
        {
            if (pwm_period_us < PWM_PERIOD_MAX)
            {
                pwm_period_us += 1000;
            }
            else
            {
                pwm_dir = FALLING_DIR;
            }
        }
        else if (pwm_dir == FALLING_DIR)
        {
            if (pwm_period_us > PWM_PERIOD_MIN)
            {
                pwm_period_us -= 1000;
            }
            else
            {
                pwm_dir = RISING_DIR;
            }
        }
        else
        {
            serial_write("  ## Led direction error.");
        }
        led_pwm_out->period_us(pwm_period_us);
        led_pwm_out->write(0.50f);
        cable_pwm_out->period_us(pwm_period_us);
        cable_pwm_out->write(0.50f);
        pwm_generate_mutex.unlock();
        ThisThread::sleep_for(100);
    }
}