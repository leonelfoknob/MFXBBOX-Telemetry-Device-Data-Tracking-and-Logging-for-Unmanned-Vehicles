#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_MPU6050.h>

Adafruit_MPU6050 mpu;
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

void setup(void) 
{
  Serial.begin(9600);
  mag.begin();
  sensor_t sensor;
  mag.getSensor(&sensor);
  mpu.begin();
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop(void){
  /* Get a new sensor event */ 
  sensors_event_t event,a, g, temp; 
  mag.getEvent(&event);
  mpu.getEvent(&a, &g, &temp);
  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("orientation X: "); Serial.print(event.magnetic.x);
  Serial.print(", Y: "); Serial.print(event.magnetic.y);
  Serial.print(", Z: "); Serial.print(event.magnetic.z);
  Serial.println(" uT");

  Serial.print("Acceleration X: ");
  Serial.print(a.acceleration.x);
  Serial.print(", Y: ");
  Serial.print(a.acceleration.y);
  Serial.print(", Z: ");
  Serial.print(a.acceleration.z);
  Serial.println(" m/s^2");

  Serial.print("Rotation X: ");
  Serial.print(g.gyro.x);
  Serial.print(", Y: ");
  Serial.print(g.gyro.y);
  Serial.print(", Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" degC");
  delay(500);
}
