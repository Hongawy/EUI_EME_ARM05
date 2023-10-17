#include <stdbool.h>
#include <stdint.h>
#include "inc/hw_pwm.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/gpio.c"
#include "driverlib/pwm.h"
#include "driverlib/pwm.c"
#include "driverlib/debug.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"

void delayMS(int ms) {
    SysCtlDelay( (SysCtlClockGet()/(3*1000))*ms ) ;
}
int main(void)
{
    //Set the clock
    SysCtlClockSet(SYSCTL_SYSDIV_1 | SYSCTL_USE_OSC |   SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    //Configure PWM Clock to match the system clock
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    // Enable the GPIO Port F clock
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    // Enable the PWM1 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1);
    //
    // Set pins PF1 to have an alternating function (PWM pins)
    GPIOPinConfigure(GPIO_PF1_M1PWM5); //PCTL
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1); //AFSEL
    // Configure the PWM generator for count down mode with immediate updates
    // to the parameters.
    //
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2, PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC);
    //
    // Set the period. For a 50 KHz frequency, the period = 1/50,000, or 20
    // microseconds. For a 16 MHz clock, this translates to 320 clock ticks.
    // Use this value to set the period.
    //
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, 320);
    //
    // Set the pulse width of PWM1 for a 50% duty cycle.
    //
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, 100);
    //
    //
    // Start the timers in generator 2.
    //
    PWMGenEnable(PWM1_BASE, PWM_GEN_2);
    //
    // Enable the outputs.
    //
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true);
    //Fade
    bool fadeUp = true;
    unsigned long increment = 10;
    unsigned long pwmNow = 100;
    while(1)
    {
        delayMS(20);
        if (fadeUp) {
            pwmNow += increment;
            if (pwmNow >= 320) { fadeUp = false; }
        }
        else {
            pwmNow -= increment;
            if (pwmNow <= 10) { fadeUp = true; }
        }
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5,pwmNow);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_6,pwmNow);
        PWMPulseWidthSet(PWM1_BASE, PWM_OUT_7,pwmNow);
    }
}
