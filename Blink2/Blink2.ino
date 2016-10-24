/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN takes care 
  of use the correct LED pin whatever is the board used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
*/
const int buttonPin = 2;     // the number of the pushbutton pin
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int outputPort[] = {3,5,6};
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  
  for(int i=0; i<3; i++){
    pinMode(outputPort[i], OUTPUT);
  }
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if(buttonState){
  for(int i=0;i<3;i++){
    digitalWrite(outputPort[i], HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1000);                       // wait for a second
    digitalWrite(outputPort[i], LOW);    // turn the LED off by making the voltage LOW
    buttonState = 0;
    }
  }
}
