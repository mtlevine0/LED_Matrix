//**************************************************************
//  Name    : 16x16 LED Display Test                                
//  Author  : Matt Levine
//  Date    : 27 Nov, 2013    
//  Version : 1.0                                             
//  Notes   :                          
//****************************************************************

#include <avr/pgmspace.h>
#include "font.h" // ascii offset of 32

int outputEn = 11;
int latchPin = 10;
int clockPin = 8;
int dataPinA = 9;
int dataPinB = 12;
int dataPinC = 7;
int dataPinD = 6;

uint8_t buffer[128];
String message = "#HIGHFEST brought to you by: 36 N. High St. #THEZOO               ";

void setup(void){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(outputEn, OUTPUT);
  pinMode(dataPinA, OUTPUT);
  pinMode(dataPinB, OUTPUT);
  pinMode(dataPinC, OUTPUT);
  pinMode(dataPinD, OUTPUT);
  analogWrite(outputEn, 250);
}

void loop(void){
	for(uint8_t letter = 0; letter < 95; letter++){// Scroll through entire font  
		for(uint8_t i = 0; i < fontInfo[letter][0]*2 + 1; i+=2){
      		if(fontInfo[letter][0]*2 == i){// Place one pixel spaces between each character
				buffer[127]=0x00;
				buffer[126]=0x00;
				buffer[125]=0x00;
				buffer[124]=0x00;
				for(uint8_t j = 0; j < 124; j+=4){
					buffer[j] = buffer[j+4];
				  	buffer[j+1] = buffer[j+5];
				  	buffer[j+2] = buffer[j+6];
				  	buffer[j+3] = buffer[j+7];
				}
			}else{    
				buffer[127] = pgm_read_byte_near(font + fontInfo[letter][1] + i + 3);
				buffer[126] = pgm_read_byte_near(font + fontInfo[letter][1] + i + 2);
				buffer[125] = pgm_read_byte_near(font + fontInfo[letter][1] + i + 1);
				buffer[124] = pgm_read_byte_near(font + fontInfo[letter][1] + i);
				for(uint8_t j = 0; j < 124; j+=4){// Scroll buffer one pixel at a time
					buffer[j] = buffer[j+4];
					buffer[j+1] = buffer[j+5];
					buffer[j+2] = buffer[j+6];
					buffer[j+3] = buffer[j+7];
				}
			}
			writeDisplay();// Write buffer to display
			delay(10);
		}
	}
} 

//
// User functions
//

void halt(void){
   while(true){} 
}

void writeDisplay(void){
  digitalWrite(latchPin, LOW); 
  for(uint8_t i = 0; i < 128; i+=4){//32
    shiftOut(dataPinA, dataPinB, dataPinC, dataPinD, clockPin, MSBFIRST, buffer[i], buffer[i+1], buffer[i+2], buffer[i+3]);
  }
  digitalWrite(latchPin, HIGH);
}

void shiftOut(uint8_t dataPinA, uint8_t dataPinB, uint8_t dataPinC, uint8_t dataPinD, uint8_t clockPin, uint8_t bitOrder, uint8_t valA, uint8_t valB, uint8_t valC, uint8_t valD){
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    if(bitOrder == LSBFIRST){
      digitalWrite(dataPinA, !!(valA & (1 << i)));
      digitalWrite(dataPinB, !!(valB & (1 << i)));
      digitalWrite(dataPinC, !!(valC & (1 << i)));
      digitalWrite(dataPinD, !!(valD & (1 << i)));

    }else{      
      digitalWrite(dataPinA, !!(valA & (1 << (7 - i))));
      digitalWrite(dataPinB, !!(valB & (1 << (7 - i))));
      digitalWrite(dataPinC, !!(valC & (1 << (7 - i))));
      digitalWrite(dataPinD, !!(valD & (1 << (7 - i))));

    }      
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);            
  }
}
