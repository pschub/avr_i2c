
/*
   i2c Helper
   Patrick Schubert
   Created March 30, 2013
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

#define START_BIT (1<<5)
#define STOP_BIT (1<<4)

#define ADX_MASK (0x1)
//#define ACK (0x40)
//#define NACK_MASK (0xBF)

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
