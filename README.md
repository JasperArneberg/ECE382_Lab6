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

## Required 

##Debugging


##Conclusion
In conclusion,

##Documentation
I used http://www.tablesgenerator.com/markdown_tables to generate markdown tables efficiently. 
