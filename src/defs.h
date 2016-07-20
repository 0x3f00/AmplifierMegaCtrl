#ifndef _DEFS_H
#define _DEFS_H



#define F_CPU 8000000UL

#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#define PIN_AMP_POWER		PORTB0
#define PIN_AMP_SPEAKERS	PORTB2

#define PIN_SRC_DETECTOR_1	PORTC0

#define PIN_SRC_RELAY_0		PORTC1
#define PIN_SRC_RELAY_1		PORTC2
#define PIN_SRC_RELAY_2		PORTC3

#define PIN_VOL_DOWN		PORTD1
#define PIN_VOL_UP			PORTD0

#define AUDIO_SRC_OFFSET ((uint8_t*)4)




#endif // _DEFS_H