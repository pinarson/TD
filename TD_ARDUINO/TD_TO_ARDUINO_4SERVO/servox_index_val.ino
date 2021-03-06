//
// Parse incoming messages consisting of 2 decimal values followed by a carriage return
//  Exemple : msg = (str(channel.index) +' '+ str(val) )
//  In TouchDesigner:    op('serial1').send( msg, terminator='\n') 
//
#include <Servo.h>


//Setup buffer
char buffer[16];   //maximum expected length 
int len = 0;
Servo myservo1; // create Servo object 
Servo myservo2;
Servo myservo3;
Servo myservo4;

void setup()
{
  Serial.begin(9600); 
  
  //define the three pins as outputs
  Serial.begin(9600); 
  pinMode(9, OUTPUT); 
  myservo1.attach(9);  
  pinMode(10, OUTPUT); 
  myservo2.attach(10);
  pinMode(5, OUTPUT); 
  myservo3.attach(5);  
  pinMode(3, OUTPUT); 
  myservo4.attach(3); 
  
//Stanby position servo 90 or close to 90 ex:servo4 value -> position zero = 84
  myservo1.write(90);
  myservo2.write(90);
  myservo3.write(90);
  myservo4.write(84);
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
            int index;
            int val;
            int n = sscanf(buffer, "%d %d", &index, &val);
            
            if(n == 2)
            {
              if(index == 0)
              {
                myservo1.write(val);
                Serial.println(val);  
              }
              if(index == 1)
              {
                myservo2.write(val);
                Serial.println(val);
              }
              if(index == 2)
              {
                myservo3.write(val);
                Serial.println(val);
              }
              if(index == 3)
              {
                myservo4.write(val);
                Serial.println(val);
              }
            }
            else
            {
                 // parsing error, reject
                 Serial.print("PARSING ERROR, REJECTED!!!");
            }
            len = 0; // reset buffer counter
        }
    }
}
