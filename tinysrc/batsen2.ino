#include <ATTinyCore.h>

//#include <Arduino.h>

#include <math.h>

float readval =0;
const float voltage_divide = 16.13;
const float voltage_ref = 1.1;

const float volt_minA = 0.8;
const float volt_maxA = 1.2;
const float volt_minB = 3.3;
const float volt_maxB = 3.7;

const float volt_overcharge = 0.2;

float volt_min = volt_minA + volt_minB;
float volt_max = volt_maxA + volt_maxB + volt_overcharge;

void setmux(int pinid){
  if(pinid == 2) ADMUX  = (ADMUX & 0xF0) | 0x01;
  if(pinid == 4) ADMUX  = (ADMUX & 0xF0) | 0x02;
  if(pinid == 3) ADMUX  = (ADMUX & 0xF0) | 0x03;
  if(pinid == 6) ADMUX  = (ADMUX & 0xF0) | 0x0F; //temp
  }

int readmux(){
  int readval = 0;
  ADCSRA |= 0x40;                  // ADC READ
  while(ADCSRA & 0x40);            // wait  
  readval = (ADCH << 8) + ADCL;    // result
  return(readval);
  }

void setpin_Out(int pinid){
  DDRB |= 0x01 << pinid;
  }

void setpin_High(int pinid){ 
  PORTB |= (0x01 << pinid);
  }
  
void setpin_Low(int pinid){
  PORTB &= ~(0x01 << pinid);
  }

void setup() {
  
  ADCSRA = 0x06;                   // DISABLE ADC , PRESCALE = /64
  ADMUX  = (ADMUX & 0x0F) | 0x80;  // 1.1V REFERENCE
  ADCSRA |= 0x80;                  // ENABLE
  ADMUX  = (ADMUX & 0xF0) | 0x01;  // PORT-B2 READ-SELECT 
  
  delay(200);

  ADCSRA |= 0x40;                  // ADC READ
  while(ADCSRA & 0x40);            // wait  
  readval = (ADCH << 8) + ADCL;    // result

  DDRB = 0x03;                     // PB 0,1,!5 OUTPUT MODE
  PORTB = 0x23;                     // PB 0,1,5 OUTPUT HIGH (pullup p5)
  
  delay(250);
  
  }


void statusPin(int pinid, float vread){

  float k = 0;
  
  byte tx = 0x01 << pinid;

  while ((k+1.0) < vread){  // long blink off per volt
    PORTB ^= tx; //off 
    delay(500);
    PORTB ^= tx; //on
    delay(250);
    k += 1.0;
    }
    
  delay(250);
  while ((k+0.2) < vread){  //short blink off per 0.2 volt
    PORTB ^= tx; //off
    delay(125);
    PORTB ^= tx; //on
    delay(125);
    k += 0.2;
    }
    
  delay(250);
  while ((k+0.05) < vread){  //short blink on per 0.05 volt
    PORTB ^= tx; //off
    delay(250);
    PORTB ^= tx; //on
    delay(50);
    k += 0.05;
    }

  PORTB ^= tx; //off
  delay(250);
  PORTB ^= tx; //on

  }



void loop() {

  float voltread = ((float)readval / 1023.0) * voltage_divide * voltage_ref;
     
  if(voltread >= volt_max){
    setpin_Low(1);    
    }
    
  statusPin(0, voltread);
   
  delay(1000);

 // ADCSRA |= 0x40;                  // ADC READ
 // while(ADCSRA & 0x40);            // wait  
 // readval = (ADCH << 8) + ADCL;    // result
 
 readval = readmux();
    
 }
