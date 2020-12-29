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





// Declare button pins
const int enter = 2;
const int minus = 3;
const int plus = 4;

int enterState = 0;
int minusState = 0;
int plusState = 0;


const int buzzer = 7;
const int g = 5;
const int b0 = 13;
const int b1 = 12;
const int b2 = 11;
const int b3 = 10;
const int b4 = 9;


// time for pomodoro intervals, in minutes
int time_pomodoro = 25;
int time_break = 5;
int num_pomodoros = 4;


/**
 * play a melody to indicate beginning of a pomodoro session
 */
void playPomodoro() {
   int melody[] = {
    NOTE_C5, NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6
  };
  int noteDurations[] = {
    4, 4, 4, 4, 4, 4, 4, 4
  };

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

/**
 * Play a melody to indicate it is time for a break
 */
void playBreaktime() {

  // THE LICK
  int melody[] = {
    NOTE_C6, NOTE_D6, NOTE_DS6, NOTE_F6, NOTE_D6, NOTE_AS5, NOTE_C6
  };
  int noteDurations[] = {
    4, 4, 4, 4, 2, 4, 2
  };
  
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 7; thisNote++) {

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


void allLEDoff() {
  digitalWrite(g, LOW);
  digitalWrite(b0, LOW);
  digitalWrite(b1, LOW);
  digitalWrite(b2, LOW);
  digitalWrite(b3, LOW);
  digitalWrite(b4, LOW);
}

void allLEDon() {
  digitalWrite(g, HIGH);
  digitalWrite(b0, HIGH);
  digitalWrite(b1, HIGH);
  digitalWrite(b2, HIGH);
  digitalWrite(b3, HIGH);
  digitalWrite(b4, HIGH);
}

void resetButtonStates() {
  enterState = 0;
  minusState = 0;
  plusState = 0;
}


/**
 * Show an integer in binary using the 5 blue LED
 * Maximum int that can be shown = 31
 */
void displayBinary(int num) {
  int blueLED[5] = {0};
  int index = 0;
  int rem = 0;
  
  while (num > 0) {
    rem = num % 2;
    num = num / 2;
    blueLED[index] = rem;
    index++;
  }
  
  digitalWrite(b0, blueLED[0]);
  digitalWrite(b1, blueLED[1]);
  digitalWrite(b2, blueLED[2]);
  digitalWrite(b3, blueLED[3]);
  digitalWrite(b4, blueLED[4]);
}


/**
 * Use buttons and LED to set timer settings for pomodoro
 */
void setVariable(int & var) {
  while (enterState != HIGH) {    
    
    minusState = digitalRead(minus);
    plusState = digitalRead(plus);

    if (minusState == HIGH) {
      var--;
    }

    if (plusState == HIGH) {
      var++;
    }

    displayBinary(var);

    // Turn on green LED if number is valid, aka between 1-31
    if (var >= 1 && var <= 31) {
      digitalWrite(g, HIGH);
    }
    else {
      digitalWrite(g, LOW);
    }

    enterState = digitalRead(enter);
    delay(150);
  }
}


/**
 * Setup code, only run once when board first starts up
 */
void setup() {

  Serial.begin(9600);

  // Set pin modes
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
  setVariable(time_pomodoro);
  allLEDoff();
  resetButtonStates();
  Serial.print(time_pomodoro);
  delay(500);

  // set break time
  setVariable(time_break);
  allLEDoff();
  resetButtonStates();
  Serial.print(time_break);
  delay(500);

  // set number of pomodoros before long break
  setVariable(num_pomodoros);
  allLEDoff();
  resetButtonStates();
  Serial.print(num_pomodoros);
  delay(500);
  
  // startup sequence
  digitalWrite(b0, HIGH);
  delay(100);
  digitalWrite(b1, HIGH);
  delay(100);
  digitalWrite(b2, HIGH);
  delay(100);
  digitalWrite(b3, HIGH);
  delay(100);
  digitalWrite(b4, HIGH);
  delay(100);
  digitalWrite(g, HIGH);
  playPomodoro();
  allLEDoff();

  
}



/**
 * Run loop forever, timer portion
 */
void loop() {

  delay(500);
  
}
