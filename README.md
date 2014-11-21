ECE382_Lab6
===========

Lab 6: PWM Robot Motion

C2C Jasper Arneberg  
T5 ECE 382  
Capt Trimble  

##Introduction
Microcontrollers can serve a variety of purposes. In this lab, we use the MSP430 to drive a robot.

##Prelab
###PWM Strategy
Pulse Width Modulation creates an average voltage signal. In order to provide enough current to a motor, the signal is fed into a motor driver. To change direction of motor rotation, the polarity of the voltage must be switched. This can be accomplished by having a separate signal go to both the positive and negative terminals of a given motor.

####PWM Movement
|               | Left + | Left - | Right + | Right - |
|---------------|--------|--------|---------|---------|
| moveForward() | ON     | OFF    | ON      | OFF     |
| moveBack()    | OFF    | ON     | OFF     | ON      |
| moveRight()    | ON     | OFF    | OFF     | ON      |
| moveLeft()   | OFF    | ON     | ON      | OFF     |

It is very important that the positive and negative signal for a given motor are never on at the same time. This will create a short circuit and could lead to circuitry damage. It is also important to not change the direction of the motor too abruptly. For this reason, every command to the robot begins with a delay after turning off all the voltage.

####Pseudocode
The following pseudocode was developed to help structure the code early on. 
```
#define MAX_RIGHT 60
#define MAX_LEFT 60

main() : 
set pin output directions
initialize Timer A
TACCR0 = 100;
TACCR1 = MAX_RIGHT
TACCR2 = MAX_LEFT

setRightForward() : 
right+ PWM
right- Reset

setRightBackward() :
right+ Reset
right- PWM

setLeftForward() : 
left+ PWM
left- Reset

setLeftBackward() :
left+ Reset
left- PWM

moveForward() : 
turn off all signals
delay 10000 cycles
setRightForward();
setLeftForward();

moveBack() :
turn off all signals
delay 10000 cycles
setRightBackward();
setLeftBackward();

moveRight() : 
turn off all signals
delay 10000 cycles
setRightBackward();
setLeftForward();

moveLeft() :
turn off all signals
delay 10000 cycles
setRightForward();
setLeftBackward();
```

###Pin Assignments
Each of the four motor driver voltage terminals were assigned pin numbers.

| Function           | Motor Driver Pin | MSP430 Pin |
|--------------------|------------------|------------|
| Left PWM	     | 1A               | P2.2       |
| Left Reverse Control  | 2A               | P2.1       |
| Right PWM          | 3A               | P2.4       |
| Right Reverse Control | 4A               | P2.3       |
| IR Sensor          | N/A              | Xin        |

