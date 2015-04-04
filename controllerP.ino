/**********
P is for pistol
J is for joystick
Written by Adam Kemp (atomkemp), 2015
**********/

#include <Encoder.h>
#include <LiquidCrystal.h>

LiquidCrystal lcd(4,5,6,7,8,9);
Encoder myEnc(2,3);

boolean rev1 = true;
boolean rev2 = false;
boolean rev3 = false;
boolean rev4 = false;
boolean rev5 = false;
boolean rev6 = false;

float ch1Scale = 0;
float ch2Scale = 65;
float ch3Scale = 0;
float ch4Scale = 0;
float ch5Scale = 0;
float ch6Scale = 0;

int trim1 = 18;
int trim2 = 0;
int trim3 = 0;
int trim4 = 0;
int trim5 = 0;
int trim6 = 0;

int ch1 = 0;
int ch1Max = 580;
int ch1Min = 410;

int ch2 = 0;
int ch2Max = 675;
int ch2Min = 370;

int ch3 = 0;
int ch3Max = 1023;
int ch3Min = 0;

int ch4 = 0;
int ch4Max = 1023;
int ch4Min = 0;

int ch5 = 0;
int ch5Max = 1023;
int ch5Min = 0;

int ch6 = 0;
int ch6Max = 1008;
int ch6Min = 14;

float battMultiplier = 0.004889;
unsigned long time = 0;
int frameRate = 10;
boolean ack = false;

byte tempValue = 0;

boolean select = false;
int chSelect = 1;

byte serialBuffer[100];

byte mid[8] = {
  0b01110,
  0b00100,
  0b00100,
  0b11111,
  0b00100,
  0b00100,
  0b01110,
  0b00000
};

byte up1[8] = {
  0b01110,
  0b00100,
  0b11111,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b00000
};

byte up2[8] = {
  0b01110,
  0b11111,
  0b00100,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b00000
};

byte top[8] = {
  0b11111,
  0b00100,
  0b00100,
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b00000
};

byte down1[8] = {
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b11111,
  0b00100,
  0b01110,
  0b00000
};

byte down2[8] = {
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b00100,
  0b11111,
  0b01110,
  0b00000
};

byte bottom[8] = {
  0b01110,
  0b00100,
  0b00100,
  0b01110,
  0b00100,
  0b00100,
  0b11111,
  0b00000
};

byte rf1[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b00000
};

byte rf2[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01010,
  0b00000,
  0b00100,
  0b01110,
  0b00000
};

byte rf3[8] = {
  0b01110,
  0b10001,
  0b00100,
  0b01010,
  0b00000,
  0b00100,
  0b01110,
  0b00000
};

byte batt0[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000
};

byte batt1[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b00000
};

