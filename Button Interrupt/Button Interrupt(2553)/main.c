#include <msp430.h> 


/**
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog 

    P1SEL =0; //select GPIO 
    P1DIR |=BIT0; //set Port 1.0 output

    P1DIR &=~(BIT3); //set Port 1.3 input
    P1REN|=BIT3;
    P1OUT|=BIT3;

    P1IE |=BIT3;//enable interrupt
    P1IES |=BIT3;//set falling edge
    P1IFG &=~(BIT3);//clear flag

    //lpm
    _BIS_SR(LPM4_bits + GIE);
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1OUT ^=0x01; // Toggle
    //Debounce
    volatile unsigned int duration = 500;
    do (duration--);
    while (duration != 0);
    P1IFG &=~(BIT3); // clear flag
}
