/*
 Example using the SparkFun HX711 breakout board with a scale
 By: Nathan Seidle
 SparkFun Electronics
 Date: November 19th, 2014
 License: This code is public domain but you buy me a beer if you use this and we meet someday (Beerware license).
 
 This example demonstrates basic scale output. See the calibration sketch to get the calibration_factor for your
 specific load cell setup.
 
 This example code uses bogde's excellent library: https://github.com/bogde/HX711
 bogde's library is released under a GNU GENERAL PUBLIC LICENSE
 
 The HX711 does one thing well: read load cells. The breakout board is compatible with any wheat-stone bridge
 based load cell which should allow a user to measure everything from a few grams to tens of tons.

 Arduino pin 2 -> HX711 CLK
 3 -> DAT
 5V -> VCC
 GND -> GND
 
 The HX711 board can be powered from 2.7V to 5V so the Arduino 5V power should be fine.
 test
*/

#include "HX711.h"

#define calibration_factor -100          //This value is obtained using the SparkFun_HX711_Calibration sketch

#define DOUT_00  3
#define CLK_00  2
#define DOUT_01  5
#define CLK_01  4

HX711 scale_00(DOUT_00, CLK_00);
HX711 scale_01(DOUT_01, CLK_01);

void setup() {
  Serial.begin(9600);
  Serial.println("HX711 scale demo");

  scale_00.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_00.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0

  scale_01.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale_01.tare();	//Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  
  scale_00.set_gain(128);
  Serial.print("Reading scale_00 Channel A: ");
  Serial.print(scale_00.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println();
  
/*   scale_00.set_gain(32);
  Serial.print("Reading scale_00 Channel B: ");
  Serial.print(scale_00.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println(); 
  
  scale_01.set_gain(128);
  Serial.print("Reading scale_01 Channel A: ");
  Serial.print(scale_01.get_units(), 1); //scale.get_units() returns a float
  Serial.print(" lbs"); //You can change this to kg but you'll need to refactor the calibration_factor
  Serial.println(); */
  
}
