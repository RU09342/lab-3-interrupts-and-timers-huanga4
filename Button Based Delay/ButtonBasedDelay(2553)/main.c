#include <msp430.h> 


/**
 * Austin Huang
 * main.c
 * Button Based Delay
*/

int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog 

    P1SEL &= ~BIT0;
    P1DIR |= BIT0;
    P1OUT &= ~(BIT0);
    P1DIR &= ~(BIT3);
    P1REN |= BIT3;
    P1OUT |= BIT3;


    P1IE |= BIT3; //enable interrupt
    P1IES |= BIT3; //set falling edge
    P1IFG &= ~(BIT3); //clear flag

    TA0CTL = TASSEL_1 + MC_1 + ID_2; //setTimer A, Up mode, divider value 4
    TA0CCTL0 = 0x10; //set compare mode for CCTL
    TA0CCR0 = 1600; // LED blinks at 32kHZ*2/1600 or 10.6 Hz
    
    _BIS_SR(LPM0_bits + GIE);  // enter LPM0
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1OUT ^= 0x01; //Toggle 

}

#pragma vector=PORT1_VECTOR
__interrupt void PORT_1(void)
{
    //Debounce
    P1IE &= ~BIT3;
    __delay_cycles(1);

if (buttonPress == 0) //Falling edge
{
        TA1CTL = TASSEL_1+ MC_3; // select Timer A, Continuous
        TA1CCR0 = 0xFFFF; /initialize TA1CCR0
        TA1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TA0CCR0 = 1; //reset CCR0

    }
    else if (buttonPress == 1) //rising edge
    {
        TA1CTL = MC_0; // stop counting
        TA0CCR0 = TA1R; //new CCR0 value
        if (TA0CCR0 > 65500) 
            TA0CCR0 = 0xFFFF;
        if (TA0CCR0 < 3000) 
            TA0CCR0 = 3000;
        TA1CTL = TACLR; //clear Timer A1
        buttonPress = 0;
    }

    P1IES ^= BIT3;
    P1IE |= BIT3;
    P1IFG &= ~(BIT3);
}
