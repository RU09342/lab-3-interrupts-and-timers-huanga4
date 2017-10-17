#include <msp430.h> 


/**
 * main.c
 * Austing Huang
 * Button Interrupt 5994
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog 

    PM5CTL0 &= ~LOCKLPM5;
    P1DIR |=BIT0; //set Port 1.0 output

    P5DIR &=~(BIT5); //set Port 5.5 input
    P5REN|=BIT5;//enable pull-up/pull-down resistor
    P1OUT|=BIT1; //choose pull-up resistor

    P5IE |=BIT5;//enable interrupt 
    P5IES |=BIT5;//set falling edge
    P5IFG &=~(BIT5);//clear flag

    _BIS_SR(LPM4_bits + GIE); //Low Power Mode 4
}

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    P1OUT ^=0x01; // Toggle
    volatile unsigned int duration = 500;
    do (duration--);
    while (duration != 0);
    P5IFG &=~(BIT5); // clear flag
}
