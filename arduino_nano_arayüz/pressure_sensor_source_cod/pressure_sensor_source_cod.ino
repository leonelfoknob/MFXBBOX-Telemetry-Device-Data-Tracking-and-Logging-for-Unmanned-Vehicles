#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;
double Pressure,Altitude;

void setup() {
  Serial.begin(115200);
  if (!bmp.begin()) {
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  while (1) {}
  }
}
  
void loop() {
  print_bmp180_data();
}

void bmp180_data(){
  Pressure = bmp.readSealevelPressure();
  Altitude = bmp.readAltitude(101500);
}

void print_bmp180_data(){
    bmp180_data();
    Serial.print("Pressure at sealevel (calculated) = ");
    Serial.print(Pressure);
    Serial.print(" Pa");
    Serial.print(" | ");

  // you can get a more precise measurement of altitude
  // if you know the current sea level pressure which will
  // vary with weather and such. If it is 1015 millibars
  // that is equal to 101500 Pascals.
    Serial.print("Real altitude = ");
    Serial.print(Altitude);
    Serial.println(" meters");
}
