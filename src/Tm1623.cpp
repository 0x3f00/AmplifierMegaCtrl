

#include "defs.h"
#include "Tm1623.h"


Tm1623 gTm;




void Tm1623::init()
{
	for(uint8_t i = 0; i < TM1623_MEM_SIZE; i++)
		mem[i] = 0;

	DDRB |= (1 << TM1623_DIO_PIN);
	DDRB |= (1 << TM1623_CLK_PIN);
	DDRB |= (1 << TM1623_STB_PIN);

	
	send_cmd(0x2);	// 6 Grids, 12 Segments mode

	send_cmd(0x40);
	send_cmd(0x80);

	PORTB &= ~(1<<TM1623_STB_PIN);
	send(0xC0);
	clear();
	PORTB |= (1<<TM1623_STB_PIN);

	send_cmd(0x88 | TM1623_BRIGHTNESS);
}


// Here comes segments
const char table_even[18] = {
	 0b11101000         // 0
	,0b00001000         // 1
	,0b11011000         // 2
	,0b10011000         // 3
	,0b00111000         // 4
	,0b10111000         // 5
	,0b11111000         // 6
	,0b00001000         // 7
	,0b11111000         // 8
	,0b10111000         // 9
	,0b01110000         // A
	,0b11110000         // B
	,0b11100000         // C
	,0b11010000         // D
	,0b11110000         // E
	,0b01110000         // F
	,0b00000000         // " "
	,0b00010000         // "-"
};

const char table_odd[18] = {
	 0b1011         // 0
	,0b0011         // 1
	,0b1010         // 2
	,0b1011         // 3
	,0b0011         // 4
	,0b1001         // 5
	,0b1001         // 6
	,0b1011         // 7
	,0b1011         // 8
	,0b1011         // 9
	,0b1011         // A
	,0b0001         // B
	,0b1000         // C
	,0b0011         // D
	,0b1000         // E
	,0b1000         // F
	,0b0000         // " "
	,0b0000         // "-"
};

void Tm1623::print4(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	mem[0] = table_even[d];
	mem[1] = table_odd[d];
	mem[2] = table_even[c];
	mem[3] = table_odd[c];
	mem[4] = table_even[b];
	mem[5] = table_odd[b];
	mem[6] = table_even[a];
	mem[7] = table_odd[a];
	update();
}

void Tm1623::send_cmd(uint8_t cmd)
{
	PORTB &= ~(1<<TM1623_STB_PIN);
	send(cmd);
	PORTB |= (1<<TM1623_STB_PIN);
}

void Tm1623::send_data(uint8_t addr, uint8_t data)
{
	send_cmd(0x44);
	PORTB &= ~(1<<TM1623_STB_PIN);
	send(0xC0 | addr);
	send(data);
	PORTB |= (1<<TM1623_STB_PIN);
}

void Tm1623::send(uint8_t data)
{
	for(uint8_t i = 0; i < 8; i++)
	{
		PORTB &= ~(1<<TM1623_CLK_PIN);

		if(data & 1)
			PORTB |= (1<<TM1623_DIO_PIN);
		else
			PORTB &= ~(1<<TM1623_DIO_PIN);

		data >>= 1;
		PORTB |= (1<<TM1623_CLK_PIN);
	}		    
}

void Tm1623::clear()
{
	for(uint8_t i = 0; i < TM1623_MEM_SIZE; i++)
		mem[i] = 0;
    update();
}

void Tm1623::update()
{
	for(uint8_t i = 0; i < TM1623_MEM_SIZE; i++)
		send_data(i, mem[i]);
}

