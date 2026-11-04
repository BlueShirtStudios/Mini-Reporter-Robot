const byte joystickXPin = 39;
const byte joystickYPin = 36;

int xMin = 4095, xMax = 0, yMin = 4095, yMax = 0;

void setup()
{
  Serial.begin(115200);
  analogSetAttenuation(ADC_11db);
}

void loop()
{
  int x = analogRead(joystickXPin);
  int y = analogRead(joystickYPin);

  xMin = min(xMin, x); xMax = max(xMax, x);
  yMin = min(yMin, y); yMax = max(yMax, y);

  Serial.printf("X:%d (%d..%d)  Y:%d (%d..%d)\n", x, xMin, xMax, y, yMin, yMax);
  delay(50);
}