![alt text](https://github.com/JasperArneberg/ECE382_Lab6/blob/master/schematic.png?raw=true "Schematic")

##Required Functionality: Robot Movement

Here was the setup for the PWM to drive the motors, built off Dr. Coulston's lab6.c code on his website.
```
    WDTCTL = WDTPW|WDTHOLD;                 // stop the watchdog timer

    P2DIR |= BIT1;							// P2.1 is left directional control
    P2OUT &= ~BIT1;							// initialize output to 0, forward drive

    P2DIR |= BIT2;							// P2.2 is associated with TA1CCR1
    P2SEL |= BIT2;							// P2.2 is associated with TA1CCTL1

    P2DIR |= BIT3;							//P2.3 is right directional control
    P2OUT &= ~BIT3;

    P2DIR |= BIT4;							// P2.4 is associated with TA1CCR2
    P2SEL |= BIT4;							// P2.4 is associated with TA1CCTL2

    TA1CTL = ID_3 | TASSEL_2 | MC_1;					// Use 1:8 presclar off MCLK
    TA1CCR0 = PERIOD;							// set signal period for PWM
    TA1CCR0 = 100;
```

A function named moveForward() was created to set the robot in forward motion:
```
void moveForward(int speed) {
	P2OUT &= ~BIT1;						//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = speed;

	P2OUT &= ~BIT3;						//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = speed;
}
```

This function uses the Reset/Set mode in order to have the duty cycle equal to the speed parameter.

The moveBack() method was constructed to allow the robot to move backwards at a desired speed:
```
void moveBack(int speed) {
	P2OUT |= BIT1;						//set left reverse select
	TA1CCTL1 = OUTMOD_3;					//Set/Reset mode
	TA1CCR1 = speed;

	P2OUT |= BIT3;						//set right reverse select
	TA1CCTL2 = OUTMOD_3;					//Set/Reset mode
	TA1CCR2 = speed;
}
```

In this method, the Set/Reset mode is utilized. The reverse select is set high for both the left and right motor.

The maximum speed of the robot is limited by the stall current, which should never exceed 1 A. The stall current was tested at different duty cycles as can be seen in the table below:

| Duty Cycle | Stall Current |
|------------|---------------|
| 60%        | 0.62 A        |
| 70%        | 0.68 A        |
| 80%        | 0.78 A        |
| 90%        | 0.96 A        |

For the reason, the MAX_SPEED parameter was set to 90, representing a 90% duty cycle.

Below is the function for turnLeft(). One wheel drives forward and the other drives backwards. The duty cycle is set to TURN_SPEED so that it is always consistent.
```
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
```

The timing was the trickiest part of this functionality. The number of clock cycles per degree of revolution, approximately 3400, was determined by timing the robot as it spun continuously. The robot made 10 complete revolutions in 11.8 seconds, meaning each revolution of 360 degrees was 1.18 seconds. The clock speed is approximately 1 MHz, or 1,000,000 cycles per second. 1.18 sec/rev * (1 rev / 360 deg) * 1,000,000 cycles/sec = 3300 cycles/deg, rounded to two decimal places.

It was also determined that there was a start-up time associated with turning. The motors take time to get to full speed. By experimentation, a start-up time of 200,000 cycles was determined.

The same delay scheme was used for turnRight(), and it worked fairly well. 

##A Functionality: Remote-Controlled Robot Movement

The A functionality was fairly straightforward to implement since Lab 5 had similar requirements. In fact, it took about 25 minutes to fully complete the remote-controlled robot movement.

When testing the A functionality, it was very difficult to steer the robot. This was because the turnRight() and turnLeft() methods only worked at a duty cycle of 60%, which is relatively fast. An extra paramter, speed, was added into the turnRight() and turnLeft() methods. This allowed the robot to turn at different speeds, giving finer control.

##Debugging
The MSP430 was not able to stay powered when the USB cable was disconnected. To investigate the error, the voltages of the board were measured. For some reason, the 5 V rail only had 0.5 V across it. After further analysis, an electrical error in the hardware was discovered. The 5 V output was traced along to a pin on th every left side of the board. Once this pin was connected to the 5 V rail, everything worked as expected.

The wheels always spun as expected when the MSP430 was plugged directly into the computer. When the MSP430 was disconnected from the computer, however, errors often occurred. After some probing, it appeared that the chip was being reset. This was likely due to the noise of the motors. This was fixed by putting a capacitor between the reset pin and ground.

When attempting to drive the robot backwards, the motors exerted very little torque. This led to hardly any movement of the robot even though the wheels would spin when off the ground. To investigate the error, the voltage levels were observed. When the robot moved forward, the voltage across the 5 V rail stayed at a consistent 4.92 V. However, when the robot wheels moved backward, the voltage dropped to nearly 4.6 V. To fix this problem, a capactior was added across the 12 V rail. This allowed the motors to draw power when they needed it. After this was implemented, the voltage across the 5 V rail was consistently 4.92 V the entire time, and the torque of the motor increased greatly.

The following code worked at first. However, when stopMoving() was called after moveBack(), the wheels did not stop moving.
```
void stopMoving() {
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = 0;

	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = 0;
}
```

This error was fixed by clearing the reverse select pins. Essentially, this turned stopMoving() into the moveForward() subroutine with a speed of 0.
```
void stopMoving() {
	P2OUT &= ~BIT1;							//clear left reverse select
	TA1CCTL1 = OUTMOD_7;					//Reset/Set mode
	TA1CCR1 = 0;

	P2OUT &= ~BIT3;							//clear right reverse select
	TA1CCTL2 = OUTMOD_7;					//Reset/Set mode
	TA1CCR2 = 0;
}
```


##Conclusion
In conclusion, this laboratory exercise demonstrated one of the many applications of microcontrollers. Pulse width modulation is fairly simple, but it is vitally important when dealing with electromechanical actuators such DC motors. It relies on precise timing, which is why it is necessary to know how timers function. This laboratory exercise has set a good baseline for skills and functionality that will be used in Lab 7 and Lab 8.

##Documentation
I used http://www.tablesgenerator.com/markdown_tables to generate markdown tables efficiently.  
C2C Evan Richter explained to me Dr. Coulston's strategy for driving the motors, only using a total of two PWM signals.