byte batt2[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

byte batt3[8] = {
  0b01110,
  0b10001,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

byte batt4[8] = {
  0b01110,
  0b10001,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

byte batt5[8] = {
  0b01110,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000
};

void setup() {  
  // initialize LCD and set up the number of columns and rows: 
  lcd.begin(8, 2);
  lcd.createChar(0, bottom);
  lcd.createChar(1, down2);
  lcd.createChar(2, down1);
  lcd.createChar(3, mid);
  lcd.createChar(4, up1);
  lcd.createChar(5, up2);
  lcd.createChar(6, top);
  Serial.begin(57600);
  Serial.setTimeout(20);
  pinMode(10, INPUT);
  digitalWrite(10, HIGH);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  pinMode(13, OUTPUT);
}

void loop() {
  checkEncoder();
  if (millis() >= time && ack == true) {
    digitalWrite(13, HIGH);
    time = millis() + frameRate;
    sendHeader();
    writePos(ch1Min, ch1Max, 1, rev1, ch1Scale, trim1);
    writePos(ch2Min, ch2Max, 2, rev2, ch2Scale, trim2);
    writePos(ch3Min, ch3Max, 3, rev3, ch3Scale, trim3);
    writePos(ch4Min, ch4Max, 4, rev4, ch4Scale, trim4);
    writePos(ch5Min, ch5Max, 4, rev5, ch5Scale, trim5);
    writePos(ch6Min, ch6Max, 4, rev6, ch6Scale, trim6);
    sendFooter();
    digitalWrite(13, LOW);
    ack = false;
  }
  //checkBatt();
  //checkAck();
  checkTelemetry();
  setScale();
}

void setScale() {
  ch1Scale = map(analogRead(4), 0, 1023, 0, 100);
}  

void checkAck() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil(0,serialBuffer,10);
    if (serialBuffer[0] == 'O' && serialBuffer[1] == 'K') {
      ack = true;
    }
  }
}

void checkTelemetry() {
  if (Serial.available() > 0) {
    Serial.readBytesUntil('\0',serialBuffer,10);
    if (serialBuffer[0] == 'O' && serialBuffer[1] == 'K') {
      ack = true;
    }
    else if (serialBuffer[0] == 1 && serialBuffer[1] == 1 && serialBuffer[2] == 1) {
      float _tempVoltage = serialBuffer[3];
      byte _tempCurrent = serialBuffer[4];
      
      lcd.setCursor(0,1);
      lcd.print(_tempVoltage/10);
      lcd.setCursor(3,1);
      lcd.print("V ");
      lcd.print(_tempCurrent);
      lcd.print("A ");
    }
    memset(serialBuffer,0,sizeof(serialBuffer)); //clear buffer
    Serial.flush();
  }
}

void sendHeader() {
  Serial.write(1);
  Serial.write(1);
  Serial.write(1);
}

void sendFooter() {
  Serial.print('\0');
}

void checkEncoder() {
  if (myEnc.read() == 0) {
    chSelect = 1;
  }
  else if (myEnc.read() >= 1 && myEnc.read() < 5) {
    //myEnc.write(4);
    chSelect = 2;
  }
  else if (myEnc.read() >= 5 && myEnc.read() < 9) {
    //myEnc.write(8);
    chSelect = 3;
  }
  else if (myEnc.read() >= 9 && myEnc.read() < 13) {
    //myEnc.write(12);
    chSelect = 4;
  }
  else if (myEnc.read() >= 13 && myEnc.read() < 17) {
    //myEnc.write(16);
    chSelect = 5;
  }
  else if (myEnc.read() >= 17 && myEnc.read() < 21) {
    //myEnc.write(20);
    chSelect = 6;
  }
  else if (myEnc.read() < 0 || myEnc.read() >= 21) {
    myEnc.write(0);
  }
  
  if (digitalRead(10) == LOW && !select) {
    select = true;
    chSelect = 1;
    myEnc.write(0);
    while (digitalRead(10) == LOW) {
    }
  }
  else if (digitalRead(10) == LOW && select) {
    select = false;
    while (digitalRead(10) == LOW) {
    }
  }
}

void checkBatt() {
  float _battLevel = analogRead(5) * battMultiplier;
  lcd.setCursor(5,1);
  lcd.print(_battLevel);
  //lcd.print("V ");
}
  
void writePos(int _chMin, int _chMax, int _chNum, boolean _reverse, float _chScale, int _trim) {
  unsigned long _chVal = 0;
  //_chMax = _chMax - ((_chMax - _chMin)*(_chScale/100)); //set scale
  //_chMin = _chMin + ((_chMax - _chMin)*(_chScale/100));
  
  int _chStep = (_chMax - _chMin)/7;
  int _chPos = 0;
  int _smoothing = 10;
  byte _outByte = 0;
    
  for (int i = 0; i < _smoothing; i++) {
     _chVal += analogRead(_chNum-1);
  }
  _chVal /= _smoothing;
  
  if (_chVal <= _chMin) {
    _chVal = _chMin;
  }
  else if (_chVal >= _chMax) {
    _chVal = _chMax;
  }
  
  if (!_reverse) {
    _outByte = byte(map(_chVal,_chMin,_chMax,1+(90*(_chScale/100)),181-(90*(_chScale/100))));
  }
  else if (_reverse) {
    _outByte = byte(map(_chVal,_chMin,_chMax,181-(90*(_chScale/100)),1+(90*(_chScale/100))));
  }

  _outByte += _trim; //account for channel trim

  /*if (_chNum == 1) {
    lcd.setCursor(0,1);
    lcd.print(_outByte);
    if (_outByte < 100) {
      lcd.setCursor(3,1);
      lcd.print(" ");
    }
  }*/
  
  if (_outByte >= 181) {
    _outByte = 181;
  }
  else if (_outByte <= 1) {
    _outByte = 1;
  }
  
  Serial.write(_outByte);
  
  if (_chVal > _chMax - _chStep) {
    _chPos = 6;
  }
  else if (_chVal > _chMax - (_chStep*2) && _chVal < _chMax - (_chStep)) {
    _chPos = 5;
  }
  else if (_chVal > _chMax - (_chStep*3) && _chVal < _chMax - (_chStep*2)) {
    _chPos = 4;
  }
  else if (_chVal > _chMax - (_chStep*4) && _chVal < _chMax - (_chStep*3)) {
    _chPos = 3;
  }
  else if (_chVal > _chMax - (_chStep*5) && _chVal < _chMax - (_chStep*4)) {
    _chPos = 2;
  }
  else if (_chVal > _chMax - (_chStep*6) && _chVal < _chMax - (_chStep*5)) {
    _chPos = 1;
  }
  else if (_chVal > _chMax - (_chStep*7) && _chVal < _chMax - (_chStep*6)) {
    _chPos = 0;
  }

  if (select && chSelect == _chNum) {
    lcd.setCursor(_chNum-1,0);
    lcd.print("*");
  }
  else {

  switch(_chPos){
    case 6:
      lcd.setCursor(_chNum-1,0);
      lcd.write(6);
      break;
    case 5:
      lcd.setCursor(_chNum-1,0);
      lcd.write(5);
      break;
    case 4:
      lcd.setCursor(_chNum-1,0);
      lcd.write(4);
      break;
    case 3:
      lcd.setCursor(_chNum-1,0);
      lcd.write(3);
      break;
    case 2:
      lcd.setCursor(_chNum-1,0);
      lcd.write(2);
      break;
    case 1:
      lcd.setCursor(_chNum-1,0);
      lcd.write(1);
      break;
    case 0:
      lcd.setCursor(_chNum-1,0);
      lcd.write(byte(0));
      break;
  }
  }
}
