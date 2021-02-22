// TestProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "dataformat.h"
#include "config.h"
#include <ctime>

int main()
{
    std::cout << "Hello World!\n";

    double minus = -4.5645;
    double plus = 105.000000;

    //int minus_m8b8 = getm8b8(minus);
    //int plus_m8b8 = getm8b8(plus);

    //int minus_m16b8 = getm16b8(minus);
    //int plus_m16b8 = getm16b8(plus);

    //printf("[m8b8] minus case: %f ->  %d\n", minus, minus_m8b8);
    //printf("[m8b8] plus case: %f ->  %d\n", plus, plus_m8b8);
    //printf("[m16b8] minus case: %f ->  %d\n", minus, minus_m16b8);
    //printf("[m16b8] plus case: %f ->  %d\n", plus, plus_m16b8);

    //double minus_m8b8_trans = translatem8b8(minus_m8b8);
    //double plus_m8b8_trans = translatem8b8(plus_m8b8);
    //double minus_m16b8_trans = translatem16b8(minus_m16b8);
    //double plus_m16b8_trans = translatem16b8(plus_m16b8);

	//printf("[m8b8, translate] minus case: %f ->  %d -> %f\n", minus, minus_m8b8, minus_m8b8_trans);
	//printf("[m8b8, translate] plus case: %f ->  %d -> %f\n", plus, plus_m8b8, plus_m8b8_trans);
	//printf("[m16b8, translate] minus case: %f ->  %d -> %f\n", minus, plus_m16b8, minus_m16b8_trans);
	//printf("[m16b8, translate] plus case: %f ->  %d -> %f\n", plus, plus_m16b8, plus_m16b8_trans);


	std::cout << "RealTest!\n";
#ifdef POW_TEST
	int nRandom_base = 2;
	int nRandom_exponent = rand() % 256;
	int x = power(nRandom_base, nRandom_exponent);
	printf("Power: %d**%d = %d\n", nRandom_base, nRandom_exponent, x);
#endif

#ifdef DATAFORMAT_LOCAL_TEST
	int max_count = TEST_CASE;
	int count = 0;
	int sign = 0;

	while (count < max_count)
	{
		double x = 0.0;
		double y = 0.0;
		// rand 0-32767
		//srand((unsigned int)time(0));
		double nRandom_integer_x = rand() % 127;
		double nRandom_fractional_x = (rand() % 127) / 127.0;

		double nRandom_integer_y = rand() % 32767;
		double nRandom_fractional_y = (rand() % 32767) / 32767.0;

		double resolution = round(1.0 / 255.0 * power(10, 7)) / power(10, 7);

		if (sign == 0)
		{
			x = nRandom_integer_x + nRandom_fractional_x;
			y = nRandom_integer_y + nRandom_fractional_y;
			if (count > trunc(max_count / 2))sign++;
		}
		else
		{
			x = -1.0 * (nRandom_integer_x + nRandom_fractional_x);
			y = -1.0 * (nRandom_integer_y + nRandom_fractional_y);
		}

		int m8b8 = getm8b8(x);
		int m16b8 = getm16b8(y);

		double m8b8_trans = translatem8b8(m8b8);
		double m16b8_trans = translatem16b8(m16b8);

		if ((abs(x - m8b8_trans) < resolution) && (abs(y - m16b8_trans) < resolution))
		{
			printf("time: %d value: m8b8 %f, m16b8 %f\n", count, x, y);
			count++;
			if (count == max_count)printf("DONE\n");
			continue;
		}
		else
		{
			printf("ERROR : time: %d #m8b8: value %f -> %f ,#m16b8: value %f -> %f\n", count, x, m8b8_trans, y, m16b8_trans);
			break;
		}
	}
#endif

#ifdef DATAFORMAT_TEST
	int max_count = TEST_CASE;
	int count = 0;
	int sign = 0;

	while(count < max_count)
	{
		double x = 0.0;
		double y = 0.0;
		// rand 0-32767
		//srand((unsigned int)time(0));
		double nRandom_integer_x = rand() % 127;
		double nRandom_fractional_x = (rand() % 127) / 127.0;

		double nRandom_integer_y = rand() % 32767;
		double nRandom_fractional_y = (rand() % 32767) / 32767.0;

		double resolution = round(1.0 / 255.0 * power(10, 7)) / power(10, 7);

		 if(sign == 0)
		 {
		 	x = nRandom_integer_x + nRandom_fractional_x;
			y = nRandom_integer_y + nRandom_fractional_y;
		 	if(count>trunc(max_count/2))sign++;
		 }
		 else
		 {
		 	x = -1.0*(nRandom_integer_x + nRandom_fractional_x);
			y = -1.0*(nRandom_integer_y + nRandom_fractional_y);
		 }

		 int m8b8 = fi(x, 8, 8);
		 int m16b8 = fi(y, 16, 8);

		 int m8b8_comp = getm8b8(x);
		 int m16b8_comp = getm16b8(y);
		 if ((m8b8 != m8b8_comp) || (m16b8 != m16b8_comp))printf("ERROR: fi\n");
		 //printf("%d %d, %d %d\n", m8b8, m8b8_comp, m16b8, m16b8_comp);

		 double m8b8_trans = translatefi(m8b8, 8, 8);
		 double m16b8_trans = translatefi(m16b8, 16, 8);

		 if((abs(x-m8b8_trans)< resolution)&&(abs(y-m16b8_trans)< resolution))
		 {
			printf("time: %d value: m8b8 %f, m16b8 %f\n", count, x, y);
			count++;
			if (count == max_count)printf("DONE\n");
		 	continue;
		 }
		 else
		 {
			printf("ERROR : time: %d #m8b8: value %f -> %f ,#m16b8: value %f -> %f\n",count, x, m8b8_trans, y, m16b8_trans);
		 	break;
		 }
	}
#endif
    return 0;
}
