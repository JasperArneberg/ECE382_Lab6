#include <msp430.h>

void main(void)
{
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P1DIR |= BIT2;                // TA0CCR1 on P1.2
    P1SEL |= BIT2;                // TA0CCR1 on P1.2

    TACTL &= ~MC1|MC0;            // stop timer A0

    TACTL |= TACLR;                // clear timer A0

    TACTL |= TASSEL1;           // configure for SMCLK

    TACCR0 = 100;                // set signal period to 100 clock cycles (~100 microseconds)
    TACCR1 = 25;                // set duty cycle to 25/100 (25%)

    TACCTL1 |= OUTMOD_7;        // set TACCTL1 to Reset / Set mode

    TACTL |= MC0;                // count up

    while (1) {
        __delay_cycles(1000000);
        TACCR1 = 25;            // set duty cycle to 25/100 (25%)
        __delay_cycles(1000000);
        TACCR1 = 50;
        __delay_cycles(1000000);
        TACCR1 = 0;
    }
}
