#include <msp430.h>
/**
 * main.c
 * Austin Huang
 * Press For Speed
 */

unsigned int player1, player2 = 0;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;
    P1SEL &= ~BIT0;
    P1DIR |= BIT0; //set Port 1.0 output ---LED
    P4SEL &= ~BIT7;
    P4DIR |= BIT7; //set Port 4.7 output ---LED
    P3SEL &= ~BIT0;
    P3DIR |= BIT0; //set Port 3.4 output ---LED

    P1DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P1REN |= BIT1; //enable pull-up resistor on
    P1OUT |= BIT1;

    P1IE |= BIT1; //enable the interrupt on Port 1.1
    P1IES &= ~BIT1; //set as falling edge
    P1IFG &= ~(BIT1); //clear interrupt flag

    P2DIR &= ~(BIT1); //set Port 1.1 input --- pushbutton
    P2REN |= BIT1; //enable pull-up resistor on
    P2OUT |= BIT1;

    P2IE |= BIT1; //enable the interrupt on Port 1.1
    P2IES &= ~BIT1; //set as falling edge
    P2IFG &= ~(BIT1); //clear interrupt flag

    TA0CTL = TASSEL_1 + MC_1;
    TA0CCTL0 = 0x10;
    TA0CCR0 = 500;

    __enable_interrupt();
    _BIS_SR(LPM4_bits + GIE);
    __no_operation();
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    if (player1 > player2)
    {
        P3OUT |= BIT0;    //On while player 1 ahead
    }
    else if(player1 < player2)
    {
        P3OUT &= ~BIT0;   //Off while player 2 ahead
    }
    else
    {
        P3OUT ^= BIT0;   //Blinks while tied
    }

    if (player1 > 60000 || player2 > 60000)
    {
        player2 = 0;
        player2 = 0;   // resets after certain amount
    }
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(100);
    P1IE |= BIT1;
    player2++;    //Modifies p2
    P4OUT ^= BIT7;
    P1IFG &= ~(BIT1);
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_IRS(void)
{
    P1IE &= ~BIT1;
    __delay_cycles(100);
    P1IE |= BIT1;
    player1++;     //Modifies p1
    P1OUT ^= BIT0;
    P2IFG &= ~(BIT1);
}
