#include <msp430.h>

void main(void)
{

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 100;						// set signal period

    TA1CCR1 = 20;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 20;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode

    while (1) {
    	__delay_cycles(1000000);
    	TA1CCR1 = 0;
    	TA1CCR2 = 100;
    	__delay_cycles(1000000);
    	TA1CCR1 = 25;            // set duty cycle to 25/100 (25%)
    	TA1CCR2 = 75;
    	__delay_cycles(1000000);
    	TA1CCR1 = 50;
    	TA1CCR2 = 50;

    } // end loop
}
