#include <AltSoftSerial.h>
AltSoftSerial altSerial;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  altSerial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  char c;
  if (Serial.available()) {
    c = Serial.read();
    altSerial.print(c);
  }
  if (altSerial.available()) {
    c = altSerial.read();
    Serial.print("Message from ESP32: ");
    Serial.println(c);
  }
}
