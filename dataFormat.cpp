#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include "config.h"

void printBinary(int input)
{
    printf("Binary of %d: ", input);
    int mask;
    if (input == 0)
    {
        printf("0");
    }
    for (int i = 31; i >= 0; i--)
    {
        mask = 1 << i;
        printf("%d", input & mask ? 1 : 0);
        if (i % 8 == 0)printf(" ");
    }
    printf("\n");
}
int makeserialf(int number)
{
    if(number > 32)return 0;
    int finalNum = 0;
    int calNum = 1;
    for (int i = 0; i < number; i++)
    {
        finalNum = finalNum | calNum;
        calNum = calNum << 1;
    }

    return finalNum;
}
//double power(double base, double exponent)
//{
//    double x = 1.0;
//    for(int i=0; i<exponent; i++){
//        x = x*base;
//    }
//    return x;
//}

int power(int base, int exponent)
{
    int x = 1;
    for (int i = 0; i < exponent; i++) {
        if (x & (1 << 31))return -1;
        x = x * base;
    }
    return x;
}

int fi(double number, int integer, int fractional)
{

    int multi2Fractional = power(2, fractional);
    int multi2Number = power(2, integer + fractional);

#ifdef DEBUG_FI
    printf("-----------------------------------------------------\n");
    printf("number %f m%db%d\n", number, integer, fractional);   
    printf("2**(%d) = Fractional %d, 2**(%d+%d) = number %d, number * multi2Fractional = %f\n",fractional, multi2Fractional, integer, fractional, multi2Number,  number * multi2Fractional);
#endif
    
    if ((multi2Fractional == -1) || (multi2Number) == -1)return (power(2, integer - 1)-1);

    if (number > power(2, integer-1))
    {
        printf("ERROR;NUM, value %f < %d\n",number, power(2, integer - 1));
        return power(2, integer - 1);
    }
    else
    {
        if (number == 0)
        {
            return 0;
        }
        else if (number > 0)
        {

#ifdef DEBUG_FI
            printf("m%db%d + Change to: %f -> %d\n", integer, fractional, number, int(number * multi2Fractional));
            if(integer==8)printf("Original %d\n",getm8b8(number));
            if(integer==16)printf("Oirignal %d\n",getm16b8(number));
            printf("----------------------------------------------------\n");
#endif
            return int(number * multi2Fractional);
        }
        else
        {

#ifdef DEBUG_FI
            printf("m%db%d - Change to: %f -> %d\n", integer, fractional, number, int(multi2Number + number * multi2Fractional));
            if(integer==8)printf("Original %d\n",getm8b8(number));
            if(integer==16)printf("Oirignal %d\n",getm16b8(number));
            printf("-----------------------------------------------------\n");
#endif
            return int(multi2Number + number * multi2Fractional);
        }
    }
}

//for signed value
int getm8b8(double x)
{
    if (x > 255)
    {
        printf("error: value is too large");
        return 254;
    }
    //printf("hello, getm8b8\n");
    //printf("Value :%f\n", x);
    if (x == 0)
    {
        //printf("0 Change to: %d\n", 0);
        return 0;
    }
    if (x > 0)
    {
        //printf("+ Change to: %d\n", int(x * power(2, 8)));
        //printBinary(int(x * power(2, 8)));
        return (int(x*power(2, 8)));
    }
    else
    {
        //printf("- Change to: %d\n", int(power(2, 16) + x * power(2, 8)));
        return (int(power(2, 16)+x*power(2, 8)));
    }
}
int getm16b8(double x)
{
    if (x > 32768)
    {
        printf("error: value is too large");
        return 32767;
    }

    //printf("hello, getm16b8\n");
    //printf("Value : %f\n", x);

    if (x == 0)
    {
        //printf("0 Change to: %d\n", 0);
        return 0;
    }
    if (x > 0)
    {
        //printf("+ Change to: %d\n", int(x * power(2.0, 8)));
        return (int(x*power(2, 8)));
    }
    else
    {
        //printf("- Change to: %d\n", int(power(2, 24) + x * power(2, 8)));
        return (int(power(2, 24)+x*power(2, 8)));
    }
}
//For signed
double translatem8b8(int x)
{
    
    int sign = (x) & (0x8000);
    double fraction = x & (0xff);
    double integer = (x >> 8) & (0xff);
    
#ifdef DEBUG
    printf("m8b8\n");
    if (sign == 0)printf("number is positive\n");
    else 
    { 
        printf("number is negative\n"); 
    }
    printf("original: ");
    printBinary(x);
    printf("fractional: ");
    printBinary(fraction);
    printf("integer: ");
    printBinary(integer);
#endif
    if (x == 0)return 0;
    else if (sign == 0) {
        if (fraction >= 255.0)
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }
        return integer + ((fraction) / 255.0);
    }
    else
    {
        integer = power(2, 8) - integer;

        if (fraction > 255.0)
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }
        return -integer + ((fraction) / 255.0);
    }
}
double translatem16b8(int x)
{
    int sign = (x) & (0x800000);
    double fraction = x & (0xff);
    double integer = (x >> 8) & (0xffff);

#ifdef DEBUG
    printf("m16b8\n");
    if (sign == 0)printf("number is positive\n");
    else 
    { 
        printf("number is negative\n"); 
    }
    printf("original: ");
    printBinary(x);
    printf("fractional: ");
    printBinary(fraction);
    printf("integer: ");
    printBinary(integer);
#endif
    
    if (x == 0)return 0;
    else if (sign==0) {
        if (fraction > 255)
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }
        return integer + ((fraction) / 255.0);
    }
    else
    {
        integer = power(2, 16) - integer;

        if (fraction > 255)
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }
        return -integer + ((fraction) / 255.0);
    }
}

double translatefi(int x, int integer, int fractional)
{
    int mask;
    int sign = 0;
    mask = 1 << (integer + fractional - 1);
    sign = x & mask ? 1 : 0;

    int refFractional = makeserialf(fractional);
    int refInteger = makeserialf(integer);

    double fractionLocal = x & (refFractional);
    double integerLocal = (x >> fractional) & (refInteger);

    double numerator = fractionLocal;
    double denominator = power(2, fractional) - 1;
    double fractionalFinal = numerator / denominator;
    double numberFinal = 0.0;

#ifdef DEBUG
    printf("fi\n");
    if (sign == 0)printf("number is positive\n");
    else
    {
        printf("number is negative\n");
    }
    printf("original: ");
    printBinary(x);
    printf("fractional: ");
    printBinary(fractionLocal);
    printf("integer: %f\n", integerLocal);
    printBinary(integerLocal);

    printf("fractionalFinal: ");
    printf("%f\n", fractionalFinal);
#endif

    if (x == 0)return 0;
    else if (sign == 0) {
        if (fractionLocal > (power(2, fractional)-1))
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }

        numberFinal = integerLocal + fractionalFinal;
        return numberFinal;
    }
    else
    {
        integerLocal = power(2, integer) - integerLocal;
        if (fractionLocal > power(2, fractional)-1)
        {
            printf("ERROR: fraction number > 255\n");
            return 0;
        }
        numberFinal = -integerLocal + fractionalFinal;
        return numberFinal;
    }
}
