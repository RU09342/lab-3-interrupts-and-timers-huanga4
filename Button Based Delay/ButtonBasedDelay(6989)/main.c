#include <msp430.h>

/**
 * main.c
 * Austin Huang
 * ButtonBasedDlay
 */

int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1DIR &= ~(BIT1);
    P1REN |= BIT1;
    P1OUT |= BIT1;


    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES |= BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1 + ID_1; //Set up Timer A, Count up, and divider 2.
    TA0CCTL0 = 0x10; //Set up compare mode for CCTL
    TA0CCR0 = 6000; // LED will blink at 32kHZ*2/6000 = 10.6 Hz

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // Enter Low Power Mode 4

}

#pragma vector=TIMER0_A0_VECTOR
    __interrupt void Timer_A0(void)
{

    P1OUT ^= 0x01;

}


// Timer0 Interrupt Service Routine
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debouncing
    P1IE &= ~BIT1;
    __delay_cycles(1);

//If statement for falling edge of ButtonPress
if (buttonPress == 0)
    {
        TA1CTL = TASSEL_1 + MC_3; // Selecting Timer A and Count Continuous
        TA1CCR0 = 0xFFFF; //Initialize value of TA1CCR0
        TA1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TA0CCR0 = 1; //Reset CCR0

    }

    //If statement for rising edge of ButtonPress
    else if (buttonPress == 1)
    {
        TA1CTL = MC_0; //Stop Counting
        TA0CCR0 = TA1R; //Assign new value for CCR0

        if (TA0CCR0 > 65500) //Slowest
            TA0CCR0 = 0xFFFF;
        if (TA0CCR0 < 1000) // Fastest
            TA0CCR0 = 1000;

        TA1CTL = TACLR; //Clear Timer A1
        buttonPress = 0;
    }

    //Service routine
    P1IES ^= BIT1;
    P1IE |= BIT1;
    P1IFG &= ~(BIT1);
}
