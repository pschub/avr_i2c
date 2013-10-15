
/*
   i2c Helper
   Created March 30, 2013
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

#include "i2c.h"
#include <avr/io.h>
#include <avr/interrupt.h>

//calculates the correct baud generator setting
#define BAUD_REGISTER_SETTING (((F_CPU)/(F_I2C) - 16)/(2*PS_I2C))


#define i2c_clearInt {TWCR |= 0x80;}
#define i2c_waitForInt {while(!(TWCR & 0x80));}
#define i2c_setAck {TWCR |=0x40;}
#define i2c_setNack {TWCR &= 0xBF;}

#define STATUS_MASK (0xF8)
#define STATUS_SLAW_ACKED (0x18) //sla sent + slave acked (uC is MT)
#define STATUS_SLAR_ACKED (0x40) //sla sent + slave acked (uC is MR)
#define STATUS_DATA_ACKED (0x28) //slave acked data (uC is MT)
#define STATUS_STARTED (0x8)
#define STATUS_RSTARTED (0x10)
#define STATUS_ARBITRATION_LOST (0x38)

#define START_BIT (0x20)
#define STOP_BIT (0x10)

#define ADX_MASK (0x1)

#define WRITE (0x0)
#define READ (0x1)

char sendSla(char adx, char write);

void setupI2C(void){
   //uses portc5 (scl) and portc4 (sda)
   DDRC |= (1<<5) | (1<<4);
   PORTC |= (1<<5) | (1<<4);       //set pins
   TWBR = BAUD_REGISTER_SETTING;   //set baud generator
   TWSR = (TWSR & PS_I2C_REGISTER_MASK) | PS_I2C_REGISTER_SETTING;
   TWCR = 0;
   //TWCR |= (1<<TWIE) | (1<<TWEN);  //enable i2c and interrupt
}

char writeByte(char adx, char byte2Send){
    char ret;
    ret = sendSla(adx, WRITE);
    if (ret){
        return ret;
    }
    TWDR = byte2Send;
    i2c_clearInt;
    i2c_waitForInt;
    if ((TWSR & STATUS_MASK) !=  STATUS_DATA_ACKED){ //check for ack)
        return 0x3; //not acked
    }
    TWCR |= STOP_BIT; //send stop
    i2c_clearInt;
    return 0;        
}

char readByte(char adx, char* byte2Get){
    char ret;
    i2c_setNack;
    ret = sendSla(adx, READ);
    if (ret){
        return ret;
    }
    i2c_clearInt;
    i2c_waitForInt;
    *byte2Get = TWDR;
    TWCR |= STOP_BIT;
    return 0;
}

char sendSla(char adx, char not_write){
    TWCR |= (1<<TWEN);
    TWCR &= 0xFE; //disable i2c interrupt
    TWCR |= START_BIT;// send start 
    i2c_clearInt;
    i2c_waitForInt;
    if (!(TWSR & STATUS_STARTED)){ //check for start bit sent
        return 0x1;
    }
    TWDR = (adx) | (ADX_MASK & not_write);  //load adx+w
    TWCR &= ~START_BIT; //clear start
    i2c_clearInt;
    i2c_waitForInt;//wait for ack
    if (not_write){
        return ((TWSR & STATUS_MASK) == STATUS_SLAR_ACKED) ? 0 : 0x2;
    }
    else{
        return ((TWSR & STATUS_MASK) == STATUS_SLAW_ACKED) ? 0 : 0x3;
    }
}
