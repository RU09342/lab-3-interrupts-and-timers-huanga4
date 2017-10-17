#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * Timer A Blink
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;// Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            
    P1OUT &= ~(BIT0); // Clear P1.0
    P1DIR |=BIT0; //set Port 1.0 LED

    P2OUT &= ~(BIT0); //Clear P2.0
    P2DIR |=BIT0; //set Port 2.0 LED

    TB0CTL= TBSSEL_1 + MC_1; //Set up Timer B0, Up mode, ACLK
    TB0CCTL0 = 0x10; //set up compare mode
    TB0CCR0 = 12000; //set up TB0CCR0; LED frequency = 32kHz/12000 
    TB0CCTL1 = 0x10;    // Set up compare mode
    TB0CCR1 = 3000;     // CCR1 assigned value
    
    _BIS_SR(LPM4_bits + GIE); //Low Power Mode 4

}

#pragma vector=TIMER0_B0_VECTOR
#pragma vector = TIMER0_B1_VECTOR
__interrupt void Timer_B(void)
{
    P1OUT ^= 0x01; //Toggle 
    switch (TB0IV) //choose correct value for corresponding interrupt
    {
    case TB0IV_TBCCR1: P2OUT ^= BIT0; //Toggle
    break;
    }
}
