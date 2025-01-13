#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

static BLEUUID serviceUUID("4fafc201-1fb5-459e-8fcc-c5c9c331914b");

int rssiVal1 = 0;
int scanTime = 5;  //In seconds
BLEScan *pBLEScan;

class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());

    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {
        rssiVal1 = advertisedDevice.getRSSI();
    } 
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Scanning...");

  BLEDevice::init("");
  pBLEScan = BLEDevice::getScan();  //create new scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);  //active scan uses more power, but get results faster
  pBLEScan->setInterval(100);
  pBLEScan->setWindow(99);  // less or equal setInterval value
}

void loop() {
  // put your main code here, to run repeatedly:
  BLEScanResults *foundDevices = pBLEScan->start(scanTime, false);
  Serial.print("Devices found: ");
  Serial.println(foundDevices->getCount());
  Serial.println("Scan done!");
  pBLEScan->clearResults();  // delete results fromBLEScan buffer to release memory
  Serial.print("The RSSI Value of the Target Server:");
  Serial.println(rssiVal1);
  delay(2000);
}








Distance calculation formula:

#define PI 3.1415926535897932384626433832795
float roomW = 20.0;
float roomL = 10.0;
float x1 = 0, y1 = 0;
float x2 = 0, y2 = roomL;
float x3 = (roomW/2), y3 = roomL;
int coord[2];
void setup() {
  // put your setup code here, to run once:
  triangulate(7, 21.19, 10.44);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(coord[0]);
  Serial.print(" ");
  Serial.println(coord[1]);
  delay(1000);
}
void triangulate(float d1, float d2, float d3){
  float x = (pow(d1, 2.0) - pow(d2, 2.0) + pow(roomW, 2.0))/(2*roomW);
 
  float y = sqrt(pow(d3, 2.0) - pow(abs(x - x3), 2.0));
  y = roomL - y;
  coord[0] = x;
  coord[1] = y;
}
float distance(int pt1[2], int pt2[2]){
  return sqrt(pow((pt1[0]-pt2[0]), 2.0) + pow((pt1[0]-pt2[0]), 2.0));
}
float angle(int pt1[2], int pt2[2]){
  a = tan(abs((pt1[0]-pt2[0])/(pt1[1]-pt2[1])));
  a *= 180/PI;
  if(pt1[1]>pt2[1]){
    a += 90;
  }
    if(pt1[0]>pt2[0]){
    a *= -1;
  }
}
