/*
  this code assumes the room is 80 cm by 80 cm and the bot is at 0,0
  while the phone is at 8,8.

  not all the constants are here, some might be in the other ino files

  make sure you download the arduino2 i2c code on the second arduino too

  the code needs:
  - function to assign bluetooth distances to triangulate or some way to
    get the coordinates of the phone
  
  - triangulate function needs to be fixed so it only uses two arguments

  - 3rd coordinates of esp need to be deleted

  - boolean or other indicator that the bin has been called/dismissed

  - testing for other angles

  other things needed:
  - extra features added to arduino2
*/
#include <IRremote.h>
#define IR_RECEIVE_PIN A0
// Initialize the IR receiver
IRrecv irrecv(IR_RECEIVE_PIN);
decode_results results;

#include <Servo.h>
const int servoPin = 13;   // Servo motor control pin
Servo myServo; // Create a Servo object


//encoders
#include <Wire.h>

int motor1pin1 = 4;
int motor1pin2 = 5;

int motor2pin1 = 6;
int motor2pin2 = 7;

#define ENCA 2 // YELLOW
#define ENCB 3 // WHITE

volatile int pos1 = 0;
volatile int pos2 = 0;

float pAngle1 = 0;
float pAngle2 = 0;

volatile float angle1, angle2;
int rev1, rev2;

float trashCanAngle = 0.0;

//triangulation
#include <math.h>

//constants
float roomW = 20.0;
float roomL = 20.0;

float x1 = 0, y1 = 0;
float x2 = 0, y2 = roomL;
float x3 = (roomW/2), y3 = roomL;

#define REV_TO_CM 1.35 //this is actually decimeters, i just didnt feel like changing the name everywhere
#define DEG_PER_REV 71 //may need to adjust

void position2(){
  Wire.requestFrom(8, 3);
  pos2 = 0;
  rev2 = 0;

  while (Wire.available()) {
    pos2 = Wire.read() << 8;
    pos2 |= Wire.read();
    rev2 = (int8_t)Wire.read();
  }

  Wire.endTransmission();

  if(pos2<0){ //these values are the pulses per rev of the second motor because they were weird for me
    angle2 = pos2/470.0 * 360.0; //default is 540 so if this seems like too little try that first
  }else{                     //you will need to adjust on both this code and arduino2 code
    angle2 = pos2/480.0 * 360.0;
  }
}

void interrupt(){
  if(pos1 > 350){
    pos1 = 0;
    rev1++;
  }else if(pos1 < -330){
    pos1 = 0;
    rev1--;
  }
  if(digitalRead(ENCA) > digitalRead(ENCB)){
    pos1--;
  }
  else{
    pos1++;
  }

  if(angle1 > 0){
    angle1 = pos1/350.0 * 360.0;
  }else{
    angle1 = pos1/330.0 * 360.0;
  }
}

void setup() {
  // Attach the servo motor to the servo pin
  myServo.attach(servoPin);
  
  // Move servo to initial position
  myServo.write(180);

  Serial.begin(9600);
  
  pinMode(ENCA,INPUT);
  pinMode(ENCB,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(ENCA),interrupt,RISING);
  
  pinMode(motor1pin1, OUTPUT);
  pinMode(motor1pin2, OUTPUT);
  pinMode(motor2pin1, OUTPUT);
  pinMode(motor2pin2, OUTPUT);

  Wire.begin();

  trashCanAngle = 0.0;

  IrReceiver.begin(IR_RECEIVE_PIN);

}

int isGoing = 0;

bool getCoords = true;
bool turning = false;
bool angleAdjust = false;
bool goFwd = false;
bool leave = false;

int binCoord[2];
int phoneCoord[2];
int homeCoord[2] = {0, 0};
float turnToAngle = 0;
float travelDistance = 0;

