/*******************************************
  * Title: Lab 6: Robot Motion
  * Author: C2C Jasper Arneberg
  * Created: 21 November 2014
  * Description: This header file defines all data structures and
  * constants for use in the main.c file. This header file also
  * enables functionality with the IR sensor.
*******************************************/

typedef		unsigned char		int8;
typedef		unsigned short		int16;
typedef		unsigned long		int32;
typedef		unsigned long long	int64;

#define		TRUE				1
#define		FALSE				0

//constants for robot movement
#define PERIOD 100;
#define TURN_SPEED 60;
#define MAX_SPEED 90;

//-----------------------------------------------------------------
// Function prototypes found in lab6.c
//-----------------------------------------------------------------
void initMSP430();
__interrupt void pinChange (void);
__interrupt void timerOverflow (void);

//-----------------------------------------------------------------
// Each PxIES bit selects the interrupt edge for the corresponding I/O pin.
//	Bit = 0: The PxIFGx flag is set with a low-to-high transition
//	Bit = 1: The PxIFGx flag is set with a high-to-low transition
//-----------------------------------------------------------------
#define		IR_PIN			(P2IN & BIT6)
#define		HIGH_2_LOW		P2IES |= BIT6
#define		LOW_2_HIGH		P2IES &= ~BIT6

#define		averageLogic0Pulse	610		//as determined with 8 samples
#define		averageLogic1Pulse	1496
#define		averageStartPulse	0x1100
#define		minLogic0Pulse		averageLogic0Pulse - 100
#define		maxLogic0Pulse		averageLogic0Pulse + 100
#define		minLogic1Pulse		averageLogic1Pulse - 100
#define		maxLogic1Pulse		averageLogic1Pulse + 100
#define		minStartPulse		averageStartPulse - 100
#define		maxStartPulse		averageStartPulse + 100

#define		POWER	0x2BD53AC7
#define		ZERO	0x61A0906F
#define		ONE		0x61A000FF
#define		TWO		0x61A0807F
#define		THREE	0x61A040BF
#define		ENTER	0x61A018E7

#define		UP		0x61A050AF			//channel up
#define		DOWN	0x61A0D02F			//channel down
#define		LEFT	0x61A0B04F			//volume down
#define		RIGHT	0x61A030CF			//volume up
