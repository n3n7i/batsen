

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
  
  
  
  
