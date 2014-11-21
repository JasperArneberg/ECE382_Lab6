#include <msp430.h>

/**
 * Name: C2C Jasper Arneberg
 * Date: 20 Nov 14
 * Description: This program drives a robot using pulse width modulation.
 * This is accomplished by utilizing Timer A.
 */

void main(void)
{

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P2DIR |= BIT1;							// P2.1 is left directional control
    P2OUT &= ~BIT1;							// initialize output to 0, forward drive

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT3;							//P2.3 is right directional control
    P2OUT &= ~BIT3;

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 100;							// set signal period

    TA1CCR1 = 20;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 20;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Set / Reset mode

    while (1) {
       	__delay_cycles(1000000);
    	TA1CCR1 = 40;
    	TA1CCR2 = 60;
    	P2OUT ^= BIT1;						//toggle left reverse control
    	P2OUT ^= BIT3;						//toggle right reverse control
    	__delay_cycles(1000000);
    	TA1CCR1 = 50;
    	TA1CCR2 = 50;

    } // end loop
}
