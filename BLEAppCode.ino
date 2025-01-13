Variables

//triangulation
#include <math.h>


float roomW = 20.0;
float roomL = 10.0;


float x1 = 0, y1 = 0;
float x2 = 0, y2 = roomL;
float x3 = (roomW/2), y3 = roomL;

Loop
bool getCoords = true;


int binCoord[2];
int phoneCoord[2];
float turnToAngle = 0;
float travelDistance = 0;


void loop() {
  //position2();


  if(getCoords){  //getting coords
    triangulate(1, 20.02, 13.45, binCoord); //replace with bot position
    triangulate(9, 21.93, 10.04, phoneCoord); //replace with phone position
    turnToAngle = binAngle(binCoord, phoneCoord);
    travelDistance = binDistance(binCoord, phoneCoord);
  }


  //Serial.println(String(binCoord[0]) + " " + String(binCoord[1]));
  //Serial.println(String(phoneCoord[0]) + " " + String(phoneCoord[1]));
  Serial.println(travelDistance);
  Serial.println(turnToAngle);
  delay(1000);


  //turnRevolution(1, 1);
  //turnRevolution(2, -1);


  //moveFwd(5);


  //Serial.print("left motor: " + String(angle1) + " " + String(rev1));
  //Serial.println(" right motor: " + String(angle2) + " " + String(rev2));
}


Triangulation methods

void triangulate(float d1, float d2, float d3, int coord[]){
  float x = (pow(d1, 2.0) - pow(d2, 2.0) + pow(roomW, 2.0))/(2*roomW);
 
  float y = sqrt(pow(d3, 2.0) - pow(x - x3, 2.0));
  y = roomL - y;


  coord[0] = (x+.5);
  coord[1] = (y+.5);
}


float binDistance(int pt1[], int pt2[]){
  return sqrt(pow((pt1[0]-pt2[0]), 2.0) + pow((pt1[1]-pt2[1]), 2.0));
}


float binAngle(int pt1[], int pt2[]){
 
  if(pt1[0] == pt2[0]){
    if(pt1[1] >= pt2[1]){
      return -90.0;
    }else{
      return 90.0;
    }
  }
 


  float a = atan(float(pt1[1]-pt2[1])/float(pt1[0]-pt2[0]));
 
  a *= 180/PI;


  if(pt1[0]>pt2[0]){
    if(pt1[1]<pt2[1]){
      a += 180;
    }else if(pt1[1]>=pt2[1]){
      a -= 180;
    }
  }


  return a;
}

