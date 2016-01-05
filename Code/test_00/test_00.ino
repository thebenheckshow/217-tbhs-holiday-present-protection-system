/**
 * Take measurement from weigh scales sensor and output to serial
 * port. By reversing polarity of excite signal can reduce common
 * mode noise. Take 256 samples and get mean before outputing 
 * result -- this helps reduce noise and adds extra resolution.
 * Joe Desbonnet, 28 Oct 2010. jdesbonnet@gmail.com. 
 */
 
 // Define bridge excite digital IO lines
#define D0 3
#define D1 2

// Use ADC port 0
#define AN0 0
 
// Delain in microseconds to allow signal to settle 
// after excite polarity reversal
#define DELAY 800

void setup()   {
  
  Serial.begin(115200); 
  
  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);
}

void loop()                     
{
  
  // Get zero point ('tare') of scale
  int tare = measure ();
  
  while (true) {
     Serial.println ( measure() - tare );
     //delay (20);
  }
  
}

int measure () {
  int i,a0,a1;
  long s;
  for (i = 0; i < 256; i++) {
    digitalWrite (D0,HIGH);
    digitalWrite (D1,LOW);
    delayMicroseconds(DELAY);
    a0 = analogRead(AN0);
     
    // reverse polarity
    digitalWrite (D0,LOW);
    digitalWrite (D1,HIGH);
    delayMicroseconds(DELAY);
    a1 = analogRead(AN0);
    
    s += (long) (a0 - a1);
      
   } // next i
   
   // Don't care about polarity
   if ( s < 0) {
       s = -s;
   }
   
   // As there is a large number of samples being averaged we
   // can probably extract another bit or two from the ADC.
   // Going to be conservative and going for one extra bit.
   // So instead of dividing by 256, will divide by just 128
   // instead (ie right shift 7 bits).
   
   return s>>7;
}

