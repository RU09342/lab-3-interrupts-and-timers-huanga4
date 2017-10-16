#include <msp430.h>

/**
 * main.c
 * Austin Huang
 * 2311 ButtonBasedDelay
 */

int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;


    P1DIR |= BIT0; //set Port 1.0 output ---LED
    P1DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN |= BIT1; //enable pull-up resistor
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES |= BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    TB0CTL = TBSSEL_1 + MC_1 + ID_2; //Set up Timer A, Count up, and divider 4.
    TB0CCTL0 = 0x10; //Set up compare mode for CCTL
    TB0CCR0 = 1600; // LED will blink at 32kHZ*2/1600 = 10 Hz

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // Enter Low Power Mode 4
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{

    P1OUT ^= 0x01; //Toggle LED

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debouncing
    P1IE &= ~BIT1;
    __delay_cycles(1);

    if (buttonPress == 0) //Falling-edge of a button
    {
        TB1CTL = TBSSEL_1+ MC_3; // Selecting Timer A and Count Continuous
        TB1CCR0 = 0xFFFF; //Initialize value of TA1CCR0
        TB1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TB0CCR0 = 1; //Reset CCR0

    }
    else if (buttonPress == 1) //Rising-edge of a button
    {
        TB1CTL = MC_0; //Stop Counting
        TB0CCR0 = TB1R; //Assign new value for CCR0
        if (TB0CCR0 > 65500)
            TB0CCR0 = 0xFFFF;
        if (TB0CCR0 < 3000)
            TB0CCR0 = 3000;
        TB1CTL = TBCLR; //Clear Timer A1
        buttonPress = 0;
    }

    P1IES ^= BIT1;
    P1IE |= BIT1;
    P1IFG &= ~(BIT1);
}
