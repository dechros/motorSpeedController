/**
 * @file pwm_generator.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM Generator class related declerations.
 * @version 0.1
 * @date 2022-06-14
 *
 * @copyright Copyright (c) 2022
 *
 */

#include "pwm_generator/pwm_generator.h"

Thread thread;
Mutex pwm_mutex;
PwmOut *led_pwm_out;
PwmOut *cable_pwm_out;

void pwm_set_pins(PinName led_pin, PinName cable_pin)
{
    pwm_mutex.lock();
    led_pwm_out = new PwmOut(led_pin);
    cable_pwm_out = new PwmOut(cable_pin);
    pwm_mutex.unlock();
    serial_write("PWM pins are initialized.");
}

void pwm_start_thread()
{
    thread.start(pwm_thread);
    serial_write("PWM generator thread is started.");
}

void pwm_thread()
{
    PWM_PERIOD_DIR pwm_dir = RISING_DIR;
    int pwm_period_ms = 0;

    while (true)
    {
        pwm_mutex.lock();
        if (pwm_dir == RISING_DIR)
        {
            if (pwm_period_ms < 100)
            {
                pwm_period_ms++;
            }
            else
            {
                pwm_dir = FALLING_DIR;
            }
        }
        else if (pwm_dir == FALLING_DIR)
        {
            if (pwm_period_ms > 1)
            {
                pwm_period_ms--;
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
        led_pwm_out->period_ms(pwm_period_ms);
        led_pwm_out->write(0.50f);
        cable_pwm_out->period_ms(pwm_period_ms);
        cable_pwm_out->write(0.50f);
        pwm_mutex.unlock();
        ThisThread::sleep_for(pwm_period_ms);
    }
}