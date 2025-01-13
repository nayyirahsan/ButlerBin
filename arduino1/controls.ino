void moveFwd(float distance){ //change to float
  int revs = distance/REV_TO_CM;

  turnRevolution(1, revs);
  turnRevolution(2, revs);
}

void turnRevolution(int motor, int revs){
  if(revs > 0){
    if(motor == 1){
      if(rev1 < revs){
        setMotor(1, motor);
      }else{
        setMotor(0, motor);
      }
    }else if(motor == 2){
      if(rev2 < revs){
        setMotor(1, motor);
      }else{
        setMotor(0, motor);
      }
    }
  }else if(revs < 0){
    if(motor == 1){
      if(rev1 > revs){
        setMotor(-1, motor);
      }else{
        setMotor(0, motor);
      }
    }else if(motor == 2){
      if(rev2 > revs){
        setMotor(-1, motor);
      }else{
        setMotor(0, motor);
      }
    }
  }
}

void turnAngle(int motor, float angle){
  if(angle < 0){
    if(motor == 1){
      if(angle1 > angle){
        setMotor(-1, motor);
      }else{
        setMotor(0, motor);
      }
    }else if(motor == 2){
      if(angle2 > angle){
        setMotor(-1, motor);
      }else{
        setMotor(0, motor);
      }
    }
  }else{
    if(motor == 1){
      if(angle1 < angle){
        setMotor(1, motor);
      }else{
        setMotor(0, motor);
      }
    }else if(motor == 2){
      if(angle2 < angle){
        setMotor(1, motor);
      }else{
        setMotor(0, motor);
      }
    }
  }
}

void setMotor(int dir, int motor){
  int p1, p2;
  
  if(motor == 1){
    p1 = motor1pin1;
    p2 = motor1pin2;
  }else{
    p1 = motor2pin1;
    p2 = motor2pin2;
  }
  
  if(dir>0){
      digitalWrite(p1, HIGH);
      digitalWrite(p2, LOW);
  }else if(dir<0){
      digitalWrite(p1, LOW);
      digitalWrite(p2, HIGH);
  }else{
      digitalWrite(p1, LOW);
      digitalWrite(p2, LOW);
  }
}

void resetRevs(){
  rev1 = 0;
  pos1 = 0;
  Wire.beginTransmission(8);
  Wire.write(0x01); // Send the command byte
  Wire.endTransmission();
}

void binToAngle(float angle){
  int a = int(angle + 0.5);

  int revolutions = abs(a)/DEG_PER_REV;
  int partialAngle = abs(a)%DEG_PER_REV;

  float wheelAngle = (2.86*partialAngle) - 0.0131*pow(partialAngle, 2) + 0.00259*pow(partialAngle, 3) - 0.0000276*pow(partialAngle, 4); 
  //i havent tested this equation yet

  if(a == 0){
    return;
  }

  if(a > 0){
    if(rev1 < revolutions){
      setMotor(1, 1);
    }else{
      if(angle1 < wheelAngle){
        setMotor(1, 1);
      }else{
        setMotor(0, 1);

      }
    }

    if(rev2 > -revolutions){
      setMotor(-1, 2);
    }else{
      if(angle2 > -wheelAngle){
        setMotor(-1, 2);
      }else{
        setMotor(0, 2);
      }
    }

  }else if(a < 0){
    if(rev2 < revolutions){
      setMotor(1, 2);
    }else{
      if(angle2 < wheelAngle){
        setMotor(1, 2);
      }else{
        setMotor(0, 2);
      }
    }

    if(rev1 > -revolutions){
      setMotor(-1, 1);
    }else{
      if(angle1 > -wheelAngle){
        setMotor(-1, 1);
      }else{
        setMotor(0, 1);
      }
    }
  }

  if(abs(rev1) >= revolutions && abs(rev2) >= revolutions && abs(angle1) >= wheelAngle && abs(angle2) >= wheelAngle){
    turning = false;
    angleAdjust = true;
  }
}