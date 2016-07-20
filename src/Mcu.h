#ifndef _MCU_H
#define _MCU_H

#include "Amp.h"
#include "ClockSetup.h"


class Mcu {
public:
	void		init();
	void		loop();

	uint8_t		get(uint8_t pin);
	void		set(uint8_t pin, uint8_t val);
	
protected:
	ClockSetup	m_clock_setup;
};

#endif // _MCU_H
