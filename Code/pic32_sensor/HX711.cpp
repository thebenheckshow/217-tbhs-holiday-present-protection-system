#include <Arduino.h>
#include <HX711.h>

HX711::HX711(byte dout, byte pd_sck, byte gain) {
	PD_SCK 	= pd_sck;
	DOUT 	= dout;
	
	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, INPUT);

	set_gain(gain);
}

HX711::~HX711() {

}

bool HX711::is_ready() {
	return digitalRead(DOUT) == LOW;
}

void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}

	digitalWrite(PD_SCK, LOW);
	read();
}

long HX711::read() {
	// wait for the chip to become ready
	while (!is_ready());

	byte data[3];

	// pulse the clock pin 24 times to read the data
	for (byte j = 3; j--;) {
		for (char i = 8; i--;) {
			digitalWrite(PD_SCK, HIGH);
			bitWrite(data[j], i, digitalRead(DOUT));
			digitalWrite(PD_SCK, LOW);
		}
	}

	// set the channel and the gain factor for the next reading using the clock pin
	for (int i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		digitalWrite(PD_SCK, LOW);
	}

	data[2] ^= 0x80;

	return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8) | (uint32_t) data[0];
}

long HX711::read_average(byte times) {
	long sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += read();
	}
	return sum / times;
}

double HX711::get_value(byte times) {

	if (GAIN == 3) {
		return read_average(times) - OFFSETA;	
	}
	else {
		return read_average(times) - OFFSETB;			
	}
	
}

float HX711::get_units(byte times) {
	
	if (GAIN == 3) {
		
		return (get_value(times) / SCALEA);
		
	}
	else {
		
		return (get_value(times) / SCALEB);
		
	}
	
	
	//return get_value(times) / SCALE;
}

void HX711::tare(byte times) {
	
	double sum = read_average(times);
	
	if (GAIN == 3) {
		OFFSETA = sum;		
	}
	else {
		OFFSETB = sum;				
	}
		
}

void HX711::set_scaleA(float scale) {
	SCALEA = scale;
}

void HX711::set_scaleB(float scale) {
	SCALEB = scale;
}

void HX711::set_offset(long offset, byte whichOffset) {
	
	if (whichOffset) {
		OFFSETB = offset;	
	} 
	else {
		OFFSETA = offset;		
	}
		
}

void HX711::power_down() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);	
}

void HX711::power_up() {
	digitalWrite(PD_SCK, LOW);	
}