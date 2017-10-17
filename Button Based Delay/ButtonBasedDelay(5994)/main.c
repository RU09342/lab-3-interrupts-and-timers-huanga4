#include <msp430.h> 


/**
 * main.c
 * Austin Huang
 * ButtonBasedDelay
 */
int buttonPress;

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; //Stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode


    P1DIR |= BIT0; //set Port 1.0 LED

    P5DIR &= ~(BIT5); //set Port5.5 Button
    P5REN |= BIT5; //enable pull-up resistor
    P5OUT |= BIT5;
    P5IE |= BIT5; //enable interrupt
    P5IES |= BIT5; //set falling edge
    P5IFG &= ~(BIT5); //clear flag

    TA0CTL = TASSEL_1 + MC_1 + ID_1; //setTimer A,Up mode, divider value 2
    TA0CCTL0 = 0x10; //set compare mode for CCTL
    TA0CCR0 = 1600; // LED blinks at 32kHZ*2/1600 or 10 Hz

    __enable_interrupt(); //enable interrupt
    _BIS_SR(LPM4_bits + GIE); // Enter Low Power Mode 4
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    P1OUT ^= 0x01;

}

#pragma vector=PORT5_VECTOR
__interrupt void PORT_5(void)
{
    //Debounce
    P5IE &= ~BIT5;
    __delay_cycles(1);

    if (buttonPress == 0){ //falling edge
        TA1CTL = TASSEL_1+ MC_3; // select Timer A, Continuous 
        TA1CCR0 = 0xFFFF; //initialize TA1CCR0
        TA1CCTL0 = CAP; //Capture mode
        buttonPress = 1;
        TA0CCR0 = 1; //reset CCR0

    }
    else if (buttonPress == 1){ //rising edge
        TA1CTL = MC_0; //stop counting
        TA0CCR0 = TA1R; // new CCR0 value
        if (TA0CCR0 > 65500){
                TA0CCR0 = 0xFFFF;
        }
        if (TA0CCR0 < 2000){
            TA0CCR0 = 2000;
        }
        TA1CTL = TACLR; //clear Timer A1
        buttonPress = 0;
    }

    P5IES ^= BIT5;
    P5IE |= BIT5;
    P5IFG &= ~(BIT5);
}
