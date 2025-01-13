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