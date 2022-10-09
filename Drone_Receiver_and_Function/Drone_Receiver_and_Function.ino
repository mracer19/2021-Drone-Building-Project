#include <Server.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//Defining RF Receiver pins, channel, & array size
RF24 radio (9, 10);
const uint64_t transmission = 0xE8E8F0F0E1FF;
int receiveData[5];

//Defining Motors
Servo T1; //Front Left
Servo T2; //Front Right
Servo T3; //Back Right
Servo T4; //Back Left

//Defining Motor pins
int T1_pin = 2;
int T2_pin = 3;
int T3_pin = 4;
int T4_pin = 5;

//Defining movement terms (w/o values)
int ppm-THROTTLE;
int ppm_ROLL;
int ppm_PITCH;
int ppm_YAW;

//Important for calculating YAW
int opposing_YAW = 0;


void setup() {
  //Initializing terminal
  Serial.begin(9600);

  //Defining pin actions (INPUT/OUTPUT)
  pinMode(T1_pin, OUTPUT);
  pinMode(T2_pin, OUTPUT);
  pinMode(T3_pin, OUTPUT);
  pinMode(T$_pin, OUTPUT);

  //RF Initialization
  radio_initiate();
}

void loop() {
  radio.read();

  throttle_INCREASE();
  throttle_DECREASE();

  roll_LEFT();
  roll_RIGHT();

  pitch_UP();
  pitchDOWN();

  yaw_LEFT();
  yaw_RIGHT();

}

//Setting values to movement terms from RF receiver data
void radio_read() {
  radio.read(receicedData, sizeof(receivedData));
  int ppm_THROTTLE = receivedData[0];
  int ppm_ROLL = receivedData[1];
  int ppm_PITCH = receivedData[2];
  int ppm_YAW = receivedData[3];
}

//The following 23 lines of code is the movement of throttle
void throttle_INCREASE() {
  for(ppm_THROTTLE > 1500; ppm_THROTTLE <= 2000; ppm_THROTTLE++) {
    T1.write(ppm_THROTTLE);
    T2.write(ppm_THROTTLE);
    T3.write(ppm_THROTTLE);
    T4.write(ppm_THROTTLE);
  }
}

void throttle_DECREASE() {
  for(ppm_THROTTLE <1500; ppm_THROTTLE >= 1000; ppm_THROTTLE--) {
    T1.write(ppm_THROTTLE);
    T2.write(ppm_THROTTLE);
    T3.write(ppm_THROTTLE);
    T4.write(ppm_THROTTLE);
  }
}
/*
  The purpose of throttle on the drone is to change its location on the z-axis.
  By tilting the right joystick up on the controller, the throttle increases and the drone increases height.
  By tilting the right joystick down on the controller, the throttle decreases and the drone decreases height.
  The throttle value is the same on all 4 motors so that equal lift is applied to all motors.
*/

//The following 19 lines of code is the movement of roll
void roll_RIGHT() {
  for(ppm_ROLL > 1500; ppm_ROLL <= 2000; ppm_ROLL++) {
    T1.write(ppm_ROLL);
    T4.write(ppm_ROLL);
  }
}

void roll_LEFT() {
  for(ppm_ROLL < 1500; ppm_ROLL >= 1000; ppm_ROLL--) {
    T2.write(ppm_ROLL);
    T3.write(ppm_ROLL);
  }
}
/*
  The purpose of roll is to tilt the drone from side to side.
  Tilting the right joystick right increases the roll value, causing the two left motors to increase lift, causing the drone to roll to the right.
  Tilting the right joystick left decreases the roll value, causing the two right motors to decrease lift, causing the drone to roll to the left.
  If the values on one side of the drone decreased while the other side increased, the drone will do flips--this is less than desireable.
*/

//The following 19 lines of code is the movement of pitch
void pitch_UP() {
  for(ppm_PITCH > 1500; ppm_PITCH <= 2000; ppm_PITCH++) {
    T1.write(ppm_PITCH);
    T2.write(ppm_PITCH);
  }
}

void pitch_DOWN() {
  for(ppm_PITCH < 1500; ppm_PITCH >= 1000; ppm_PITCH--){ 
    T3.write(ppm_PITCH);
    T4.write(ppm_PITCH);
  }
}
/*
  The prupose of pitch is to tilt the drone forwards and backwards.
  Tilting the left joystick up increases the pitch value, causing the two front motors to increase lift, causing the front of the drone to pitch/tilt up.
  Tilting the left joystick down decreases the pitch value, causing the two back motors to increase lift, causing the back of the drone to pitch/tilt up.
  If the values on one side of the drone decreased while the other side increased, the drone will do flips--this is less than desireable.
*/

//The following 19 lines of code is the movement of pitch
void yaw_RIGHT() {
  for(ppm_YAW >1500; ppm_YAW <= 2000; ppm_YAW++) {
    opposing_YAW = ppm_YAW * -1;
    T1.write(ppm_YAW);
    T3.write(ppm_YAW);
    T2.write(opposing_YAW);
    T4.write(opposing_YAW);
  }
}

void yaw_LEFT() {
  for(ppm_YAW < 1500; ppm_YAW >= 1000; ppm_YAW--) {
    opposing_YAW = ppm_YAW * -1;
    T2.write(ppm_YAW);
    T4.write(ppm_YAW);
    T1.write(opposing_YAW);
    T3.write(opposing_YAW);
  }
}
/*
  The purpose of YAW is to spin the drone on its axis.
  In order for YAW to work properly, two diagonally opposed motors need to rotate in one direction, 
    and the other two motors need to rotate in the opposite direction.
  To achieve this, an equation is needed to derive an equal but opposite value of the YAW value to
    cause the correct 2 motors to spin in a counter-rotation to the other 2 motors.
  Tilting the left joystick to the right increases the YAW value, causing T1 and T2 to increase RPM 
    while also causing T2 and T4 to rotate at the same speed in the opposite direction. This turns the
    drone to the right.
  Tilting the left joystick to the right decreases the YAW value, causing T2 and T4 to increase RPM
    while also causing T1 and T3 to rotate at the same speed in the opposite direction. This turns the
    drone to the left. 
*/