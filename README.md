# LCD Actuation assignment

# Introduction
This code is to be uploaded onto a electronics system consisting of:
1. Arduino UNO 
2. LCD Shield (DF Robotics Shield)
3. IR sensor
4. Stepper motor + motor driver

# Setup
1. To connect the motor to the lcd-shield/arduino, follow the guidelines from this link: [motor connection](https://create.arduino.cc/projecthub/debanshudas23/getting-started-with-stepper-motor-28byj-48-3de8c9)
2. Connect the IR sensor signal pin to any analog pin on the LCD shield (except A0)
3. Connect the LCD shield to the arduino by simply clipping the shield directly above the arduino 

# Usage
On the home screen on the LCD you will see 3 options:

* CM: Continuous motor test mode
* PM: Precise motor test mode
* IR: Infrared sensor test mode

**CM MODE**
1. Select 'Start' with the SELECT button
2. Adjust the speed of the motor with the UP or DOWN button
3. Change the direction of the motor spin with the LEFT or RIGHT button
4. Press the RESET button to exit the PM mode

**PM MODE**
1. Select 'Start' with the SELECT button
2. Use the UP or DOWN buttons to increment to decrement the angle of rotation
3. Press the RIGHT button to start the motor 
4. Press the RESET button to exit the PM mode

**IR MODE**
The aim is to measure distance using the IR sensor. However it should be noted that this functionality isn't completely tested because it is not properly calibrated yet.