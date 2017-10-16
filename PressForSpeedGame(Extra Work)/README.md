#Press For Speed Game
##Author: Austin Huang  Co op: Matthew Rodriguez, Seamus Plunkett
##Summary 
An LED on the board wil blink at a contant rate, another LED will blink a constant different rate.

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
    PXDIR &= ~BITX;                        // Set P5.6 as input
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
Each player will have a score that is stored in a field variable. The player with the higher score is winning. The scores determine the state of an LEDs.
###LED state
Timer A is set with a CCR0 value of 500. This means every 500 cycles an interrupt occurs. Everytime this intrerupt occurs a block of code that
compares the two players' scores. The scores represent the number of taps on the button. If player 1 is winning, the LED will be on.
If player 2 is winning, the LED will be off. If the two players have equal score the LED will blink.
###Button Press
When a button is pressed on the board an interrupt is caused that increments its respective player's score.

##Differences between boards
The pin assignments on each board can be different.
Certain boards with FR require high impedence on a pin to be disabled.
Certain boards use a different timer.
Certain boards are unable to use PXSEL and need the timer connected to the LED externally.

##Implementation 
Use a C compiler to run the code and upload it to an MSP430. Player one and player two may press their respective buttons to increase their "score".
While player one is winning an LED will shine, while player 2 is winning the LED will not shine. A tie will result in the LED blinking rapidly.