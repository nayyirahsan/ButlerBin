#include <IRremote.h>
#include <math.h>

// Pin definitions for L298N Motor Driver
const int enA = 8;  
const int in1 = 9;  
const int in2 = 10; 

const int in3 = 11; 
const int in4 = 12; 
const int enB = 13; 

// IR receiver pin
const int receiverPin = 7; 

// Button definitions (adjust the numbers as needed)
#define IR_BUTTON_1 12
#define IR_BUTTON_REWIND 

IRrecv irrecv(receiverPin);
decode_results results;

// Room dimensions and anchor point coordinates
float roomW = 20.0;  // Width of the room
float roomL = 10.0;  // Length of the room
float x1 = 0, y1 = 0;
float x2 = 0, y2 = roomL;
float x3 = roomW / 2, y3 = roomL;

// Original location
const int originalX = 0;
const int originalY = 0;

// Current location
int currentX = 0;
int currentY = 0;


void triangulate(float d1, float d2, float d3, int coord[]);
void moveToOriginalPosition();
void stopMotors();
void startMotors();

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enA, OUTPUT);
  
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);

  irrecv.enableIRIn();
  Serial.begin(9600);

  // Set current location to "original" location
  currentX = originalX;
  currentY = originalY;
}

void loop() {
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX); 

    // Start the motors when button "1" is pressed
    if (results.value == IR_BUTTON_1) {
      startMotors();
    }
    
    // Move to the original location when the rewind button is pressed
    if (results.value == IR_BUTTON_REWIND) {
      moveToOriginalPosition();
    }

    irrecv.resume(); 
  }

  // Regularly update the current position using triangulation
  
  float d1 = 5.0, d2 = 15.0, d3 = 10.0;
  int coord[2];
  triangulate(d1, d2, d3, coord);
  currentX = coord[0];
  currentY = coord[1];
}

// Function to move the car back to its original position
void moveToOriginalPosition() {
  while (currentX != originalX || currentY != originalY) {
    
    if (currentX < originalX) {
      // Move right
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
    } else if (currentX > originalX) {
      // Move left
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
    }

    if (currentY < originalY) {
      // Move forward
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    } else if (currentY > originalY) {
      // Move backward
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    // Update current position
    float d1 = 5.0, d2 = 15.0, d3 = 10.0; // Random sample values
    int coord[2];
    triangulate(d1, d2, d3, coord);
    currentX = coord[0];
    currentY = coord[1];
  }

  // Stop the motors once the original position is reached
  stopMotors();
}

// Triangulation function to calculate current position
void triangulate(float d1, float d2, float d3, int coord[]){
  float x = (pow(d1, 2.0) - pow(d2, 2.0) + pow(roomW, 2.0))/(2*roomW);
  
  float y = sqrt(pow(d3, 2.0) - pow(x - x3, 2.0));
  y = roomL - y;
  coord[0] = (x + 0.5); 
  coord[1] = (y + 0.5); 
}

// Function to start the motors
void startMotors() {
  // Motor A forward
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, 255); 

  // Motor B forward
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, 255); 
}

// Function to stop the motors
void stopMotors() {

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);

  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}
