#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Button Interrupt 5529
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    P1SEL =0;
    P1DIR |=BIT0;

    P1DIR &=~(BIT1);
    P1REN|=BIT1;//enable pull-up/pull-down resistor on
    P1OUT|=BIT1; //choose the pull-up resistor

    P1IE |=BIT1;//enable the interrupt on Port 1.1
    P1IES |=BIT1;//set as falling edge
    P1IFG &=~(BIT1);//clear interrupt flag

    //enter low power mode
    _BIS_SR(LPM4_bits + GIE);
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; // Toggle P1.1
    //Debouncing
    volatile unsigned int duration = 500;
    do (duration--);
    while (duration != 0);
    P1IFG &=~(BIT1); // Clear flag
}
