#include <msp430.h>

/**
 * main.c
 * Austin Huang
 * ButtonBasedDlay
 */

int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog 

    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0;
    P1DIR &= ~(BIT1);
    P1REN |= BIT1;
    P1OUT |= BIT1;


    P1IE |= BIT1; //enable interrupt 
    P1IES |= BIT1; //set falling edge
    P1IFG &= ~(BIT1); //clear flag

    TA0CTL = TASSEL_1 + MC_1 + ID_1; //set up Timer A, Up mode, divider value 2
    TA0CCTL0 = 0x10; //set compare mode for CCTL
    TA0CCR0 = 6000; // LED blinks at 32kHZ*2/6000 

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // Enter Low Power Mode 4

}

#pragma vector=TIMER0_A0_VECTOR
    __interrupt void Timer_A0(void)
{
    P1OUT ^= 0x01;
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debounce
    P1IE &= ~BIT1;
    __delay_cycles(1);


    if (buttonPress == 0){  //falling edge
        TA1CTL = TASSEL_1 + MC_3; // Selecting Timer A and Count Continuous
        TA1CCR0 = 0xFFFF; //Initialize value of TA1CCR0
        TA1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TA0CCR0 = 1; //Reset CCR0
    } 
    else if (buttonPress == 1){  //rising edge
        TA1CTL = MC_0; //stop counting
        TA0CCR0 = TA1R; //new CCR0 value
        if (TA0CCR0 > 65500){ 
            TA0CCR0 = 0xFFFF;
        }
        if (TA0CCR0 < 1000){
            TA0CCR0 = 1000;
        }
        TA1CTL = TACLR; //clear TimerA1
        buttonPress = 0;
    }

    P1IES ^= BIT1;
    P1IE |= BIT1;
    P1IFG &= ~(BIT1);
}
