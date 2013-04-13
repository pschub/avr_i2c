
/*
   i2c Helper
   Patrick Schubert
   Created March 30, 2013

   Currently implementing just initialization and single byte read/writes.
   Nice things to have in the future:
   - Interrupt-driven
   - Circular buffers
   - Ice cream maker
*/

#ifndef I2CHELPER_H
#define I2CHELPER_H

#ifndef F_CPU
#define F_CPU 8000000
#endif

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

void setupI2C(void);
char writeByte(char adx, char byte2Send);
char readByte(char adx, char* byte2Get);


#endif //I2CHELPER_H
