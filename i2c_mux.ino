#include <Wire.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox1 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox2 = Adafruit_VL53L0X();
Adafruit_VL53L0X lox3 = Adafruit_VL53L0X();

int errorF=5;
int errorL=5;
int errorR=20;

int readingF;
int readingL;
int readingR;

// Select I2C BUS
void TCA9548A(uint8_t bus){
  Wire.beginTransmission(0x70);  // TCA9548A address
  Wire.write(1 << bus);          // send byte to select bus
  Wire.endTransmission();
}


void getTOFValues(Adafruit_VL53L0X lox, int bus,int tofErr) {
  TCA9548A (bus);
  Serial.print("Sensor number on bus");
  Serial.println(bus);

  VL53L0X_RangingMeasurementData_t measure;
    
  Serial.print("Reading a measurement... ");
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if(bus==1){
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      readingF=measure.RangeMilliMeter-tofErr;
      Serial.print("Distance (mm): "); Serial.println(readingF);
    } else {
      Serial.println(" out of range ");
    }
  }

  if(bus==2){
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      readingL=measure.RangeMilliMeter-tofErr;
      Serial.print("Distance (mm): "); Serial.println(readingL);
    } else {
      Serial.println(" out of range ");
    }
  }

  if(bus==7){
    if (measure.RangeStatus != 4) {  // phase failures have incorrect data
      readingR=measure.RangeMilliMeter-tofErr;
      Serial.print("Distance (mm): "); Serial.println(readingR);
    } else {
      Serial.println(" out of range ");
    }
  }
}



void setup() {
  Serial.begin(115200);

  // wait until serial port opens for native USB devices
  while (! Serial) {
    Serial.println("serial wait");
    delay(1);
  }


  // Start I2C communication with the Multiplexer
  Wire.begin();

  // Init sensor on bus number 2
  TCA9548A(1);
  Serial.println("Adafruit VL53L0X test1");
  if (!lox1.begin()) {
    Serial.println(F("Failed to boot VL53L0X_1"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n1")); 
  
  // Init sensor on bus number 3
  TCA9548A(2);
  Serial.println("Adafruit VL53L0X test2");
  if (!lox2.begin()) {
    Serial.println(F("Failed to boot VL53L0X_2"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n2")); 
  

    // Init sensor on bus number 7
  TCA9548A(7);
  Serial.println("Adafruit VL53L0X test3");
  if (!lox3.begin()) {
    Serial.println(F("Failed to boot VL53L0X_3"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n3")); 
}

void loop() { 
  //Print values for sensor 1
  getTOFValues(lox1, 1,errorF);
  getTOFValues(lox2, 2,errorL);
  getTOFValues(lox3, 7,errorR);
  delay(1000);
}