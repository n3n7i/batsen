
#include <Arduino.h>

float readval =0;
const float voltage_divide = 10.0;

const float voltage_ref = 1.1;

void setup() {
  
  ADCSRA = 0x03;                   // DISABLE ADC , PRESCALE = /8
  ADMUX  = (ADMUX & 0x0F) | 0x80;  // 1.1V REFERENCE
  ADCSRA |= 0x80;                  // ENABLE
  ADMUX  = (ADMUX & 0xF0) | 0x01;  // PORT-B2 READ-SELECT 
  
  DDRB = 0x03;                     // PB 0,1,!5 OUTPUT MODE
  PORTB = 0x23;                     // PB 0,1,5 OUTPUT HIGH (pullup p5)

  delay(10);

  ADCSRA |= 0x40;                  // ADC READ
  while(ADCSRA & 0x40);            // wait  
  readval = (ADCH << 8) + ADCL;    // result
  
  }



void loop() {

  float voltread = (readval / 1023.0) * voltage_divide * voltage_ref;

  float k = 0;

  while ((k+1.0) < voltread){  // long blink off per volt
    PORTB ^= 0x01; //off 
    delay(750);
    PORTB ^= 0x01; //on
    delay(500);
    k += 1.0;
    }

  while ((k+0.2) < voltread){  //short blink off per 0.2 volt
    PORTB ^= 0x01; //off
    delay(250);
    PORTB ^= 0x01; //on
    delay(250);
    k += 0.2;
    }

  delay(2500);


  ADCSRA |= 0x40;                  // ADC READ
  while(ADCSRA & 0x40);            // wait  
  readval = (ADCH << 8) + ADCL;    // result
    
  }
  



