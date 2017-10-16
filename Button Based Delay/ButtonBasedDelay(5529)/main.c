#include <msp430.h>

/**
 * main.c
 */

unsigned long count = 0;
int buttonPressed, contCount = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1SEL &= ~BIT0;
    P1DIR |= BIT0; //set Port 1.0 output ---LED
    P4SEL &= ~BIT7;
    P4DIR |= BIT7; //set Port 4.6 output ---LED

    P1DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN |= BIT1; //enable pull-up resistor on
    P1OUT |= BIT1;
    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES |= BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1;
    TA0CCTL0 = 0x10;
    TA0CCR0 = 12000;

    P4OUT &= ~BIT7;


    __enable_interrupt();
    _BIS_SR(LPM4_bits + GIE);
    }

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{

    P1OUT ^= 0x01;
    count++;
    if(count > 65000){
        count = 65000;
    }

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(1);

    contCount ^= 1;

    if (buttonPressed == 0)
    {
        count = 3000;
        buttonPressed = 1;
        P4OUT |= BIT7;
        TA0CCR0 = 1;
    }
    else if (buttonPressed == 1)
    {
        TA0CCR0 = count;

        buttonPressed = 0;
        P4OUT &= ~BIT7;
    }

    contCount ^= 1;
    P1IES ^= BIT1; //toggle to set as rising edge
    P1IE |= BIT1;
    P1IFG &= ~(BIT1);
}
