#include <Arduino.h>
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available()) {
    char data = Serial.read();
    Serial.write(data);
  }
  delay(500);
} 
