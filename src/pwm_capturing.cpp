/**
 * @file pwm_capturing.cpp
 * @author Halit Cetin (halit.cetin@alten.com)
 * @brief This file includes PWM capturing related declerations.
 * @version 0.1
 * @date 2022-06-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */

#include "pwm_capturing.h"

bool interrupted = false;

Thread pwm_capture_thread;
Mutex pwm_capture_mutex;
DigitalIn *pwm_in = NULL;
InterruptIn *input_interrupt = NULL;

void pwm_capturing_set_pin(PinName input_pin)
{
    pwm_capture_mutex.lock();
    pwm_in = new DigitalIn(input_pin);
    input_interrupt = new InterruptIn(input_pin);
    input_interrupt->rise(&pwm_input_rise);
    pwm_capture_mutex.unlock();
    serial_write("PWM input pins are initialized.");
}

void pwm_capturing_start_thread()
{
    if (pwm_in == NULL || input_interrupt == NULL)
    {
        serial_write("  ## Null pin error.");
    }
    else
    {
        pwm_capture_thread.start(pwm_capturing_thread);
        serial_write("PWM capturing thread is started.");
    }
}

void pwm_input_rise()
{
    interrupted = true;
}

void pwm_capturing_thread()
{
    while (true)
    {
        pwm_capture_mutex.lock();
        if (interrupted == true)
        {
            interrupted = false;
            serial_write("Pulse.");
        }
        pwm_capture_mutex.unlock();
        ThisThread::yield();
    }
}