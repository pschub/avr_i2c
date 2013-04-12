/*
i2cDemo
Patrick Schubert
Started March 30, 2013

*/

#define F_CPU 8000000


#include <avr/io.h>
#include <avr/interrupt.h>
#include "i2c.h"

#define EEPROM_ADX 0x1
#define SENS_ADX 0xA
#define OFFSET_ADX 0xC
//RH = (offset-SoH)*sens/2^12
// where SoH is the measured frequency

//function prototypes
void setupUC(void);


int main(void)
{
   char i2cReturn; 
   char deviceAdx = 0xA2;
   volatile int i,j;
    setupUC();
    setupI2C();
        
    for(;;){
        PORTD ^= 0x1;
        if (!writeByte(deviceAdx,0xA)){
          for (i=0; i<25;i++){
            for (j=0; j<10; j++){

            }
           }
           readByte(deviceAdx, &i2cReturn);
        }
        for (i=0; i<25;i++){
            for (j=0; j<10; j++){

            }
        }
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
  
  //input pins
  DDRC = 0; //set portC as input
  PORTC = 0x00;
}


