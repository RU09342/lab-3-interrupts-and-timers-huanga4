#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Timer A Blink
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog

    P1OUT &= ~(BIT0); // Clear P1.0
    P1DIR |=BIT0; //set Port 1.0 LED

    P1OUT &= ~(BIT6); //Clear
    P1DIR |=BIT6; //set Port 1.6 LED

    TA0CTL= TASSEL_1 + MC_1; //Set up Timer B0, Up mode, ACLK
    TA0CCTL0 = 0x10; //set up compare mode
    TA0CCR0 = 12000; //assign TA0CCR0; LED frequency = 32kHz/12000 
    TA0CCTL1 = 0x10; // Set up compare mode
    TA0CCR1 = 3000; // assign TA0CCR1; LED frequency = 32kHz/3000
    
    _BIS_SR(LPM4_bits + GIE);  //Low Power Mode 4
}

#pragma vector=TIMER0_A0_VECTOR
#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
    P1OUT ^= 0x01; //Toggle
    switch (TA0IV) //choose correct value for corresponding interrupt
    {
        case TA0IV_TACCR1: P1OUT ^= BIT6; //Toggle 
        break;
    }
}
