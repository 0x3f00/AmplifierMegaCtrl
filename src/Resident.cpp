/*
 * Resident.cpp
 *
 * Created: 14.06.2016 21:11:37
 *  Author: glassy
 */ 

#include "defs.h"

#include "Mcu.h"

Mcu gMcu;

int main(void)
{
	gMcu.init();
	sei();
    while(1)
    {
        gMcu.loop();
    }
}

