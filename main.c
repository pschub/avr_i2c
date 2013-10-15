/*
i2cDemo
Started March 30, 2013

Copyright (C) 2013 Patrick Schubert 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#define F_CPU 1000000


#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "i2c.h"

#define EEPROM_ADX 0x1
#define SENS_ADX 0xA
#define OFFSET_ADX 0xC

//function prototypes
void setupUC(void);
int getAnInt(void);
void printNum(int num);


int main(void)
{
    char i2cReturn;
    char deviceAdx = 0xA2;
    unsigned int value;
    int i;
    setupUC();
    setupI2C();

    UBRR0 = 12;
    UCSR0B = 0x8; //enable TX
    UCSR0C = 0x26; //4800 8/1/even parity


    for(;;){
        PORTD ^= 0x1;
        while ( (~UCSR0A) & (1<<UDRE0));
        UDR0 = '?';
        _delay_ms(1000);
        writeByte(deviceAdx, 0xA);
        _delay_ms(5);
        for (i=0; i<2; i++){
            while ( (~UCSR0A) & (1<<UDRE0));
            UDR0 = ':';
            _delay_ms(500);
            readByte(deviceAdx, &i2cReturn);
            value = i2cReturn;
            value = value<<8;
            _delay_ms(5);
            readByte(deviceAdx, &i2cReturn);
            _delay_ms(500);
            value |= i2cReturn;
            printNum(value);
            _delay_ms(1000);
        }

    }

    return 0;
}


void printNum(int num){
    char digit = num/10000;
    while ( (~UCSR0A) & (1<<UDRE0));
    UDR0 = digit + 0x30;
    digit = (num%10000)/1000;
    while ( (~UCSR0A) & (1<<UDRE0));
    UDR0 = digit + 0x30;
    digit = (num%1000)/100;
    while ( (~UCSR0A) & (1<<UDRE0));
    UDR0 = digit + 0x30;
    digit = (num%100)/10;
    while ( (~UCSR0A) & (1<<UDRE0));
    UDR0 = digit + 0x30;
    digit = (num%10);
    while ( (~UCSR0A) & (1<<UDRE0));
    UDR0 = digit + 0x30;
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


