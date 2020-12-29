/*
  Melody

  Plays a melody

  circuit:
  - 8 ohm speaker on digital pin 8

  created 21 Jan 2010
  modified 30 Aug 2011
  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Tone
*/

#include "pitches.h"


// THE LICK
int melody[] = {
  NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_F6, NOTE_D6, NOTE_AS5, NOTE_C6
};
int noteDurations[] = {
  4, 4, 4, 4, 2, 4, 2
};


// Declare button pins
const int enter = 1;
const int minus = 2;
const int plus = 3;

int enterState = 0;
int minusState = 0;
int plusState = 0;

int buzzer = 7;

const int g = 5;
const int b0 = 9;
const int b1 = 10;
const int b2 = 11;
const int b3 = 12;
const int b4 = 13;


void setup() {
  pinMode(g,  OUTPUT);
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);

  pinMode(enter, INPUT);
  pinMode(minus, INPUT);
  pinMode(plus,  INPUT);

  // blink lights or something cool

  // set pomodoro time

  // set break time

  // set number of pomodoros before long break

  // all lights flash, wait for start button to be pressed
  
}


void playBreaktime() {
  
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzer, melody[thisNote], noteDuration);
    
    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(buzzer);
  }
}


// the loop function runs over and over again forever
void loop() {

  // check 


  enterState = digitalRead(enter);

  if (enterState == HIGH) {
    digitalWrite(g, HIGH);
    digitalWrite(b0, HIGH);
    digitalWrite(b1, HIGH);
    digitalWrite(b2, HIGH);
    digitalWrite(b3, HIGH);
    digitalWrite(b4, HIGH);
//    playBreaktime();
  }
  else {
    digitalWrite(g, LOW);
    digitalWrite(b0, LOW);
    digitalWrite(b1, LOW);
    digitalWrite(b2, LOW);
    digitalWrite(b3, LOW);
    digitalWrite(b4, LOW);
  }

  delay(50);
  
}
