#include <Keypad.h>



String password = "";
String pass[] = {"1234","5678","9012"};
boolean correctpass = false;
int currentPhase = 0 ;    //which phase of the puzzle are they on

const byte ledpins[] = {2, 3, 4}; //pins for success leds
int ledNum = 3;                   //change this number for more leds (and add pins to above array)
const byte lock = 12;             //pin for relay to maglock

// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
const byte ROWS = 4; // Four rows
const byte COLS = 4; // columns

byte rowPins[ROWS] = { 10, 9, 8, 7 };// Connect keypad COL0, COL1 and COL2 to these Arduino pins.
byte colPins[COLS] = { A0, A1, A2, A3 };

// Define the Keymap
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Create the Keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  for (int i = 0 ; i < ledNum ; i++) {
    pinMode(ledpins[i], OUTPUT);
  }
  puzzlereset();
  keypad.addEventListener(keypadEvent); //add an event listener for this keypad


}

void loop() {
  // put your main code here, to run repeatedly:
  delay(50);
  keypad.getKey();
  if (correctpass) {
    unlock();
  }
}

void keypadEvent(KeypadEvent eKey) {
  switch (keypad.getState()) {
    case PRESSED:

      Serial.print("Enter:");
      Serial.println(eKey);
      delay(10);
      switch (eKey) {
        case '#': checkPassword(); delay(1); password = ""; break;

        case '*': password = ""; delay(1); break;

        default: password = password + eKey; break;
      }
  }
}

void checkPassword() {
  if (password == pass[currentPhase]) {
    digitalWrite(ledpins[currentPhase],HIGH);
    currentPhase ++;
    Serial.println("PASSWORD CORRECT");
  }
  if (currentPhase == ledNum){
    correctpass = true;
  }
  else {
    correctpass = false;
  }
}


void unlock() {
  Serial.println("UNLOCKED");
  digitalWrite(lock, HIGH);
  delay(2000);
  puzzlereset();
}

void puzzlereset() {
  
  Serial.println("puzzle reset");
  correctpass = false;
  for (int i = 0 ; i < ledNum ; i++) {
    digitalWrite(ledpins[i], LOW);
  }
  digitalWrite(lock, LOW);

}
