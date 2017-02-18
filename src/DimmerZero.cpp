/*
  DimmerZero.cpp - Library for dimmer application with SAMD21G18A (e.g. Arduino Zero/M0).
  Created by E.Burkowski, February 18, 2017.
  Released into the public domain.
*/

#include "Arduino.h"
#include "DimmerZero.h"

/*
Recommended pwm pins: 2,3,4,5,6,7,11,13
For other pins see: https://github.com/Adminius/DimmerZero
Supported PWM frequencies 250Hz, 500Hz, 1000Hz. default is 1000Hz
*/

DimmerZero::DimmerZero(int pin)
{
  _pin = pin;
  _invert = false;
  _frequency = 1000;
  _maxValue = 12000;
  }


DimmerZero::DimmerZero(int pin, bool invert)
{
  _pin = pin;
  _invert = invert;
  _frequency = 1000;
  _maxValue = 12000;
}


void DimmerZero::init()
{	bool timerTCC0 = false;
	bool timerTCC1 = false;
	bool timerTCC2 = false;
	
	byte div = 1;
	if (_frequency <= 250)
		div = 8;
	else if(_frequency > 250 && _frequency <= 500)
		div = 4;
	else
		div = 2;
	
	REG_GCLK_GENDIV = GCLK_GENDIV_DIV(div) |		// Divide the 48MHz clock source by divisor div: e.g. 48MHz/4=12MHz
						GCLK_GENDIV_ID(4);			// Select Generic Clock (GCLK) 4
	while (GCLK->STATUS.bit.SYNCBUSY);				// Wait for synchronization

	REG_GCLK_GENCTRL = GCLK_GENCTRL_IDC |			// Set the duty cycle to 50/50 HIGH/LOW
						GCLK_GENCTRL_GENEN |		// Enable GCLK4
						GCLK_GENCTRL_SRC_DFLL48M |	// Set the 48MHz clock source
						GCLK_GENCTRL_ID(4);			// Select GCLK4
	while (GCLK->STATUS.bit.SYNCBUSY);				// Wait for synchronization
 
	PORT->Group[g_APinDescription[_pin].ulPort].PINCFG[g_APinDescription[_pin].ulPin].bit.PMUXEN = 1;
	switch (_pin) {
		case 0:
			PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC1 = true;
			break;
		case 1:
			PORT->Group[g_APinDescription[1].ulPort].PMUX[g_APinDescription[1].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC1 = true;
			break;
		case 2:
			PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC0 = true;
			break;
		case 3:
			PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC1 = true;
			break;
		case 4:
			PORT->Group[g_APinDescription[4].ulPort].PMUX[g_APinDescription[4].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC1 = true;
			break;
		case 5:
			PORT->Group[g_APinDescription[2].ulPort].PMUX[g_APinDescription[2].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC0 = true;
			break;
		case 6:
			PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC0 = true;
			break;
		case 7:
			PORT->Group[g_APinDescription[6].ulPort].PMUX[g_APinDescription[6].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC0 = true;
			break;
		case 8:
			PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC1 = true;
			break;
		case 9:
			PORT->Group[g_APinDescription[8].ulPort].PMUX[g_APinDescription[8].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC1 = true;
			break;
		case 10:
			PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXE_F;
			timerTCC0 = true;
			break;
		case 11:
			PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC2 = true;
			break;
		case 12:
			PORT->Group[g_APinDescription[10].ulPort].PMUX[g_APinDescription[10].ulPin >> 1].reg |= PORT_PMUX_PMUXO_F;
			timerTCC0 = true;
			break;
		case 13:
			PORT->Group[g_APinDescription[11].ulPort].PMUX[g_APinDescription[11].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC2 = true;
			break;
		case A3:
			PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXO_E;
			timerTCC0 = true;
			break;
		case A4:
			PORT->Group[g_APinDescription[A3].ulPort].PMUX[g_APinDescription[A3].ulPin >> 1].reg |= PORT_PMUX_PMUXE_E;
			timerTCC0 = true;
			break;
		default:
//			#error "Not supported pin!"
			break;
	}
	
	if (timerTCC0){
		// Feed GCLK4 to TCC0
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC0_TCC1;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) {
			REG_TCC0_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC0->SYNCBUSY.bit.WAVE);
		}else{
			REG_TCC0_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC0
			while (TCC0->SYNCBUSY.bit.WAVE);  
		}
		
		REG_TCC0_PER = _maxValue;					// Set the frequency of the PWM on TCC0 
		while (TCC0->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC0_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC0 output
		while (TCC0->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	if (timerTCC1){
		// Feed GCLK4 to TCC1
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC0_TCC1;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) {
			REG_TCC1_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC1->SYNCBUSY.bit.WAVE);
		}else{
			REG_TCC1_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC1
			while (TCC1->SYNCBUSY.bit.WAVE);  
		} 
		
		REG_TCC1_PER = _maxValue;						// Set the frequency of the PWM on TCC0 
		while (TCC1->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC1_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC1 output
		while (TCC1->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	if (timerTCC2){
		// Feed GCLK4 to TCC2
		REG_GCLK_CLKCTRL = GCLK_CLKCTRL_CLKEN |		// Enable GCLK4 to TCC0 and TCC1
		GCLK_CLKCTRL_GEN_GCLK4 |					// Select GCLK4
		GCLK_CLKCTRL_ID_TCC2_TC3;					// Feed GCLK4 to TCC0 and TCC1
		while (GCLK->STATUS.bit.SYNCBUSY);			// Wait for synchronization
		
		if(_invert) {
			REG_TCC2_WAVE |= TCC_WAVE_WAVEGEN_DSBOTH;
			while (TCC0->SYNCBUSY.bit.WAVE);
		}else{
			REG_TCC2_WAVE |= TCC_WAVE_POL(0xF) | TCC_WAVE_WAVEGEN_DSBOTH;	// Setup dual slope PWM on TCC2
			while (TCC2->SYNCBUSY.bit.WAVE);  
		}
		
		REG_TCC2_PER = _maxValue;					// Set the frequency of the PWM on TCC0 
		while (TCC2->SYNCBUSY.bit.PER);				// Wait for synchronization
		
		REG_TCC2_CTRLA |= TCC_CTRLA_PRESCALER_DIV1 |// Divide GCLK4 by 1
							TCC_CTRLA_ENABLE;		// Enable the TCC2 output
		while (TCC2->SYNCBUSY.bit.ENABLE);			// Wait for synchronization

	}
	
#ifndef __SAMD21G18A__
    #error "This library only supports SAMD21G18A based boards (e.g. Zero/M0...)"
#endif

}

void DimmerZero::setValue(int value)
{
	int normalizedValue = value;
	if (value > _maxValue){
		normalizedValue = _maxValue;
	}
	if (value < 0){
		normalizedValue = 0;
	}
	switch (_pin) {
		case 0:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 1:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 2:
			REG_TCC0_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 3:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 4:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 5:
			REG_TCC0_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 6:
			REG_TCC0_CC2 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC2);		// Wait for synchronization
			break;
		case 7:
			REG_TCC0_CC3 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC3);		// Wait for synchronization
			break;
		case 8:
			REG_TCC1_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 9:
			REG_TCC1_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC1->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case 10:
			REG_TCC0_CC2 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC2);		// Wait for synchronization
			break;
		case 11:
			REG_TCC2_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC2->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case 12:
			REG_TCC0_CC3 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC3);		// Wait for synchronization
			break;
		case 13:
			REG_TCC2_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC2->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		case A3:
			REG_TCC0_CC0 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC0);		// Wait for synchronization
			break;
		case A4:
			REG_TCC0_CC1 = normalizedValue;		// Set the PWM signal
			while (TCC0->SYNCBUSY.bit.CC1);		// Wait for synchronization
			break;
		default:
			break;
	}
}

int DimmerZero::getMaxValue(){
	return _maxValue;
}

void DimmerZero::setMaxValue(int maxValue){
	_maxValue = maxValue;
}

void DimmerZero::setFrequency(int freq){
	_frequency = freq;
}

