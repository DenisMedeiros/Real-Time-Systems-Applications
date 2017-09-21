#include "adc.h"
#include <iostream>

using namespace std;

ADC::ADC(AINx id)
{
	this->adcNumber = id;
	this->baseDir = "/sys/bus/iio/devices/iio:device0/in_voltage" + std::to_string(this->adcNumber) + string("_raw");
}

int ADC::getIntValue()
{
	int retorno = -1;
	fstream fs;
	fs.open(this->baseDir.c_str(), fstream::in | fstream::binary);
	if(fs.good())
	{
		fs >> retorno;
		fs.close();
	}
	return retorno;
}

float ADC::getPercentValue()
{
	int temp = getIntValue();
	return (temp*100.0f)/4096.0;
	
}

