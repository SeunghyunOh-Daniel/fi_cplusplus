#ifndef DATAFORMAT_H
#define DATAFORMAT_H

#include <stdio.h>
#include <iostream>
#include <inttypes.h>

int makeserialf(int number);
int power(int base, int exponent);
int getm8b8(double x);
int getm16b8(double x);
double translatem8b8(int x);
double translatem16b8(int x);
void printBinary(int input);
int fi(double number, int integer, int fractional);
double translatefi(int x, int integer, int fractional);
#endif /* dataFormat_h */