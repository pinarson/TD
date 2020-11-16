//
// Parse incoming messages consisting of three decimal values followed by a carriage return
//  Example  "12 34 56\n"
//  In TouchDesigner:     op('serial1').send("12 34 56", terminator="\n")
//
#include <Servo.h>
#include <Adafruit_NeoPixel.h> // Charge la librairie Neo Pixel d'Adafruit utilisé pour piloter le ruban de LED

#define PIXEL_PIN 6 // On définit le pin où est connecté la patte DATA du bandeau
#define PIXEL_COUNT 4 // On définit le nombre de LED compris sur le Ruban de LED soit 150 pour le ruban de 5m50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800); // Paramètre l'objet strip qui correspond à toute les LED du ruban


char buffer[19];   //maximum expected length 
int len = 0;
Servo myservo1; // create Servo object 
Servo myservo2;

void setup()
{
  strip.begin(); // Lance la connection
  strip.show(); // Initialise toute les led à 'off'
  Serial.begin(9600); 
  pinMode(9, OUTPUT); 
  myservo1.attach(9);  
  pinMode(10, OUTPUT); 
  myservo2.attach(10);
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
        if (len >= 19)
        {
            // overflow, resetting
            len = 0;
        }
        //
        // check for newline (end of message)
        //
        if (incomingByte == '\n')
        {
            int red, green, blue, tilt, pan;
            int n = sscanf(buffer, "%d %d %d %d %d", &red, &green, &blue, &tilt, &pan);
            if (n == 5)
            {
                all(red, green, blue);
                myservo1.write(tilt);
                myservo2.write(pan);
            }
            else
            {
                 // parsing error, reject
            }
            len = 0; // reset buffer counter
        }
    }
}


void all(int r, int v, int b)
{
   for(int i = 0 ; i < 4 ; i++)
  {
     strip.setPixelColor(i, r, v, b);
  }
  strip.show();
   delay(10); 
}
