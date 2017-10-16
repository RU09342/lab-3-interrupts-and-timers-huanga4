#Button Based Delay
##Author: Austin Huang  Co op: Matthew Rodriguez, Seamus Plunkett
##Summary 
An LED on the MSP430 will blink at a rate dependent on the duration a user holds down the button.

##Notes
Because this program can be run on different MSP430 boards that use different pins, the specific pin numbers are replaced with the variable "X" (i.e. PXOUT ^= BITX;)
The first thing this program does is stop the watch dog timer to ensure that the processor is not reset. This is done by the statement:
```c
    WDTCTL = WDTPW + WDTHOLD or WDTCTL = WDTPW | WDTHOLD
```
The pins are first initialized to determine which are inputs and outputs. 
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
###LED blinking.
When the timer reaches a specified value an interrupt occurs. When the timer reaches the TA0CCR0 the LED toggles.
###Button Press
When the button on the board is pressed it will cause an interrupt starting a counter that increments. This timer is tied to the timer 
that makes the LED blink. When the button is released another interupt is caused setting the TA0CCR0 value to the value of count having an upper limit of 65000.

##Differences between boards
The pin assignments on each board can be different.
Certain boards with FR require high impedence on a pin to be disabled.
Certain boards use a different timer.
Certain boards are unable to use PXSEL and need the timer connected to the LED externally.

##Implementation
Use a C compiler to run the code and upload it to an MSP430. Press the button for some amount of time, the LEDs blinking rate changes accordingly.