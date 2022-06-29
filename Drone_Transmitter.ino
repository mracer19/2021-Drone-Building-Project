#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

RF24 radio(9, 10); // CE, CSN
const uint64_t transmission = 0xE8E8F0F0E1FF;
int transmittedData[5];

int THROTTLE_pin = 0;
int ROLL_pin = 1;
int PITCH_pin = 4;
int YAW_pin = 5;

int BIND_pin = 2;
int ARM_pin = 3;

int THROTTLE_Val;
int ROLL_Val;
int PITCH_Val;
int YAW_Val;

int ppm_THROTTLE;
int ppm_ROLL;
int ppm_PITCH;
int ppm_YAW;

boolean BIND_comp = false;

void setup() {
  Serial.begin(9600);
  MAP();
  BIND();
}

void loop() {
  ARM();
}

void MAP () {
  THROTTLE_Val = analogRead(THROTTLE_pin);
  ROLL_Val = analogRead(ROLL_pin);
  PITCH_Val = analogRead(PITCH_pin);
  YAW_Val = analogRead(YAW_pin);

  ppm_THROTTLE = map(THROTTLE_Val, 0, 1023, 1000, 2000);
  ppm_ROLL = map(ROLL_Val, 0, 1023, 1000, 2000);
  ppm_PITCH = map(PITCH_Val, 0, 1023, 1000, 2000);
  ppm_YAW = map(YAW_Val, 0, 1023, 1000, 2000);

  int transmittedData[5] = {ppm_THROTTLE, ppm_ROLL, ppm_PITCH, ppm_YAW};
}

void BIND () {
  if(digitalRead(BIND_pin) == HIGH && ppm_ROLL == 1000 && ppm_YAW == 1000) {
    radio.begin();
    radio.openWritingPipe(transmission);
    BIND_comp = true;
  }
}

void ARM () {
  if(BIND_comp == true && digitalRead(ARM_pin) == HIGH) {
    radio.write(transmittedData, sizeof(transmittedData));
  }
}
