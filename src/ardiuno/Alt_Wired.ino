#include <ESP32Servo.h>

//Joystick Pins
const byte joyStickXPin = 39;
const byte joyStickYPin = 36;
const byte joyStickPin = 4;

//Servo Pin
const byte servoPin = 18;

//Servo Object
Servo myServo;

void setup() 
{
  Serial.begin(9600);
  analogSetAttenuation(ADC_11db);

  //Joystick
  pinMode(joyStickPin, INPUT_PULLUP);

  myServo.attach(servoPin);

}

void loop() 
{
  int valX, valY, valSW;

  //Read input from the joystick pins
  ReadJoystickInput(valX, valY, valSW);
  
  EvaluateDeadzones(valX, valY);

  int servoAngle = DetermineServoAngle(valX);

  SetServoAngle(servoAngle);

  PrintInSerial(valX, valY, valSW);

  delay(100);

}

void ReadJoystickInput(int &x, int &y, int &pressed)
{
  x = analogRead(joyStickXPin);
  y = analogRead(joyStickYPin);
  pressed = digitalRead(joyStickPin);
}

void EvaluateDeadzones(int &x, int &y)
{
  //Check x deadzone value
  if ((x > 80) && (x < 130)) 
  {
    x = 105; 
  }

  //Check y deadzone values
  if ((y > 1900) && (y < 2000)) 
  {
    y = 1955;
  }
}

int DetermineServoAngle(int x)
{
  int servoAngle = map(x, 105, 191, 0, 180);

  return constrain(servoAngle, 0, 180);
}

void SetServoAngle(int angle)
{
  myServo.write(angle);
}

void PrintInSerial(int x, int y, int pressed)
{
  Serial.print("X: ");
  Serial.print(x);
  Serial.print("\t| Y: ");
  Serial.print(y);
  Serial.print("\t| Button: ");
  
  if (pressed == LOW) 
  {
    Serial.println("PRESSED");
  } 
  else 
  {
    Serial.println("RELEASED");
  }
}
