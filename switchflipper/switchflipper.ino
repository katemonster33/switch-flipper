#include <Wire.h>

#define SLAVE_ADDRESS 0x04
#define FIRSTPIN 22
#define LASTPIN 41
#define PWM0 4
#define PWM1 13
#define MODE_SETSWITCH 1
#define MODE_GETSWITCH 2
#define MODE_SET_TEMP 3
#define MODE_SET_T_STATE 4
#define MODE_SET_PWM 5

#define T_STATE_OFF 0
#define T_STATE_COOL 1
#define T_STATE_HEAT 2

#define CURRENTTEMP_PIN 5
#define AC_PIN 43
#define HEAT_PIN 44
#define FAN_PIN 45
byte outputStates[24];

void setup() 
{
  for(int i = FIRSTPIN; i <= LASTPIN; i++)
  {
    pinMode(i, OUTPUT);
  }
  pinMode(PWM0, OUTPUT);
  //digitalWrite(i, 127);
  Serial.begin(9600); // start serial for output
  // initialize i2c as slave
  Wire.begin(SLAVE_ADDRESS);
  
  // define callbacks for i2c communication
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  
  Serial.println("Ready!");
}


float settemp = 22.0;
int thermalMode = 0; // OFF
bool heatActive = false;
bool fanActive = false;
void loop() 
{
//  float currentTemp = analogRead(CURRENTTEMP_PIN) / 205.8;
//  currentTemp = (currentTemp - 0.5) / 10;
//  if(thermalMode == T_STATE_COOL)
//  {
//    if(currentTemp <= settemp)
//    {
//      digitalWrite(AC_PIN, LOW);
//    }
//    else
//    {
//      digitalWrite(AC_PIN, HIGH);
//    }
//  }
//  else if(thermalMode == T_STATE_HEAT)
//  {
//    if(currentTemp >= settemp)
//    {
//      digitalWrite(HEAT_PIN, LOW);
//      digitalWrite(FAN_PIN, LOW);
//      heatActive = false;
//      fanActive = false;
//    }
//    else
//    {
//      if(heatActive && !fanActive)
//      {
//        digitalWrite(FAN_PIN, HIGH);
//        fanActive = true;
//      }
//      else if(!heatActive)
//      {
//        digitalWrite(HEAT_PIN, HIGH);
//        heatActive = true;
//      }
//    }
//  }
//  delay(30000);
  delay(1000);
}
byte bytesReceived[6];
int numBytesRecvd = 0;
byte mode = 0;
byte selectedPin = 0;
byte state = 0;
// callback for received data
void receiveData(int byteCount)
{
  while(Wire.available())
  {
    int tmp = Wire.read();
    if(tmp == 0)
    {
      continue;
    }
    mode = tmp;
    Serial.print(mode);
    Serial.print(' ');
    if(mode == MODE_SETSWITCH && Wire.available() == 2)
    { 
      selectedPin = Wire.read();
      Serial.print(selectedPin);
      Serial.print(' ');
      state = Wire.read();
      Serial.print(state);
      outputStates[selectedPin] = state;
      if(state == 1)
      {
        digitalWrite(selectedPin + FIRSTPIN, LOW);
      }
      else
      {
        digitalWrite(selectedPin + FIRSTPIN, HIGH);
      }
    }
    else if(mode == MODE_GETSWITCH && Wire.available() == 1)
    {
      selectedPin = Wire.read();
      if(selectedPin < 24)
      {
        state = outputStates[selectedPin];
      }
      else
      {
        state = 255;
      }
    }
    else if(mode == MODE_SET_TEMP && Wire.available() == 1)
    {
      byte b1 = Wire.read();
      settemp = ((float)b1 / 4.0);
    }
    else if(mode == MODE_SET_T_STATE && Wire.available() == 1)
    {
      thermalMode = Wire.read();
    }
    else if(mode == MODE_SET_PWM && Wire.available() == 2)
    {
      selectedPin = Wire.read();
      selectedPin = selectedPin == 0 ? PWM0 : PWM1;
      state = Wire.read();
      analogWrite(selectedPin, state);
      Serial.print(selectedPin);
      Serial.print(' ');
      Serial.print(state);
      Serial.print(' ');
    }
    Serial.println();
  }
}

// callback for sending data
void sendData()
{
  if(mode == MODE_GETSWITCH)
  {
    Wire.write(state);
  }
  else
  {
    Wire.write(mode);
  }
}
