#ifndef _TM1623_H
#define _TM1623_H

#define TM1623_BRIGHTNESS	1
#define TM1623_DIO_PIN	PORTB3
#define TM1623_CLK_PIN	PORTB4
#define TM1623_STB_PIN	PORTB5

#define TM1623_MEM_SIZE	0xD



class Tm1623
{
public:
	void init();
	void print4(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
	
protected:
	void send_cmd(uint8_t cmd);
	void send_data(uint8_t addr, uint8_t data);
	void send(uint8_t data);
	void clear();
	void update();
	
protected:
	uint8_t mem[TM1623_MEM_SIZE];
};


#endif // _TM1623_H