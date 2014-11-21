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
| Left Positive PWM  | 1A               | P2.0       |
| Left Negative PWM  | 2A               | P2.1       |
| Right Positive PWM | 3A               | P2.2       |
| Right Negative PWM | 4A               | P2.3       |
| IR Sensor          | N/A              | Xin        |

![alt text](https://github.com/JasperArneberg/ECE382_Lab6/blob/master/schematic.png?raw=true "Schematic")

##Required Functionality: Robot Movement

##A Functionality: Remote-Controlled Robot Movement

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
In conclusion,

##Documentation
I used http://www.tablesgenerator.com/markdown_tables to generate markdown tables efficiently. 
