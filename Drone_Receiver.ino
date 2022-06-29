#include <Servo.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

Servo T1;
Servo T2;
Servo T3;
Servo T4;

int T1_pin = 2;
int T2_pin = 3;
int T3_pin = 4;
int T4_pin = 5;

RF24 radio(9, 10);
const uint64_t transmission = 0xE8E8F0F0E1FF;
int receivedData[5];

int ppm_THROTTLE;
int ppm_ROLL;
int ppm_PITCH;
int ppm_YAW;

int opposing_YAW = 0;

void setup() {
  Serial.begin(9600);

  pinMode(T1_pin, OUTPUT);
  pinMode(T2_pin, OUTPUT);
  pinMode(T3_pin, OUTPUT);
  pinMode(T4_pin, OUTPUT);

  radio_initiate();
}

void loop() {
  radio_read();
  
  throttle_INCREASE();
  throttle_DECREASE();

  roll_LEFT();
  roll_RIGHT();

  pitch_UP();
  pitch_DOWN();

  yaw_LEFT();
  yaw_RIGHT();
}

void radio_initiate() {
  radio.begin();
  radio.openReadingPipe(0, transmission);
  radio.startListening();
}
//------------------------------------------------------

void radio_read(){
  radio.read(receivedData, sizeof(receivedData));
  int ppm_THROTTLE = receivedData[0];
  int ppm_ROLL = receivedData[1];
  int ppm_PITCH = receivedData[2];
  int ppm_YAW = receivedData[3];
}
//------------------------------------------------------

void throttle_INCREASE() {
  for(ppm_THROTTLE > 1500; ppm_THROTTLE <= 2000; ppm_THROTTLE++){
    T1.write(ppm_THROTTLE);
    T2.write(ppm_THROTTLE);
    T3.write(ppm_THROTTLE);
    T4.write(ppm_THROTTLE);
  }
}
void throttle_DECREASE() {
   for(ppm_THROTTLE < 1500; ppm_THROTTLE >= 1000; ppm_THROTTLE--){
    T1.write(ppm_THROTTLE);
    T2.write(ppm_THROTTLE);
    T3.write(ppm_THROTTLE);
    T4.write(ppm_THROTTLE);
  }
}
//------------------------------------------------------

void roll_LEFT() {
  for(ppm_ROLL < 1500; ppm_ROLL >= 1000; ppm_ROLL--){
    T2.write(ppm_ROLL);
    T3.write(ppm_ROLL);
  }
}
void roll_RIGHT(){
  for(ppm_ROLL > 1500; ppm_ROLL <= 2000; ppm_ROLL++){
    T1.write(ppm_ROLL);
    T4.write(ppm_ROLL);
  }
}
//------------------------------------------------------

void pitch_UP() {
  for(ppm_PITCH > 1500; ppm_PITCH <= 2000; ppm_PITCH++){
    T1.write(ppm_PITCH);
    T2.write(ppm_PITCH);
  }
}
void pitch_DOWN() {
  for(ppm_PITCH < 1500; ppm_PITCH >= 1000; ppm_PITCH--){
    T4.write(ppm_PITCH);
    T3.write(ppm_PITCH);
  }
}
//------------------------------------------------------

void yaw_LEFT() {
  for(ppm_YAW < 1500; ppm_YAW >= 1000; ppm_YAW--){
    opposing_YAW = ppm_YAW - (2 * 1500);
    T2.write(ppm_YAW);
    T4.write(ppm_YAW);
    T3.write(opposing_YAW);
    T1.write(opposing_YAW);
  }
}
void yaw_RIGHT() {
  for(ppm_YAW > 1500; ppm_YAW <= 2000; ppm_YAW++){
    opposing_YAW = ppm_YAW - (2 * 1500);
    T3.write(ppm_YAW);
    T1.write(ppm_YAW);
    T2.write(opposing_YAW);
    T1.write(opposing_YAW);
  }
}
