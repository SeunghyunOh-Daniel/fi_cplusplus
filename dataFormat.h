#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <stdio.h>
#include <iostream>
#include <inttypes.h>

int makeserialf(int number);
/*-------------------------------------------------------------------------------------
Name : 
    make mask for range of bit value

Descriptions:
    To make mask for range of bit value such as 0xff for 8bits 
	
	if number = 4, makeserialf(4) = 0b1111 = 0xf
	if number = 8, makeserailf(8) = ob11111111 = 0xff
	BUT, it's limited 32bit. So, it returns 0

Params:
    int

Returned value:
    int
    0 : excessed number
    value : correct mask value
-------------------------------------------------------------------------------------*/
int power(int base, int exponent);
int getm8b8(double x);
/*-------------------------------------------------------------------------------------
Name : 
    Convert decimal to the number had integer 8bit, fractional 8bit 

Descriptions:
    To get signed integer 8bits, fractional 8bits
    1. x = 0: 0
    2. x > 0: int(x * power(2, 8))
    3. x < 0: int(power(2, 16) + x * power(2, 8))
        - returning to 'int' is for round
    '''
    Ex1. 
        x = 34.123
        x = x * (2**8)
        x = round(x)

    Ex2.
        x = -2.12
        x = 2**8 - (2**8 * x)
        x = round(x)
    '''
Params:
    double

Returned value:
    int
-------------------------------------------------------------------------------------*/
int getm16b8(double x);
/*-------------------------------------------------------------------------------------
Name : 
    Convert decimal to the number had integer 16bit, fractional 8bit 

Descriptions:
    To get signed integer 16bits, fractional 8bits
    1. x = 0: 0
    2. x > 0: int(x * power(2, 8))
    3. x < 0: int(power(2, 24) + x * power(2, 8))
        - returning to 'int' is for round
    '''
    Ex1. 
        x = 34.123
        x = x * (2**8)
        x = round(x)

    Ex2.
        x = -2.12
        x = 2**24 - (2**8 * x)
        x = round(x)
    '''
Params:
    Double

Returned value:
    int
-------------------------------------------------------------------------------------*/
double translatem8b8(int x);
/*-------------------------------------------------------------------------------------
Name : 
    Convert the number had integer 8bit, fractional 8bit to decimal  

Descriptions:
    To covert signed integer bits, fractional bits
    
    0. signed mask: x & 0b1000000000000000(0x8000)
    1. x = 0: 0
    2. x > 0: (x >> 8) & 0xff + (x & 0xff) / (2**8 - 1)
    3. x < 0: -1*((2**8) - ((x >> 8) & 0xff)) + (x & 0xff) / (2**8 - 1)

    '''
    Ex1. 
    	translatem8b8( x, 8, 8)
        x = 34.123
        x = (x >> 8) & (0xff) + (x & (0xff))/(2**8 - 1)
        x = trunc(x)

    Ex2.
    	translatem8b8( x, 8, 8)
        x = -2.12
        x = -1*((2**8) - ((x >> 8) & (0xff)))  + (x & (0xff))/(2**8 - 1)
        x = trunc(x)
    '''
Params:
    double

Returned value:
    int
-------------------------------------------------------------------------------------*/
double translatem16b8(int x);
/*-------------------------------------------------------------------------------------
Name : 
    Convert the number had integer 8bit, fractional 8bit to decimal  

Descriptions:
    To covert signed integer bits, fractional bits
    
    0. signed mask: x & 0xx800000
    1. x = 0: 0
    2. x > 0: (x >> 8) & 0xffff + (x & 0xff) / (2**8 - 1)
    3. x < 0: -1*((2**16) - ((x >> 8) & 0xffff)) + (x & 0xff) / (2**8 - 1)

    '''
    Ex1. 
    	translatem8b8( x, 16, 8)
        x = 34.123
        x = (x >> 8) & (0xffff) + (x & (0xff))/(2**8 - 1)
        x = trunc(x)

    Ex2.
    	translatem8b8( x, 16, 8)
        x = -2.12
        x = -1*((2**16) - ((x >> 8) & (0xffff)))  + (x & (0xff))/(2**8 - 1)
        x = trunc(x)
    '''
Params:
    double

Returned value:
    int
-------------------------------------------------------------------------------------*/
void printBinary(int input);
/*-------------------------------------------------------------------------------------
Name : 
    Print 32bit binary 

Descriptions:
	Print integer value to 32bit binary 

Params:
	input: int
Return:
	void
-------------------------------------------------------------------------------------*/
int fi(double number, int integer, int fractional);
/*-------------------------------------------------------------------------------------
Name : 
    Convert decimal to integer, fractional part

Descriptions:
    To get signed integer bits, fractional bits
    1. x = 0: 0
    2. x > 0: int(x * (2**fractional))
    3. x < 0: int(x * (2**(integer + fractional)) + x * (2**fractional))
        - returning to 'int' is for trunc
    '''
    Ex1. 
    	fi( x, 8, 8)
        x = 34.123
        x = x * (2**8)
        x = trunc(x)

    Ex2.
    	fi( x, 16, 8)
        x = -2.12
        x = 2**24 - (2**8 * x)
        x = trunc(x)
    '''
Params:
    double
    int
    int

Returned value:
    int
-------------------------------------------------------------------------------------*/
double translatefi(int x, int integer, int fractional);
/*-------------------------------------------------------------------------------------
Name : 
    Convert integer, fractional part to decimal

Descriptions:
    To covert signed integer bits, fractional bits

    0. signed mask: x & (1 << (integer + fractional -1)) ? 1 : 0
    1. x = 0: 0
    2. x > 0: (x >> fracation) & makeserialf(integer) + (x & makeserialf(fractional)) / (2**fractional - 1)
    3. x < 0: -1*((2**integer) - ((x >> fracation) & makeserialf(integer))) + x & makeserialf(fractional) / (2**fractional - 1)

    '''
    Ex1. 
    	translatefi( x, 8, 8)
        x = 34.123
        x = (x >> 8) & (0xff) + (x & (0xff))/(2**8 - 1)
        x = trunc(x)

    Ex2.
    	translate( x, 16, 8)
        x = -2.12
        x = -1*((2**16) - ((x >> 8) & (0xffff)))  + (x & (0xff))/(2**8 - 1)
        x = trunc(x)
    '''
Params:
    double

Returned value:
    int
-------------------------------------------------------------------------------------*/
#endif /* dataFormat_h */