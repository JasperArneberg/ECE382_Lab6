#include <msp430.h>

void main(void)
{
	/**
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P2DIR |= BIT2;                // TA0CCR1 on P2.2
    P2SEL |= BIT2;                // TA0CCR1 on P2.2

    TA1CTL &= ~MC1|MC0;            // stop timer A0

    TA1CTL |= TACLR;                // clear timer A0

    TA1CTL |= TASSEL1;           // configure for SMCLK

    TA1CCR0 = 100;                // set signal period to 100 clock cycles (~100 microseconds)
    TA1CCR1 = 25;                // set duty cycle to 25/100 (25%)

    TA1CCTL1 |= OUTMOD_7;        // set TACCTL1 to Reset / Set mode

    TA1CTL |= MC0;                // count up

    while (1) {
        __delay_cycles(1000000);
        TA1CCR1 = 25;            // set duty cycle to 25/100 (25%)
        __delay_cycles(1000000);
        TA1CCR1 = 50;
        __delay_cycles(1000000);
        TA1CCR1 = 0;
    }
    */

    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR &= ~BIT3;
    P1REN |= BIT3;

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT4;							// P2.2 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.2 is associated with TA1CCTL2

	TA1CTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:8 presclar off MCLK
    TA1CCR0 = 0x0100;						// set signal period

    TA1CCR1 = 0x0020;
    TA1CCTL1 = OUTMOD_7;					// set TACCTL1 to Reset / Set mode

    TA1CCR2 = 0x0020;
    TA1CCTL2 = OUTMOD_3;					// set TACCTL1 to Reset / Set mode

    while (1) {

    	while((P1IN & BIT3) != 0);			// Wait for a button press
    	while((P1IN & BIT3) == 0);			// and release

        TA1CCR1 = (TA1CCR1 + 0x010) & 0xFF;	// increase duty cycle
        TA1CCR2 = (TA1CCR2 + 0x010) & 0xFF;	// decrease duty cycle

    } // end loop
}
