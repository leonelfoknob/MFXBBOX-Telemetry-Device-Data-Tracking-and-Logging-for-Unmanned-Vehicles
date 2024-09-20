/*  
  Optical Sensor Two Motor Demonstration
  DualMotorSpeedDemo.ino
  Demonstrates use of Hardware Interrupts
  to measure speed from two motors
  
  DroneBot Workshop 2017
  http://dronebotworkshop.com
*/
 
// Include the TimerOne Library from Paul Stoffregen
#include "TimerOne.h"
 
// Constants for Interrupt Pins
// Change values if not using Arduino Uno
 
const byte MOTOR1 = 2;  // Motor 1 Interrupt Pin - INT 0
const byte MOTOR2 = 3;  // Motor 2 Interrupt Pin - INT 1

double total_distance;
// Integers for pulse counters
unsigned int counter1 = 0;
unsigned int counter2 = 0;
 
// Float for number of slots in encoder disk
float diskslots = 30;  // Change to match value of encoder disk
float wheel_diameter = 0.25 ;//in meter
 
// Interrupt Service Routines
 
// Motor 1 pulse count ISR
void ISR_count1()  {
  counter1++;  // increment Motor 1 counter value
} 
 
// Motor 2 pulse count ISR
void ISR_count2()  {
  counter2++;  // increment Motor 2 counter value
} 
 
// TimerOne ISR
void ISR_timerone()
{
  Timer1.detachInterrupt();  // Stop the timer
  //distance parcoured in meter 
  float distance1 = (counter1 * wheel_diameter ) / diskslots;
  //float distance1 = (counter1 * wheel_diameter*3.14 ) / diskslots;//real calcul
  float distance2 = (counter2 * wheel_diameter ) / diskslots;
  //float distance2 = (counter2 * wheel_diameter*3.14 ) / diskslots;//real_calcul
//to calculate the distance with encoder i do whell_circonference(wheel_iameter*pi)* encoder_stick and divide it by encoder_stick_total
// of wheel le nombre de stick total qui est obtenu pour parcourir la circonference de la roue dans notre cas c'est 30 ce qui correspond 
// avec le nombre de troue sur le petit encoder attacher a la roue
  Serial.print("distance1: ");Serial.print(distance1);
  Serial.print("  distance2: ");Serial.print(distance2);

  total_distance = (distance1 + distance2)/2;
  Serial.print("  total distance parcoured :  ");Serial.println(total_distance);
  

  /*Serial.print("Motor Speed 1: ");
   
  float rotation1 = (counter1 / diskslots) * 60.00;  // calculate RPM for Motor 1
  float Speed_1 = rotation1*9.8425*3.14*60/63360;
  Serial.print(Speed_1);  
  Serial.print(" m/s");
  //Serial.print(rotation1);  
  //Serial.print(" RPM - ");
  counter1 = 0;  //  reset counter to zero
  Serial.print("Motor Speed 2: "); 
  float rotation2 = (counter2 / diskslots) * 60.00;  // calculate RPM for Motor 2
  float Speed_2 = rotation2*9.8425*3.14*60/63360;
  Serial.print(Speed_2);
  Serial.println(" m/s"); 
  //Serial.print(rotation2);  
  //Serial.println(" RPM"); 
  counter2 = 0;  //  reset counter to zero*/
  Timer1.attachInterrupt( ISR_timerone );  // Enable the timer
}
 
void setup() 
{
  Serial.begin(9600);
  
  //Timer1.initialize(1000000); // set timer for 1sec
  Timer1.initialize(10); // set timer for 1sec
  attachInterrupt(digitalPinToInterrupt (MOTOR1), ISR_count1, RISING);  // Increase counter 1 when speed sensor pin goes High
  attachInterrupt(digitalPinToInterrupt (MOTOR2), ISR_count2, RISING);  // Increase counter 2 when speed sensor pin goes High
  Timer1.attachInterrupt( ISR_timerone ); // Enable the timer
} 
 
void loop()
{
  Serial.print("  total distance parcoured1 :  ");Serial.println(total_distance);
  // Nothing in the loop!
  // You can place code here
}

// https://dronebotworkshop.com/robot-car-with-speed-sensors/
