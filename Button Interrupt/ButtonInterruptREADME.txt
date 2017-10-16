#Button Interupt
##Author: Austin Huang   Co op: Matthew Rodriguez, Seamus Plunkett
##Summary 
An LED's state(on or off) is changed when the button is pressed.

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

##How it works
When the button on the board is pressed an interrupt occurs. This executes a block of code that toggles the state of an LED.

##Differences between boards
The pin assignments on each board can be different.
Certain boards with FR require high impedence on a pin to be disabled.
Certain boards use a different timer.
Certain boards are unable to use PXSEL and need the timer connected to the LED externally.

##Implementation
Use a C compiler to run the code and upload it to an MSP430. Press the button and the LED changes states.