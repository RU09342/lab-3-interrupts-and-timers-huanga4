#Timer A Blink
##Author: Austin Huang   Co op: Matthew Rodriguez, Seamus Plunkett
##Summary 
An LED on the board blinks at a contant rate, while another LED blinks at a different rate.

##Notes
Because this program can be run on different MSP430 boards that use different pins, the specific pin numbers are replaced with the variable "X" (i.e. PXOUT ^= BITX;)
The first thing this program does is stop the watch dog timer to ensure that the processor is not reset. This is done by the statement:
```c
    WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD
```
The pins are initialized to determine which are inputs and outputs. 
Example:
```c
    PXDIR |= BITX;                          // Set P1.0 to output direction
    PXOUT &= ~BITX;                         // Switch LED off
    PXDIR  &= ~BITX;                        // Set P5.6 as input
    PXOUT |= BITX;                          // Configure P5.6 for Pull-Up
    PXREN |= BITX;                          // Enable Pull Up of P5.6
```
Timer is then initialized.
Example:
```c
    TA0CTL = TASSEL_2 + MC_1 +TACLR ;
    TA0CCTL1 = OUTMOD_7;
    TA0CCR0 = 1000;
    TA0CCR1 = 500;
```
##How it works
When the timer reaches a certain value an interupt occurs. When the timer reaches the TA0CCR0 the LED will toggle on or off, after a certain amount of time
based on how many times that interupt is caused the next LED will toggle on or off.

##Differences between boards
The pin assignments on each board can be different.
Certain boards with FR require high impedence on a pin to be disabled.
Certain boards use a different timer.
Certain boards are unable to use PXSEL and need the timer connected to the LED externally.


##Implementation
Use a C compiler to run the code and upload it to an MSP430. The two LEDs blink at different rates.