//
// Parse incoming messages consisting of three decimal values followed by a carriage return
//  Example  "12 34 56\n"
//  In TouchDesigner:     op('serial1').send("12 34 56", terminator="\n")
//
char buffer[16];   //maximum expected length 
int len = 0;
#include <Servo.h>

Servo myservo1;

void setup()
{
  Serial.begin(9600); 
  pinMode(9, OUTPUT); 
  myservo1.attach(9);  
}
void loop()
{
    if (Serial.available() > 0) 
    {
        int incomingByte = Serial.read();
        buffer[len++] = incomingByte;
        //
        // check for overflow
        //
        if (len >= 16)
        {
            // overflow, resetting
            len = 0;
        }
        //
        // check for newline (end of message)
        //
        if (incomingByte == '\n')
        {
            int val1;
            int n = sscanf( buffer, "%d" , &val1 );
            if (n == 1)
            {
                //Serial.print(val1);
                myservo1.write(val1);
            }
            else
            {
                 // parsing error, reject
            }
            len = 0; // reset buffer counter
        }
    }
}
