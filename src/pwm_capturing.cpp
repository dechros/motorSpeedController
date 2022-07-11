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

int pulse_counter = 0;
date real_time;

Timer rpm_timer;
Thread pwm_capture_thread;
Mutex pwm_capture_mutex;
DigitalIn *pwm_in = NULL;
InterruptIn *input_interrupt = NULL;

void set_time_date()
{
    srand(time(NULL));
    real_time.day = (rand() % 31) + 1;
    real_time.hour = (rand() % 24);
    real_time.min = (rand() % 60);
    real_time.millisec = 0;

    if (real_time.day > 31 || real_time.hour > 23 || real_time.min > 59)
    {
        serial_write("  ## Time and date logic error.");
    }
    else
    {
        serial_write("Time and date is set. : Day : " + to_string(real_time.day) + " Hour : " + to_string(real_time.hour) + ":" + to_string(real_time.min));
    }
}

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
    pulse_counter++;
}

void pwm_capturing_thread()
{
    rpm_timer.start();
    while (true)
    {
        pwm_capture_mutex.lock();
        if (rpm_timer.read_ms() >= CAPTURE_PERIOD)
        {
            rpm_timer.reset();
            int rpm = pulse_counter * 600;
            pulse_counter = 0;
            rpm_mutex.lock();
            rpm_string = to_string(rpm) + "-";
            rpm_mutex.unlock();
            /* serial_write("rpm : " + to_string(rpm)); */
            std::string file_name = to_string(real_time.day) + "_" + to_string(real_time.hour) + "_" + to_string(real_time.min) + CAP_FILE_EXTENTION;
            file_append(to_string(rpm) + '-', file_name);
            update_time(CAPTURE_PERIOD);
        }
        pwm_capture_mutex.unlock();
        ThisThread::sleep_for(1);
    }
}

void update_time(int update_period)
{
    real_time.millisec += CAPTURE_PERIOD;
    if (real_time.millisec >= 60000)
    {
        real_time.millisec = 0;
        real_time.min++;
    }
    if (real_time.min >= 60)
    {
        real_time.min = 0;
        real_time.hour++;
    }
    if (real_time.hour >= 24)
    {
        real_time.hour = 0;
        real_time.day++;
    }
    if (real_time.day >= 31)
    {
        real_time.day = 1;
    }
}