void loop(){
  position2(); //checks position and revolution of second motor, do NOT DELETE AT ALL

  //Serial.println(String(binCoord[0]) + " " + String(binCoord[1]));
  //Serial.println(String(phoneCoord[0]) + " " + String(phoneCoord[1]));
  
  //Serial.println(String(angle1) + " " + String(rev1));
  //Serial.println(String(angle2) + " " + String(rev2));

  /*
  if(recieved calling signal && isGoing == 0){ //checks if bin is running or not
    isGoing = 1; //sets bin to running
    resetRevs();
    binAngle = 0.0;
  }*/
  // Check if a signal is received from the IR remote
  if (IrReceiver.decode()) {
    Serial.print("IR Code Received: ");
    Serial.println(IrReceiver.decodedIRData.command); // Print the received IR code

    if (IrReceiver.decodedIRData.command == 69) {
      isGoing = 1;
    }
    IrReceiver.resume();  // Receive the next value
  }

  if(isGoing == 1){
    if(getCoords){  //getting coords
      triangulate(0, 8, 8.94, binCoord); //replace with bin position
      triangulate(11.31, 8, 4, phoneCoord); //replace with phone position
      
      turnToAngle = binAngle(binCoord, phoneCoord);
      
      turning = true;
      getCoords = false;
    }else if(turning){ //turning to the right angle
      binToAngle(turnToAngle - trashCanAngle);
    }else if(angleAdjust){
      trashCanAngle = turnToAngle;
      
      setMotor(0, 1);
      setMotor(0, 2);

      triangulate(0, 8, 8.94, binCoord); //replace with bin position
      triangulate(11.31, 8, 4, phoneCoord); //replace with phone position
      
      travelDistance = 20.00;

      delay(1000);

      goFwd = true;
      resetRevs();
      angleAdjust = false;
    }else if(goFwd){ //traveling distance to person
      
      
      moveFwd(travelDistance); //if this is too inaccurate we can repeat the process twice

      if(rev1 >= int(travelDistance/REV_TO_CM) && rev2 >= int(travelDistance/REV_TO_CM)){
        setMotor(0, 1);
        setMotor(0, 2);
        
        Serial.println(travelDistance);

        binCoord[0] = phoneCoord[0];
        binCoord[1] = phoneCoord[1];

        goFwd = false;
        //leave = true;

        //delete everything below once signal code is ready
        resetRevs();
        delay(1000);
        // Turn the servo motor to 180 degrees
        myServo.write(-180);

        // Wait for 5 seconds
        delay(5000);

        // Return the servo motor to the initial position
        myServo.write(180);
        delay(1000);
        isGoing = -1;
        getCoords = true;
      }
    }/*else if(leave){
      if(recieved signal to go back){ //this is commented out bc it needs
        isGoing = -1;                 //the signal to go back
        resetRevs();
        binAngle = 0.0;
        getCoords = true;
        leave = false;
      }
    }*/
  }

  
  if(isGoing == -1){
    if(getCoords){  //getting coords
      triangulate(11.31, 8, 4, binCoord); //replace with bin position

      turnToAngle = binAngle(binCoord, homeCoord);
      resetRevs();

      turning = true;
      getCoords = false;
    }else if(turning){ //turning to the right angle
      binToAngle(180);
    }else if(angleAdjust){
      trashCanAngle = turnToAngle;
      
      setMotor(0, 1);
      setMotor(0, 2);

      triangulate(11.31, 8, 4, binCoord); //replace with bin position
      
      travelDistance = binDistance(binCoord, homeCoord);
      Serial.println(travelDistance);

      delay(1000);

      goFwd = true;
      resetRevs();
      angleAdjust = false;
    }else if(goFwd){ //traveling distance to person
      moveFwd(travelDistance); //if this is too inaccurate we can repeat the process twice

      if(rev1 == int(travelDistance/REV_TO_CM) && rev2 == int(travelDistance/REV_TO_CM)){
        setMotor(0, 1);
        setMotor(0, 2);
        goFwd = false;
        isGoing = 0;
      }
    }
  }
}