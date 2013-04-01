
/*
   i2c Helper
   Patrick Schubert
   Created March 30, 2013
*/

/*
   PS_I2C is the prescaler value
   PS_I2C_REGISTER_SETTING is the register value to set the PS
   These must agree for correct operation:
       PS     REG
        1      0
        4      1
       16      2
       64      3
 */
#define PS_I2C 1
#define PS_I2C_REGISTER_SETTING 0
#define PS_I2C_REGISTER_MASK 0b11111100
/*
    F_I2C is the clock rate; should be <= 400kHz
 */
#define F_I2C 400000

//calculates the correct baud generator setting
#define BAUD_REGISTER_SETTING (((F_CPU)/(F_I2C) - 16)/(2*PS_I2C))

void setupI2C(void);


void setupI2C(void){
   //uses portc5 (scl) and portc4 (sda)
   DDRC |= (1<<5) | (1<<4);
   PORTC |= (1<<5) | (1<<4);       //set pins
   TWBR = BAUD_REGISTER_SETTING;   //set baud generator
   TWSR = (TWSR & PS_I2C_REGISTER_MASK) | PS_I2C_REGISTER_SETTING
   TWCR &= ~(1<<TWINT);            //clear flag
   TWCR |= (1<<TWIE) | (1<<TWEN);  //enable i2c and interrupt

}

