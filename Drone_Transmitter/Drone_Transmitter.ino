#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

//Defining RF Receiver pins, channel, & array size
RF24 radion(9, 10); //CE, CSN pins
const uint64_t transmission = 0xE8E8F0F01FF;
int transmittedData[5];

//Defining ARM & BIND pins
int BIND_pin = 2;
int ARM_pin = 3;

//Defining joystick reading pins
int THROTTLE_pin = 0;
int ROLL_pin = 1;
int PITCH_pin = 4;
int YAW_pin = 5;

//Defining joystick readings (w/o values)
int THROTTLE_val;
int ROLL_val;
int PITCH_val;
int YAW_val;

//Defining movement terms (w/o values)
int ppm_THROTTLE;
int ppm_ROLL;
int ppm_PITCHl
int ppm_YAW;

//Needed to check if BIND is complete
boolean BIND_comp = false;

void setup() {
  Serial.begin(9600);
  MAP();
  BIND();
}

void loop() {
  ARM(); //Transmitting data from the joysticks to the reveiving drone.
}

void MAP() {
  //Reading the joystick and assigning the values to terms
  THROTTLE_val = analogRead(THROTTLE_pin);
  ROLL_val = analogRead(ROLL_pin);
  PITCH_val = analogRead(PITCH_pin);
  YAW_val = analogRead(YAW_pin);

//Mapping the joystick values to scalable values
  ppm_THROTTLE = map(THROTTLE_val, 0, 1023, 1000, 2000);
  ppm_ROLL = map(ROLL_val, 0, 1023, 1000, 2000);
  ppm_PITCH = map(PITCH_val, 0, 1023, 1000, 2000);
  ppm_YAW = map(YAW_val, 0, 1023, 1000, 2000);

//Placing each new value into a data array to be transmitted 
  int transmittedData[5] = {ppm_THROTTLE, ppm_ROLL, ppm_PITCH, ppm_YAW};
}

void BIND() {
  if(digitalRead(BIND_pin) == HIGH && ppm_ROLL == 1000 && ppm_YAW == 1000) {
    radio.begin();
    radio.openWritingpipe(transmission);
    BIND_comp = true; //Confirming that BIND is complete
  }
}

void ARM() {
  if(BIND_comp == true && digitalRead(ARM_pin) == high) {
    radio.write(transmittedData, sizeof(transmittedData));
  }
}