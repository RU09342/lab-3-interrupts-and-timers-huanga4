#include <msp430.h>

/**
 * main.c
 * Austin Huang
 * 2311 ButtonBasedDelay
 */

int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog
    PM5CTL0 &= ~LOCKLPM5;

    P1DIR |= BIT0; //set Port 1.0 LED
    P1DIR &= ~(BIT1); //set Port 1.1 Button
    P1REN |= BIT1; //enable pull-up resistor
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable interrupt
    P1IES |= BIT1; //set falling edge
    P1IFG &= ~(BIT1); //clear flag

    TB0CTL = TBSSEL_1 + MC_1 + ID_2; //set Timer A, Up mode, divider value 4
    TB0CCTL0 = 0x10; //set compare mode for CCTL
    TB0CCR0 = 1600; // LED blinks at 32kHZ*2/1600 or 10 Hz

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // enter Low Power Mode 4
}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{
    P1OUT ^= 0x01; //Toggle
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debounce
    P1IE &= ~BIT1;
    __delay_cycles(1);

    if (buttonPress == 0){ // Falling edge
        TB1CTL = TBSSEL_1+ MC_3; // select Timer A, Continuous
        TB1CCR0 = 0xFFFF; //initialize TA1CCR0
        TB1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TB0CCR0 = 1; //reset CCR0
    }
    else if (buttonPress == 1){ //rising edge
        TB1CTL = MC_0; // stop counting
        TB0CCR0 = TB1R; //new CCR0 value
        if (TB0CCR0 > 65500){
            TB0CCR0 = 0xFFFF;
        }
        if (TB0CCR0 < 3000){
            TB0CCR0 = 3000;
        }
        TB1CTL = TBCLR; //clear TimerA1
        buttonPress = 0;
    }

    P1IES ^= BIT1;
    P1IE |= BIT1;
    P1IFG &= ~(BIT1);
}
