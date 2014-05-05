#include "DualVNH5019MotorShield.h"

#define NUM_SENSORS             4
#define NUM_SAMPLES_PER_SENSOR  10

DualVNH5019MotorShield md;
unsigned int sensorValues[NUM_SENSORS];

int maxAllowedSpeed = 180;
int minAllowedSpeed = 30;
int leftBaseSpeed = 120;
int rightBaseSpeed = 120;
//Kp < Kd
double Kp = 0.7;
double Kd = 5;
int lastError = 0;

int getPosition()
{
  for (int i = 0; i < NUM_SENSORS; i++) {
    int sValue = 0;
    for (int j=0; j<NUM_SAMPLES_PER_SENSOR; j++) {
      sValue += analogRead(i);
    }
    sensorValues[i] = int(sValue / NUM_SAMPLES_PER_SENSOR);
  }

  int s0, s1, s2, s3;
  s0 = map(sensorValues[3], 0, 680, 0, 128);
  s1 = sensorValues[0];
  s2 = sensorValues[1];
  s3 = map(sensorValues[2], 0, 680, 0, 128);

  int leftSide = s0 + s1;
  int rightSide = s2 + s3;
  
  return leftSide - rightSide;
}

void setup()
{
  md.init();
}

void loop()
{
  int position = getPosition();
  int error = position * 2;
  int motorSpeed = Kp * error + Kd * (error - lastError);
  lastError = error;
  int rightMotorSpeed = constrain(rightBaseSpeed + motorSpeed, minAllowedSpeed, maxAllowedSpeed);
  int leftMotorSpeed = constrain(leftBaseSpeed - motorSpeed, minAllowedSpeed, maxAllowedSpeed);

  md.setSpeeds(leftMotorSpeed, rightMotorSpeed);
}
