
#include "defs.h"
#include "Mcu.h"
#include "Amp.h"
#include "ir.h"
#include "Tm1623.h"
#include "Timer.h"
#include "Ds1307.h"

extern Amp gAmp;
extern Ir gIr;
extern Tm1623 gTm;
extern MyTimer glTimer;
extern Ds1307 gDs;

void Mcu::init()
{
	glTimer.init();
	gAmp.init();
	gIr.init();
	gTm.init();
	gDs.init();
}

void Mcu::loop()
{
	if(gIr.get())
	{
		switch(gIr.get())
		{
			case IR_POWER: gAmp.on_power_pressed(); break;
			case IR_MUTE: gAmp.on_mute_pressed(); break;
			case IR_RIGHT: gAmp.on_source_pressed(); break;
			case IR_VOL_MINUS: gAmp.on_vol_minus(); break;
			case IR_VOL_PLUS: gAmp.on_vol_plus(); break;
			case IR_CH_MINUS: gAmp.on_vol_minus(); break;
			case IR_CH_PLUS: gAmp.on_vol_plus(); break;
		}
		gIr.clear();
	}
	
	gDs.step();
	
	{
		static uint8_t h = 0;
		static uint8_t m = 0;
		uint8_t newh = 0;
		uint8_t newm = 0;

		gDs.GetShort(&newh, &newm);
		if((h != newh) || (m != newm))
		{
			h = newh;
			m = newm;
			gTm.print4((h&0x3f)>>4, h&0xf, m>>4, m&0xf);
		}			
	}		
}


