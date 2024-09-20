#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

double GPSLat,GPSLong,headingDegrees;

TinyGPSPlus gps;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  sensor_t sensor;
  mag.getSensor(&sensor);
}

void loop()
{
 
 displayCompassInfo();
  getGPSdata();
}

void getGPSdata(){
  if(Serial1.available() > 0){
    if(gps.encode(Serial1.read())){
      GPSLat = gps.location.lat();
      GPSLong = gps.location.lng();
      Serial.print("GPSLat ");Serial.print(GPSLat, 19);
      //Serial.print(F(","));
      Serial.print(" GPSLong ");Serial.print(GPSLong, 19);
    }
  }
}

void displayCompassInfo()
{
  mag.begin();
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = 0.05;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  headingDegrees = heading * 180/M_PI; 
  Serial.print(" Heading (degrees): "); Serial.println(headingDegrees,5);
}
