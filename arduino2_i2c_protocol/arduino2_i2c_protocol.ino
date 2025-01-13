#include <Wire.h>
#include <LedControl.h>
// MAX7219 LED Matrix pin configuration
const int DIN_PIN = 8;
const int CS_PIN = 10;
const int CLK_PIN = 9;
LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

// happy face
byte hf[8] = {
  B00111100,
  B01000010,
  B10010101,
  B10100001,
  B10100001,
  B10010101,
  B01000010,
  B00111100
};

int motor1pin1 = 8;
int motor1pin2 = 9;

#define ENCA 2
#define ENCB 3

volatile int pos = 0;
volatile int rev = 0;

void setup(){
  Serial.begin(9600);

  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);

  attachInterrupt(digitalPinToInterrupt(ENCA),interrupt,RISING);

  Wire.begin(8);
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);

  // Initialize MAX7219 LED Matrix
  lc.shutdown(0, false); // Wake up the display
  lc.setIntensity(0, 1); // Set brightness to a medium value
  lc.clearDisplay(0);    // Clear the display

  // Display happy face on startup
  displayHappy();
}

void loop(){
  Serial.println(rev);
  if(pos>470){
    pos = 0;
    rev++;
  }else if(pos<-480){
    pos = 0;
    rev--;
  }
}

void interrupt(){
  if(digitalRead(ENCA) > digitalRead(ENCB)){
    pos++;
  }else{
    pos--;
  }
}

void requestEvent(){
  Wire.write((pos >> 8) & 0xFF);
  Wire.write(pos & 0xFF);
  Wire.write((byte)rev);
}

void receiveEvent() {
  while(Wire.available()){
    if(Wire.read() == 0x01){
      Serial.println("wow");
      rev = 0;
      pos = 0;
    }
  }
}

void displayHappy() {
  for (int i = 0; i < 8; i++) {
    lc.setRow(0, i, hf[i]);
  }
}