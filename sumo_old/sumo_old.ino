#include <Move.h>
#include <Sight.h>
#include <Surface.h>

/**
 * Globals
 */
Move move;
Sight sight;
Surface surface;
int turn = 0;
bool escapeRight = LOW;
bool escapeLeft = LOW;
int flPin = 50;
int blPin = 52;
int frPin = 51;
int brPin = 53;

/*
int readQD()
{
  //Returns value from the QRE1113
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( QRE1113_Pin, OUTPUT );
  digitalWrite( QRE1113_Pin, HIGH );
  delayMicroseconds(10);
  pinMode( QRE1113_Pin, INPUT );
  
  long time = micros();
  
  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(QRE1113_Pin) == HIGH && micros() – time < 3000);
   int diff = micros() - time; 
   return diff;

} 
*/

int half = 1115;
const int trigPin = 3;
const int echoPin = 2;
float tolerance = 45;
float duration, distance;


void setup() 
{
  /**
   * Setup for IRs
   */
  pinMode(blPin, INPUT);
  pinMode(brPin, INPUT);
  pinMode(frPin, INPUT);
  pinMode(flPin, INPUT);

  /**
   * Setup for Sonic Sensors
   */
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
   
  /**
   * Set Data Rate
   * Only Serial.print after set
   */
  Serial.begin(9600);
}

void loop() 
{

/**
 * Turn around
 */
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  if(distance <= tolerance)
  {
    move.right();
    delay(half);
  }

  move.forward();


/**
 * Escape Sequence
 */
digitalWrite(blPin, HIGH);
digitalWrite(brPin, HIGH);
digitalWrite(frPin, HIGH);
digitalWrite(flPin, HIGH);
 
Serial.print("Back left pin reading: ");
Serial.println(digitalRead(blPin));
 
Serial.print("Back right pin reading: ");
Serial.println(digitalRead(brPin));

Serial.print("Front right pin reading: ");
Serial.println(digitalRead(frPin));

Serial.print("Front left pin reading: ");
Serial.println(digitalRead(flPin));
delay(1000);

// if either of the front sensors see the line, robot must escape
if(digitalRead(frPin) == 0 ) 
{
    escapeLeft = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4);  
}
else if(digitalRead(flPin) == 0)
{
    escapeRight = HIGH;
    // turn iterator initialized to random value between 1 and 5
    turn = random(2,4);
}

//     escape right loop
while(escapeRight == HIGH) 
{
    if(turn > 0) 
    {
    // decrement turn iterator and move right 1 second per decrement
    turn -= 1;
    move.right();
    delay(250);
    }
    
    else
    {
    // break the loop once the turn iterator is 0
    turn = 0;
    escapeRight = LOW;
    }
}

//     escape left loop
while(escapeLeft == HIGH) 
{
      if(turn > 0) 
      {
        // decrement turn iterator and move right 1 second per decrement
       turn -= 1;
       move.left();
        delay(250);
      }


      else
      {
        // break the loop once the turn iterator is 0
        turn = 0;
        escapeLeft = LOW;
      }
}
   
    // search
    if(escapeLeft != HIGH && escapeRight != HIGH) 
    {
      move.forward();
    }
}
