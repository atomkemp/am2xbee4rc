/**********
P is for pistol
J is for joystick
Written by Adam Kemp (atomkemp), 2015
**********/

#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;
Servo servo5;
Servo servo6;

byte servo1Pos = 91;
byte servo2Pos = 91;
byte servo3Pos = 91;
byte servo4Pos = 91;
byte servo5Pos = 91;
byte servo6Pos = 91;

byte serialBuffer[100];
unsigned long time = 0;
int statusCheckInterval = 500;

unsigned long servoTime = 0;
int servoRefreshRate = 15;

unsigned long watchdog = 0;
int watchdogRate = 500;
boolean watched = false;

void setup() {
  Serial.begin(57600);
  Serial.setTimeout(20);
  sendAck();
  servo1.attach(A0);
  servo2.attach(A1);
  servo3.attach(A2);
  servo4.attach(A3);
  servo5.attach(A4);
  servo6.attach(A5);
  pinMode(13, OUTPUT);
}

void loop() {
  checkWatchdog();
  checkTelemetry();
  setServos();
  if (millis() >= time) {
    time = millis() + statusCheckInterval;
    sendHeader();
    checkVoltage();
    checkCurrent();
    sendFooter();
  }
}

void checkWatchdog() {
  if (watchdog <= millis()) {
    watchdog = millis() + watchdogRate; //reset watchdog
    sendAck();
    servo1Pos = 90;
    servo2Pos = 90;
    servo3Pos = 90;
    servo4Pos = 90;
    servo5Pos = 90;
    servo6Pos = 90;
  }
}
    

void setServos(){
  if (millis() >= servoTime) {
    servoTime = millis() + servoRefreshRate;
    servo1.write(servo1Pos-1);
    servo2.write(servo2Pos-1);
    servo3.write(servo3Pos-1);
    servo4.write(servo4Pos-1);
    servo5.write(servo5Pos-1);
    servo6.write(servo6Pos-1);
  }
}

void checkVoltage() {
  float _voltage = analogRead(A7);
  _voltage = ((_voltage * 0.00322581)/0.2423)*10; //from attopilot
  Serial.write(byte(_voltage));
}

void checkCurrent() {
  float _current = analogRead(A6);
  _current = ((_current * 0.00322581)/0.0732)*100; //from attopilot
  Serial.write(byte(_current));
}

void sendHeader() {
  Serial.write(1);
  Serial.write(1);
  Serial.write(1);
}

void sendFooter() {
  Serial.print('\0');
}

void checkTelemetry() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\0',serialBuffer,10);
    if (serialBuffer[0] == 1 && serialBuffer[1] == 1 && serialBuffer[2] == 1) {
      watchdog = millis() + watchdogRate;
      digitalWrite(13, HIGH);
      servo1Pos = serialBuffer[3];
      servo2Pos = serialBuffer[4];
      servo3Pos = serialBuffer[5];
      servo4Pos = serialBuffer[6];
      servo5Pos = serialBuffer[7];
      servo6Pos = serialBuffer[8];
      memset(serialBuffer,0,sizeof(serialBuffer)); //clear buffer
      Serial.flush();
      sendAck();
      digitalWrite(13, LOW);
    }
  }
}

void sendAck() {
  Serial.print("OK");
}
