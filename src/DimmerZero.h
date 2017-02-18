/*
  DimmerZero.h - Library for dimmer application on SAMD21G18A (e.g. Arduino Zero/M0).
  Created by E.Burkowski, February 18, 2017.
  Released into the public domain.
*/
#ifndef DimmerZero_h
#define DimmerZero_h

#include "Arduino.h"

class DimmerZero
{
  public:
	DimmerZero(int pin);
	DimmerZero(int pin, bool invert);
	void setValue(int value);
	int getMaxValue();
	void setMaxValue(int maxValue);
	void setFrequency(int freq);
	void init();
  private:
	int _maxValue;
	int _pin;
	int _frequency;
	bool _invert;
};

#endif