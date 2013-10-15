
/*
   i2c Helper
   Patrick Schubert
   Created March 30, 2013
   
   Currently implementing just initialization and single byte read/writes.
   Nice things to have in the future:
   - Interrupt-driven
   - Circular buffers
   - Ice cream maker

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
