/*
i2cDemo
Patrick Schubert
Started March 30, 2013

*/

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"


//function prototypes
void setupUC(void);


int main(void)
{
    
    setupUC();
    setupI2C();
        
    for(;;){
        ;
    }
    
    return 0;
}

//initializes uC. 
void setupUC(void){
    //Setup the rtc
  cli();        
  TCCR2B |= 1<<CS10; //| 1<<CS11;    //Divide by 64
  OCR2A = 250;        //Count 15624 cycles for 1 second interrupt
  TCCR2B |= 1<<WGM12;        //Put Timer/Counter1 in CTC mode
  //TIMSK2 |= 1<<OCIE2A;        //enable timer compare interrupt
  sei();            
  
  //debug pin D0 (pin 2)
  DDRD |= (1<<1)|(1<<0);    //set as output    
  PORTD |= (1<<1)|(1<<0);        //set hi
  PORTD ^= (1<<1);
  
  //input pins
  DDRC = 0; //set portC as input
  PORTC = 0x00;
}


