#include <msp430.h> 


/**
 * main.c
 * Austing Huang
 * Button Interrupt 5994
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |=BIT0; //set Port 1.0 output

    P5DIR &=~(BIT5); //set Port 5.5 input
    P5REN|=BIT5;//enable pull-up/pull-down resistor on
    P1OUT|=BIT1; //choose the pull-up resistor

    P5IE |=BIT5;//enable the interrupt on Port 1.1
    P5IES |=BIT5;//set as falling edge
    P5IFG &=~(BIT5);//clear interrupt flag

    //low power mode
    _BIS_SR(LPM4_bits + GIE);
}
//Port 1 ISR
#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^=0x01; // Toggle P1.1
    volatile unsigned int duration = 500;
    do (duration--);
    while (duration != 0);
    P5IFG &=~(BIT5); // Clear flag
}
