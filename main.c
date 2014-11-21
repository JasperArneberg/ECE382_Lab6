#include <msp430.h>
#define PERIOD 100;
#define TURN_SPEED 60;
#define MAX_SPEED 60;

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
    TA1CCR0 = PERIOD;						// set signal period for PWM
    TA1CCR0 = 100;

    while (1) {

    	moveForward(60);
    	__delay_cycles(2000000);
       	stopMoving();
       	__delay_cycles(1000000);

       	moveBack(60);
       	__delay_cycles(2000000);
       	stopMoving();
       	__delay_cycles(1000000);

    	turnRight(90);
    	stopMoving();
    	__delay_cycles(1000000);

    	turnRight(20);
    	stopMoving();
    	__delay_cycles(1000000);

    	turnLeft(90);
    	stopMoving();
    	__delay_cycles(1000000);

    	turnLeft(20);
    	stopMoving();
    	__delay_cycles(1000000);

    } // end loop
}

void stopMoving() {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = 0;

	P2OUT &= ~BIT3;							//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = 0;
}

void moveForward(int speed) {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = speed;

	P2OUT &= ~BIT3;							//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = speed;
}

void moveBack(int speed) {
	P2OUT |= BIT1;							//set left reverse select
	TA1CCTL1 = OUTMOD_3;					//Set/Reset mode
	TA1CCR1 = speed;

	P2OUT |= BIT3;							//set right reverse select
	TA1CCTL2 = OUTMOD_3;					//Set/Reset mode
	TA1CCR2 = speed;
}

void turnLeft(int degrees) {
	P2OUT |= BIT1;							//set left reverse select
	TA1CCTL1 = OUTMOD_3;					//Set/Reset mode
	TA1CCR1 = TURN_SPEED;

	P2OUT &= ~BIT3;							//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = TURN_SPEED;

	__delay_cycles(150000);
	int i = 0;
	for (i=0; i<degrees; i++) {
		__delay_cycles(3300);
	}
}

void turnRight(int degrees) {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = TURN_SPEED;

	P2OUT |= BIT3;							//set right reverse select
	TA1CCTL2 = OUTMOD_3;					//Set/Reset mode
	TA1CCR2 = TURN_SPEED;

	__delay_cycles(200000);
	int i = 0;
	for (i=0; i<degrees; i++) {
		__delay_cycles(3300);
	}
}
