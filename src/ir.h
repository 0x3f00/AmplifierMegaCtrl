#ifndef _IR_H
#define _IR_H

#include "defs.h"

enum IR_KEY_CODES
{
	 IR_NONE = 0
	,IR_POWER
	,IR_MUTE
	,IR_DOWN
	,IR_UP
	,IR_LEFT
	,IR_RIGHT
	,IR_OK
	,IR_CH_MINUS
	,IR_CH_PLUS
	,IR_VOL_MINUS
	,IR_VOL_PLUS
	,IR_0
	,IR_1
	,IR_2
	,IR_3
	,IR_4
	,IR_5
	,IR_6
	,IR_7
	,IR_8
	,IR_9
	,IR_SETUP
	,IR_F1
	,IR_F2
	,IR_F3
	,IR_F4
	,IR_INFO
	,IR_MENU
	,IR_BACK
	,IR_EXIT
	,IR_PREV
	,IR_NEXT
	,IR_CLOCK
	,IR_REC
	,IR_OTHER
};

class Ir
{
public: 
	
	void		init();
	uint8_t		get();
	uint16_t	get_raw();
	void		clear();
	
	void		on_tick();
	void		on_interruption();
	
protected:
	uint32_t work_time; // in 100 us
	uint32_t last_edge_time;
	uint8_t edge_dir;	
	uint32_t cmd;
	uint32_t cmdOut;
};

#endif // _IR_H
