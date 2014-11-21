#include <msp430.h>
#define PERIOD 100;

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
    TA1CCR0 = PERIOD;							// set signal period for PWM

    while (1) {
        //__delay_cycles(1000000);
       	//stopMoving();
    	__delay_cycles(1000000);
    	moveForward(20);
    	__delay_cycles(1000000);
    	moveForward(60);
    	__delay_cycles(1000000);
    	moveBack(20);
    	__delay_cycles(1000000);
    	moveBack(60);


    } // end loop
}

void stopMoving() {
	TA1CCTL1 = OUTMOD_7;
	TA1CCR1 = 0;
	TA1CCTL2 = OUTMOD_7;
	TA1CCR2 = 0;
}

void moveForward(char speed) {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = speed;

	P2OUT &= ~BIT3;							//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = speed;
}

void moveBack(char speed) {
	P2OUT |= BIT1;							//set left reverse select
	TA1CCTL1 = OUTMOD_3;					//Set/Reset mode
	TA1CCR1 = speed;

	P2OUT |= BIT3;							//set right reverse select
	TA1CCTL2 = OUTMOD_3;					//Set/Reset mode
	TA1CCR2 = speed;
}
