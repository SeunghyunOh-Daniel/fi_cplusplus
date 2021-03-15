#include <stdio.h>
#include <iostream>
#include <inttypes.h>
#include <iomanip>

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
    if ((integer > 16) || (fractional > 8) || ((integer + fractional) > 24))
    {
        printf("error::value:DataFormat::fi\n");
        return 32768;
    }
    else
    {
        int multi2Fractional = power(2, fractional);
        int multi2Number = power(2, integer + fractional);

        if ((multi2Fractional == -1) || (multi2Number) == -1)
        {
            printf("error::value:DataFormat::fi\n");
            return 32768;
        }
        else if ((number > power(2, integer - 1)) || (number < -1 * power(2, integer - 1)))
        {
            printf("error::value:DataFormat::fi\n");
            return 32768;
        }
        else
        {
            if (number == 0)
            {
                return 0;
            }
            else if (number > 0)
            {
                int temp = trunc(number * multi2Fractional);
                return temp;
            }
            else
            {
                int temp = trunc(multi2Number + number * multi2Fractional);
                return temp;
            }
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
    if ((integer > 16) || (fractional > 8) || ((integer + fractional) > 24))
    {
        printf("error::value:DataFormat::fi\n");
        return 32768;
    }
    else
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

        if (x == 0)return 0;
        else if (sign == 0) {
            if (fractionLocal > (power(2, fractional) - 1))
            {
                printf("error::value:DataFormat::translatefi\n");
                return 0;
            }

            numberFinal = integerLocal + fractionalFinal;
            return numberFinal;
        }
        else
        {
            integerLocal = power(2, integer) - integerLocal;
            if (fractionLocal > power(2, fractional) - 1)
            {
                printf("error::value:DataFormat::translatefi\n");
                return 0;
            }
            numberFinal = -integerLocal + fractionalFinal;
            return numberFinal;
        }
    }
}

bool is_Littleendian()
{

    int testNumber = 0x12345678;
    uint8_t* temp = (uint8_t*)malloc(sizeof(uint8_t) * 4);
    if (temp == 0)return NULL;
    memset(temp, 0, sizeof(uint8_t) * 4);

    uint8_t* tempValue = (uint8_t*)&testNumber;
    for (uint8_t i = 0; i < 4; i++)temp[i] = tempValue[i];

    if ((temp[0] == 0x78) && (temp[1] == 0x56) && (temp[2] == 0x34) && (temp[3] == 0x12))
    {
        free(temp);
        return 1;
    }
    else
    {
        free(temp);
        return 0;
    }
}

int len(char* text)
{
    unsigned int len = 0;
    int max = 2048;

    char* tmpData = (char*)malloc(sizeof(char) * max);
    if (tmpData == 0)return -1;
    memset(tmpData, 0, sizeof(char) * max);
    sprintf(tmpData, "%s", text);
    for (uint8_t i = 0; i < max; i++)
    {
        if (tmpData[i] == 0)break;
        len++;
        if (i == max - 1)len = -1;
    }

    free(tmpData);
    if (len < 0)return -1;
    return len;
}

int convert_endian(int number)
{
    int numberLocal = number;
    uint8_t* temp;
    temp = (uint8_t*)&numberLocal;

    int resultNumber = 0;
    uint8_t* tempResult;
    tempResult = (uint8_t*)&resultNumber;

    for (uint8_t i = 0; i < 4; i++)tempResult[i] = temp[3 - i];
   
    return resultNumber;
}

char* split(char* text, char character)
{
    int max = 500;

    char* tmpData = (char*)malloc(sizeof(char) * max);
    if (tmpData == 0)return 0;
    memset(tmpData, 0, sizeof(char) * max);
    sprintf(tmpData, "%s", text);

    char* name = (char*)malloc(sizeof(char) * max);
    if (name == 0)
    {
        free(tmpData);
        return 0;
    }
    memset(name, 0, sizeof(char) * max);

    char* buf = (char*)malloc(sizeof(char) * 2);
    if (buf == 0)
    {
        free(tmpData);
        free(name);
        return 0;
    }
    memset(buf, 0, 2);

    int lengthData = len(tmpData);
    if (lengthData == -1)return 0;
    for (uint8_t i = 0; i < lengthData; i++)
    {
        char temp = tmpData[i];
        if (temp == character)break;
        if (tmpData[i] == 0)break;

        name[i] = temp;
    }

    free(buf);
    free(tmpData);
    return name;
}

void test()
{
    double x = 97.8125;
    uint8_t* temp;
    temp = (uint8_t*)&x;
    //printf("%d, %d",sizeof(x), sizeof(*temp));
    for (uint8_t i = 8; i > 0; i--)printf("%X ",temp[i]);
    //printf("%f", x);
    //0x003c3f5d95baff44
}