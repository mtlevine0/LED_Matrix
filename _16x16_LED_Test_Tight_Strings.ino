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

uint8_t buffer[128];
String message = "#HIGHFEST brought to you by: 36 N. High St. #THEZOO               ";

void setup(void){
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(outputEn, OUTPUT);
  pinMode(dataPinA, OUTPUT);
  pinMode(dataPinB, OUTPUT);
  analogWrite(outputEn, 250);
  Serial.begin(9600);
}

void loop(void){
  int length = message.length();
  for(uint8_t letter = 0; letter < length; letter++){// Scroll through entire font  
    for(uint8_t i = 0; i < fontInfo[message[letter]-32][0]*2 + 1; i+=2){
      if(fontInfo[message[letter]-32][0]*2 == i){// Place one pixel spaces between each character
        buffer[127]=0x00;
        buffer[126]=0x00;
        for(uint8_t j = 0; j < 126; j+=2){
          buffer[j] = buffer[j+2];
          buffer[j+1] = buffer[j+3];
        }
      }else{    
        buffer[127] = pgm_read_byte_near(font + fontInfo[message[letter]-32][1] + i + 1);
        buffer[126] = pgm_read_byte_near(font + fontInfo[message[letter]-32][1] + i);
        for(uint8_t j = 0; j < 126; j+=2){// Scroll buffer one pixel at a time
          buffer[j] = buffer[j+2];
          buffer[j+1] = buffer[j+3];
        }
      }
      writeDisplay();// Write buffer to display
      //delay(10);
    }
  }
} 

//
// User functions
//

void allOn(void){
  
}

void halt(void){
   while(true){} 
}

void writeDisplay(void){
  //digitalWrite(latchPin, LOW);
  PORTB = PORTB | (1<<2);
  for(uint8_t i = 0; i < 128; i+=2){
    shiftOut(dataPinA, dataPinB, clockPin, MSBFIRST, buffer[i], buffer[i+1]);
  }
  digitalWrite(latchPin, HIGH);
}

void shiftOut(uint8_t dataPinA, uint8_t dataPinB, uint8_t clockPin, uint8_t bitOrder, uint8_t valA, uint8_t valB){
  uint8_t i;
  for (i = 0; i < 8; i++)  {
    if(bitOrder == LSBFIRST){
      digitalWrite(dataPinA, !!(valA & (1 << i)));
      digitalWrite(dataPinB, !!(valB & (1 << i)));
    }else{      
      digitalWrite(dataPinA, !!(valA & (1 << (7 - i))));
      digitalWrite(dataPinB, !!(valB & (1 << (7 - i))));
    }      
    digitalWrite(clockPin, HIGH);
    digitalWrite(clockPin, LOW);            
  }
}
