#include <iostream>
#include "Adafruit_ADS1015.h"

int main(int argc, char **argv)
{
	Adafruit_ADS1115 adc;
	adc.begin();

	uint16_t yaw = adc.readADC_SingleEnded(0);
	cout << yaw << endl;

	return 0;
}

