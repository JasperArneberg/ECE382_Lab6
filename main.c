#include <msp430.h>
#include "lab6.h"

/**
 * Name: C2C Jasper Arneberg
 * Date: 20 Nov 14
 * Description: This program drives a robot using pulse width modulation.
 * This is accomplished by utilizing Timer A.
 */

int8	newIrPacket = FALSE;
int16	packetData[32];
int8	packetIndex = 0;
int32	irPacket = 0;

void main(void)
{

    initMSP430();

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

    while (1) {

    	/**
    	//Basic Functionality

    	moveForward(90);
    	__delay_cycles(2000000);
       	stopMoving();
       	__delay_cycles(1000000);

       	moveBack(90);
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
    	*/

    	//A Functonality
		if (newIrPacket==TRUE) {
			newIrPacket = FALSE;		//reset flag
			_disable_interrupt();		//disable while drawing

			if (irPacket == UP) {
				moveForward(60);
			} else if (irPacket == DOWN) {
				moveBack(60);
			} else if (irPacket == RIGHT) {
				turnRight(90);
			} else if (irPacket == LEFT) {
				turnLeft(90);
			} else if (irPacket == ENTER) {
				stopMoving();
			}

			initMSP430();				//prepare for next interrupt
		}
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

	__delay_cycles(200000);					//motor start-up delay
	int i = 0;
	for (i=0; i<degrees; i++) {
		__delay_cycles(3300);				//3300 cycles per degree
	}
}

void turnRight(int degrees) {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = TURN_SPEED;

	P2OUT |= BIT3;							//set right reverse select
	TA1CCTL2 = OUTMOD_3;					//Set/Reset mode
	TA1CCR2 = TURN_SPEED;

	__delay_cycles(200000);					//motor start-up delay
	int i = 0;
	for (i=0; i<degrees; i++) {
		__delay_cycles(3300);				//3300 cycles per degree
	}
}


// -----------------------------------------------------------------------
// In order to decode IR packets, the MSP430 needs to be configured to
// tell time and generate interrupts on positive going edges.  The
// edge sensitivity is used to detect the first incoming IR packet.
// The P2.6 pin change ISR will then toggle the edge sensitivity of
// the interrupt in order to measure the times of the high and low
// pulses arriving from the IR decoder.
//
// The timer must be enabled so that we can tell how long the pulses
// last.  In some degenerate cases, we will need to generate a interrupt
// when the timer rolls over.  This will indicate the end of a packet
// and will be used to alert main that we have a new packet.
// -----------------------------------------------------------------------
void initMSP430() {

	IFG1=0; 							// clear interrupt flag1
	WDTCTL=WDTPW+WDTHOLD; 				// stop WD

	BCSCTL1 = CALBC1_8MHZ;
	DCOCTL = CALDCO_8MHZ;

	P2SEL  &= ~BIT6;					// Setup P2.6 as GPIO not XIN
	P2SEL2 &= ~BIT6;
	P2DIR &= ~BIT6;
	P2IFG &= ~BIT6;						// Clear any interrupt flag
	P2IE  |= BIT6;						// Enable PORT 2 interrupt on pin change

	HIGH_2_LOW;
	//P1DIR |= BIT0 | BIT6;				// Enable updates to the LED
	//P1OUT &= ~(BIT0 | BIT6);			// And turn the LED off

	TA0CCR0 = 0xFFFF;					// create a 16mS roll-over period
	TACTL &= ~TAIFG;					// clear flag before enabling interrupts = good practice
	TACTL = ID_3 | TASSEL_2 | MC_1;		// Use 1:1 presclar off MCLK and enable interrupts

	_enable_interrupt();
}


/**
 * Pin change vector
 */
#pragma vector = PORT2_VECTOR			// This is from the MSP430G2553.h file

__interrupt void pinChange (void) {

	int8	pin;
	int16	pulseDuration;				// The timer is 16-bits

	if (IR_PIN)		pin=1;	else pin=0;

	switch (pin) {						// read the current pin level
		case 0:							// !!!!!!!!!NEGATIVE EDGE!!!!!!!!!!
			pulseDuration = TAR;

			irPacket <<= 1; 			//shift one bit to the left
			if (pulseDuration > minLogic1Pulse) {
				irPacket += 1;			//logical 1
			}
			packetData[packetIndex++] = pulseDuration;
			TACTL |= MC_0;				//disable timerA
			LOW_2_HIGH; 				// Setup pin interrupt on positive edge
			break;

		case 1:							// !!!!!!!!POSITIVE EDGE!!!!!!!!!!!
			TAR = 0x0000;				// time measurements are based at time 0
			HIGH_2_LOW; 				// Setup pin interrupt on positive edge
			TACTL |= MC_1;				//enable timerA
			TACTL |= TAIE;				//enable timer A interrupt
			break;
	} // end switch

	P2IFG &= ~BIT6;						// Clear the interrupt flag to prevent immediate ISR re-entry

} // end pinChange ISR

// -----------------------------------------------------------------------
//			0 half-bit	1 half-bit		TIMER A COUNTS		TIMER A COUNTS
//	Logic 0	xxx
//	Logic 1
//	Start
//	End
//
// -----------------------------------------------------------------------

#pragma vector = TIMER0_A1_VECTOR			// This is from the MSP430G2553.h file
__interrupt void timerOverflow (void) {
	TACTL &= ~TAIE;							//disable Timer A interrupt
	packetIndex = 0;						//reset packet index
	newIrPacket = TRUE;						//new packet must have arrived
	TACTL |= MC_0;							//disable timerA
	TACTL &= ~TAIFG;
}
