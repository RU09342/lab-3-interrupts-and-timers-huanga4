#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Button Based Interrupt
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog 
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |=BIT0; //set Port 1.0 output
    P1DIR &=~(BIT1); //set Port 1.1 input
    P1REN|=BIT1;//enable pull-up/pull-down
    P1OUT|=BIT1; //choose pull-up 

    P1IE |=BIT1;//enable interrupt
    P1IES |=BIT1;//set falling edge
    P1IFG &=~(BIT1);//clear flag

    _BIS_SR(LPM4_bits + GIE); //Low Power Mode
}


#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; // Toggle
    //Debounce
    volatile unsigned int duration = 500;
    do (duration--);
    while (duration != 0);
    P1IFG &=~(BIT1); // clear flag
}
