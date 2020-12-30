/*
 * Pomodoro timer
 * 
 * Software for pomodoro timer built with Arduino
 * 
 * Features 3 push buttons, 6 LEDs, and an active piezzo
 * to play sounds and emulate a timer
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
int time_pomodoro = 20;
int time_break = 5;
int num_pomodoros = 4;

// variables to be used later in loops
int pomodoros_complete = 0;
long interrupt_interval_ms = 250;
long time_passed = 0;

long total_break = 0;
long total_pomodoro = 0;


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
    int noteDuration = 700 / noteDurations[thisNote];
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
 * Display a number in decimal using 5 blue LED
 * Range of numbers shown is 1-5
 */
void displayDecimal(int num) {
  int blueLED[5] = {0};
  int index = 0;
  while (num > 0) {
    blueLED[index] = 1;
    index++;
    num--;
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

  int interval = 150;
  
  while (enterState != HIGH) {    

    // read state of buttons
    minusState = digitalRead(minus);
    plusState = digitalRead(plus);
    enterState = digitalRead(enter);

    // alter variable value
    if (minusState == HIGH) {
      var--;
    }
    if (plusState == HIGH) {
      var++;
    }

    // reset variable if it is out of bounds, wraparound
    if (var == 0) {
      var = 31;
    }
    if (var == 32) {
      var = 1;
    }

    displayBinary(var);

    delay(interval);
  }

  // turn on green LED to provide user feedback for variable set sucessfully
  digitalWrite(g, HIGH);
  delay(500);
}

void turnOnLights() {
  int delay2 = 100;
  digitalWrite(b0, HIGH);
  delay(delay2);
  digitalWrite(b0, LOW);
  digitalWrite(b1, HIGH);
  delay(delay2);
  digitalWrite(b1, LOW);
  digitalWrite(b2, HIGH);
  delay(delay2);
  digitalWrite(b2, LOW);
  digitalWrite(b3, HIGH);
  delay(delay2);
  digitalWrite(b3, LOW);
  digitalWrite(b4, HIGH);
  delay(delay2);
  digitalWrite(b4, LOW);
  digitalWrite(g, HIGH);
  delay(delay2);
  digitalWrite(g, LOW);
  digitalWrite(b4, HIGH);
  delay(delay2);
  digitalWrite(b4, LOW);
  digitalWrite(b3, HIGH);
  delay(delay2);
  digitalWrite(b3, LOW);
  digitalWrite(b2, HIGH);
  delay(delay2);
  digitalWrite(b2, LOW);
  digitalWrite(b1, HIGH);
  delay(delay2);
  digitalWrite(b1, LOW);
  digitalWrite(b0, HIGH);
  delay(delay2);
  digitalWrite(b0, LOW);
  delay(500);
}


/**
 * Sequence of LED to turn on/off to indicate start of a work and break segment
 */
void startWork() {
  int delay_ms = 250;
  int delay1 = 100;
  digitalWrite(b0, HIGH);
  delay(delay1);
  digitalWrite(b1, HIGH);
  delay(delay1);
  digitalWrite(b2, HIGH);
  delay(delay1);
  digitalWrite(b3, HIGH);
  delay(delay1);
  digitalWrite(b4, HIGH);
  delay(delay1);
  digitalWrite(g, HIGH);
  delay(delay1);
  allLEDoff();
  delay(delay_ms);
  allLEDon();
  delay(delay_ms);
  allLEDoff();
  delay(delay_ms);
  allLEDon();
  delay(delay_ms);
  allLEDoff();
  delay(delay_ms);
  allLEDon();
  delay(delay_ms);
  allLEDoff();
}

void startBreak() {
  allLEDon();
  playBreaktime();
  allLEDoff();
}

/**
 * Work sequence
 */
void onePomodoro() {
  
  time_passed = 0;

  Serial.print(total_pomodoro);

  startWork();
  
  while (time_passed < total_pomodoro) {
    enterState = digitalRead(enter);
    if (enterState == HIGH) {
      /**
       * press enter to jump directly to break
       */
      enterState = LOW;
      break;
    }
    
    time_passed += interrupt_interval_ms;
    delay(interrupt_interval_ms);
    Serial.print(time_passed);
    Serial.print("\n");
  }

  // alerts for start of break
  startBreak();
}


/**
 * Short break sequence
 */
void shortBreak() {
  
  time_passed = 0;

  Serial.print(total_break);

  displayDecimal(pomodoros_complete);
  
  while (time_passed < total_break) {
    time_passed += interrupt_interval_ms;
    delay(interrupt_interval_ms);
    Serial.print(time_passed);
    Serial.print("\n");
  }

  // after break over, lights turn on
  digitalWrite(g, HIGH);
  // press enter to start another work group
  while (enterState != HIGH) {
    enterState = digitalRead(enter);
    delay(100);
  }
  resetButtonStates();
  allLEDoff();
}


/**
 * Setup code, only run once when board first starts up
 */
void setup() {

  Serial.begin(9600);

  pinMode(g,  OUTPUT);
  pinMode(b0, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(b3, OUTPUT);
  pinMode(b4, OUTPUT);

  pinMode(enter, INPUT);
  pinMode(minus, INPUT);
  pinMode(plus,  INPUT);

  turnOnLights();
  int set_var_delay = 250;

  // set pomodoro time
  setVariable(time_pomodoro);
  total_pomodoro = time_pomodoro * 60000;
  allLEDoff();
  resetButtonStates();
  Serial.print(time_pomodoro);
  Serial.print("\n");
  delay(set_var_delay);

  // set break time
  setVariable(time_break);
  total_break = time_break * 60000;
  allLEDoff();
  resetButtonStates();
  Serial.print(time_pomodoro);
  Serial.print("\n");
  delay(set_var_delay);

  // set number of pomodoros before long break
  setVariable(num_pomodoros);
  allLEDoff();
  resetButtonStates();
  Serial.print(num_pomodoros);
  Serial.print("\n");
  delay(set_var_delay);
}


/**
 * Run loop forever, timer portion
 */
void loop() {

  onePomodoro();
  pomodoros_complete++;

  // long break after complete set number of pomodoros
  // get up and do something else for a half hour
  if (pomodoros_complete == num_pomodoros) {
    pomodoros_complete = 0;
    allLEDon();
    resetButtonStates();
    while (enterState != HIGH) {
      enterState = digitalRead(enter);
    }
    resetButtonStates();
    allLEDoff();
  }
  // otherwise take short break
  else {
    shortBreak();
  }
  
}
