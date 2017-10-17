#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Button Interrupt 5529
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog

    P1SEL =0;
    P1DIR |=BIT0;

    P1DIR &=~(BIT1);
    P1REN|=BIT1;//enable pull-up/pull-down resistor
    P1OUT|=BIT1; //choose pull-up 

    P1IE |=BIT1;//enable interrupt
    P1IES |=BIT1;//set falling edge
    P1IFG &=~(BIT1);//clear flag

    
    _BIS_SR(LPM4_bits + GIE); //low power mode
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
