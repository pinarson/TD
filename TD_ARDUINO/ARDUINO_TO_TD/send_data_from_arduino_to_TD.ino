void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
}
void loop()
{
  delay(100);
  int val = analogRead(00);
  Serial.print(val, DEC);
  Serial.print('\n');
 
}
