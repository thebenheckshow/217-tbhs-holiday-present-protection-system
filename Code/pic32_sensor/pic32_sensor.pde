
#include "HX711.h"

#define calibration_factorA -4          //This value is obtained using the SparkFun_HX711_Calibration sketch

#define calibration_factorB -2         //This value is obtained using the SparkFun_HX711_Calibration sketch


#define DOUT_00  5
#define CLK_00  6
#define DOUT_01  7
#define CLK_01  8

#define keySwitch	10
#define speaker		25

float tare0;
float tare1;
float tare2;

float sense0;
float sense1;
float sense2;

short noise = 2000;

HX711 scale_00(DOUT_00, CLK_00);
HX711 scale_01(DOUT_01, CLK_01);

void setup() {
	
	Serial.begin(9600);
	
	pinMode(keySwitch, INPUT);
	pinMode(speaker, OUTPUT);
	
	scale_00.set_gain(64);

	scale_00.tare();
	scale_00.tare();
	
	scale_00.set_gain(32);
	
	scale_00.tare();
	
	scale_00.set_scaleA(calibration_factorA); //This value is obtained by using the SparkFun_HX711_Calibration sketch
	scale_00.set_scaleB(calibration_factorB); //This value is obtained by using the SparkFun_HX711_Calibration sketch

	
	scale_01.set_gain(64);
	
	scale_01.tare();
	
	scale_01.set_scaleA(calibration_factorA); //This value is obtained by using the SparkFun_HX711_Calibration sketch
	scale_01.set_scaleB(calibration_factorB); //This value is obtained by using the SparkFun_HX711_Calibration sketch

	scale_00.set_gain(64);	
	tare0 = scale_00.get_units();

	scale_00.set_gain(32);
	tare1 = scale_00.get_units();

	scale_01.set_gain(64);
	tare2 = scale_01.get_units();

}

void loop() {

	//delay(10);


	
	scale_00.set_gain(64);
	sense0 = scale_00.get_units();
	
	scale_00.set_gain(32);
	sense1 = scale_00.get_units();
	
	scale_01.set_gain(64);
	sense2 = scale_01.get_units();	
	
	if (digitalRead(keySwitch)) {			//Key turned? Set current weights to tare

		noTone(speaker);
			
		noise = 3000;	
	
		Serial.print(sense0, 1); //scale.get_units() returns a float
		Serial.print("\t");
		Serial.print(sense1, 1); //scale.get_units() returns a float
		Serial.print("\t");	
		Serial.print(sense2, 1); //scale.get_units() returns a float
		Serial.print("\t");	
	
		Serial.print("TARE");
		
		tare0 = sense0;
		tare1 = sense1;
		tare2 = sense2;
		
		Serial.println();  		
		
	}
	else {	//If anything changes much past tare, sound alarm!
	
		if (sense0 < (tare0 - 50) or sense0 > (tare0 + 50) or sense1 < (tare1 - 50) or sense1 > (tare1 + 50) or sense2 < (tare2 - 50) or sense2 > (tare2 + 50)) {
			
			//Serial.println("ALARM!");
			
			tone(speaker, noise); 
			
			noise += 1000;
			
			if (noise > 3000) {
				
				noise = 2000;
				
			}		
			
		} 
		else {
		
			noTone(speaker);
			
			noise = 3000;
			
		}
		
	}

}
