#include <Adafruit_BMP085.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
Adafruit_BMP085 bmp;

static const int RXPin = 10, TXPin = 11; // gps 
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin);


long temps;
float distance,headingDegrees;
double bat_volt,Altitude,Acc_X,Acc_Y,Acc_Z;
double Long = -1.0;
double Lat = -1.0;
double GpsSpeed = -1.0 ;
int Pressure;


void setup() {
  Serial.begin(115200);
  ss.begin(GPSBaud);
  bmp.begin();
  initialize_mpu();
  sensor_t sensor;
  mag.getSensor(&sensor);

 
  
  delay(1000);

}

void loop() {
  
  bat_level();
  bmp180_data();
  update_mpu_val();
  get_gps_data();
  GetCompassData();
  delay(100);
  SendDataToRaspberryPy();
  /*Serial.print("batt: ");Serial.print(bat_volt);Serial.print(" Pres: ");Serial.print(Pressure);Serial.print(" Alt: ");Serial.print(Altitude);
  Serial.print(" Acc_X: ");Serial.print(Acc_X);Serial.print(" Acc_Y: ");Serial.print(Acc_Y);Serial.print(" Acc_Z: ");Serial.print(Acc_Z);
  Serial.print(" Long: ");Serial.print(Long);Serial.print(" Lat: ");Serial.print(Lat);Serial.print(" Head: "); Serial.print(headingDegrees);Serial.print(" Spd: ");Serial.println(GpsSpeed);
*/
}

void bat_level(){
  bat_volt = analogRead(A0) * (4.853 / 992.9238) ;
  bat_volt = bat_volt*(8.4/4.853);
 }

void bmp180_data(){
  Pressure = bmp.readSealevelPressure();
  Altitude = bmp.readAltitude(101500);
}

void initialize_mpu(){
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ); 
}

void update_mpu_val(){
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  Acc_X = a.acceleration.x;
  Acc_Y = a.acceleration.y;
  Acc_Z = a.acceleration.z;
}

void get_gps_data(){
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      Lat = gps.location.lat();
      Long = gps.location.lng();
      GpsSpeed = gps.speed.kmph();
  }

void GetCompassData(){
  if(mag.begin()){
  sensors_event_t event; 
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  //eskişehir declinasion angle +6* 0' E, which is ~6 Degrees, or (which we need) 0.10 radians 
  float declinationAngle = 0.10;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  headingDegrees = heading * 180/M_PI;
  }
}
void SendDataToRaspberryPy(){
  Serial.print(bat_volt);Serial.print(",");
  Serial.print(Pressure);Serial.print(",");
  Serial.print(Altitude);Serial.print(",");
  Serial.print(Acc_X);Serial.print(",");
  Serial.print(Acc_Y);Serial.print(",");
  Serial.print(Acc_Z);Serial.print(",");
  Serial.print(Long);Serial.print(",");
  Serial.print(Lat);Serial.print(",");
  Serial.print(headingDegrees);Serial.print(",");
  Serial.print(GpsSpeed);Serial.println(",");
}
