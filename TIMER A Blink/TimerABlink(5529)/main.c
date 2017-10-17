#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Timer A
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog 

    P1SEL =0; //choose GPIO option 
    P1OUT &= ~(BIT0); // Clear P1.0
    P1DIR |=BIT0; //set Port 1.0 LED

    P4SEL =0; //choose GPIO
    P4OUT &= ~(BIT7);
    P4DIR |=BIT7; //set Port 4.7 output LED


    TA0CTL= TASSEL_1 + MC_1; //Set up Timer A0, Up mode, ACLK
    TA0CCTL0 = 0x10; //set up compare mode
    TA0CCR0 = 12000; //set up TB0CCR0; LED frequency = 32kHz/12000 
    TA0CCTL1 = 0x10; //set up compare mode
    TA0CCR1 = 3000; // CCR1 assigned value
  
    _BIS_SR(LPM4_bits + GIE); //Low Power Mode 4
}

#pragma vector=TIMER0_A0_VECTOR
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT ^= 0x01; //Toggle
    switch (TA0IV)  //choose correct value for corresponding interrupt
    {
        case TA0IV_TACCR1: P4OUT ^= BIT7; //Toggle
        break;
    }
}
