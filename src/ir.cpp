
#include "defs.h"
#include "ir.h"

#define EDGE_TO_BE_FALLING 0
#define EDGE_TO_BE_RAISING 1


Ir gIr;


void Ir::init()
{
	cmd = 0;
	cmdOut = 0;
	work_time = 0;
	last_edge_time = 0;


    EICRA &= (~((1<<ISC11)|(1<<ISC10)));
    EICRA |= (1<<ISC11); // FALLING
	edge_dir = EDGE_TO_BE_FALLING;
    
    EIMSK|=(1<<INT1);
}

uint8_t Ir::get()
{
	if(!cmdOut)
		return IR_NONE;
	
	if((cmdOut & 0xFF00) == 0x3800)
	{
		switch(cmdOut & 0xFF)
		{
			case 0xE6: return IR_POWER;
			case 0x9E: return IR_MUTE;
			case 0x82: return IR_DOWN;
			case 0x84: return IR_UP;
			case 0x80: return IR_LEFT;
			case 0xA8: return IR_RIGHT;
			case 0xA6: return IR_OK;
			case 0xDC: return IR_CH_MINUS;
			case 0x86: return IR_CH_PLUS;
			case 0xD8: return IR_VOL_MINUS;
			case 0xDA: return IR_VOL_PLUS;
			case 0xFE: return IR_0;
			case 0xFC: return IR_1;
			case 0xFA: return IR_2;
			case 0xF8: return IR_3;
			case 0xF6: return IR_4;
			case 0xF4: return IR_5;
			case 0xF2: return IR_6;
			case 0xF0: return IR_7;
			case 0xEE: return IR_8;
			case 0xEC: return IR_9;
			case 0x98: return IR_SETUP;
			case 0x9A: return IR_F1;
			case 0x9C: return IR_F2;
			case 0x8C: return IR_F3;
			case 0x8A: return IR_F4;
			case 0xC4: return IR_INFO;
			case 0xEA: return IR_MENU;
			case 0xE0: return IR_BACK;
			case 0xE4: return IR_EXIT;
			case 0xD6: return IR_PREV;
			case 0xD4: return IR_NEXT;
			case 0xA2: return IR_CLOCK;
			case 0xAC: return IR_REC;
		}
		return IR_OTHER;
	}
	else if(((cmdOut & 0xFF00) == 0x3700) || ((cmdOut & 0xFF00) == 0x2700))
	{
		switch(cmdOut & 0xFF)
		{
			case 0x18: return IR_POWER;
			case 0x60: return IR_MUTE;
			case 0x7C: return IR_DOWN;
			case 0x7B: return IR_UP;
			case 0x7F: return IR_LEFT;
			case 0x57: return IR_RIGHT;
			case 0x58: return IR_OK;
			case 0x23: return IR_CH_MINUS;
			case 0x78: return IR_CH_PLUS;
			case 0x27: return IR_VOL_MINUS;
			case 0x24: return IR_VOL_PLUS;
			case 0x00: return IR_0;
			case 0x03: return IR_1;
			case 0x04: return IR_2;
			case 0x07: return IR_3;
			case 0x08: return IR_4;
			case 0x0B: return IR_5;
			case 0x0C: return IR_6;
			case 0x0F: return IR_7;
			case 0x10: return IR_8;
			case 0x13: return IR_9;
			case 0x67: return IR_SETUP;
			case 0x64: return IR_F1;
			case 0x63: return IR_F2;
			case 0x73: return IR_F3;
			case 0x74: return IR_F4;
			case 0x3B: return IR_INFO;
			case 0x14: return IR_MENU;
			case 0x1F: return IR_BACK;
			case 0x1B: return IR_EXIT;
			case 0x28: return IR_PREV;
			case 0x2B: return IR_NEXT;
			case 0x53: return IR_CLOCK;
			case 0x3C: return IR_REC;
		}
		return IR_OTHER;
	}

	return IR_OTHER;
}

uint16_t Ir::get_raw()
{
	return cmdOut;	
}

void Ir::clear()
{
	cli();
	cmdOut = 0;
	cmd = 0;
	sei();
}

ISR(INT1_vect)
{
	gIr.on_interruption();
}

void Ir::on_interruption()
{
	static uint8_t currLevel = 0;
	static uint8_t ignore_next = 0;
	static uint8_t bitPos = 0;

	if(work_time > (last_edge_time + 150)) // need reset and start receiving	
	{
		currLevel = 1;
		cmd = 0;
		cmdOut = 0;
		bitPos = 0;
		last_edge_time = 0;
		work_time = 55;
	}

	if(EDGE_TO_BE_RAISING == edge_dir)
	{
		EICRA&=(~((1<<ISC11)|(1<<ISC10)));
		EICRA|=(1<<ISC11); // Now catch FALLING
		edge_dir = EDGE_TO_BE_FALLING;
	}
	else // EDGE_FALLING
	{
		EICRA|=((1<<ISC11)|(1<<ISC10));// Now catch RAISING
		edge_dir = EDGE_TO_BE_RAISING;
	}

	if(ignore_next)
	{
		ignore_next = false;
		last_edge_time = work_time;
		return;
	}
	
	if(work_time < (last_edge_time + 75))
	{
		ignore_next = true;
		if(currLevel)
			cmd |= (0x2000>>bitPos);
		bitPos++;
	}
	else if(work_time > (last_edge_time + 75))
	{
		currLevel = !currLevel;
		if(currLevel)
			cmd |= (0x2000>>bitPos);
		bitPos++;
	}
	
	last_edge_time = work_time;	
}

void Ir::on_tick()
{
	work_time++;
	if(work_time > (last_edge_time + 200))
	{
		cmdOut = cmd;
	}
}

