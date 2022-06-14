#include <mbed.h>
#include <string>

#include "globals/enums.h"
#include "globals/variables.h"
#include "pwm_generator/pwm_generator.h"

void pwm_input_rise();

int main()
{
    input.rise(&pwm_input_rise);
    pwm_generator_thread.start(PwmGenerator::pwm_generator_thread);
    while (true)
    {
        ThisThread::yield();
    }
}

void pwm_input_rise()
{
    interrupted = true;
}