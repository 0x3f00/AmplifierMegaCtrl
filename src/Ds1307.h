#ifndef _DS1307_H
#define _DS1307_H

#include "defs.h"

class Ds1307
{
public:
	void	init();
	void	step();
	void	GetShort(uint8_t * h, uint8_t *m);
	
protected:
	void	I2C_Init();
	uint8_t I2C_Start();
	uint8_t I2C_SendAddr(uint8_t addr);
	uint8_t I2C_Write(uint8_t data);
	uint8_t I2C_ReadNack();
	void	I2C_WriteRegister(uint8_t addr, uint8_t data);
	uint8_t I2C_ReadRegister(uint8_t addr);

	enum {
		edsmHours = 0,
		edsmMinute,
	};

	enum {
		edsNone
		,edsBeginStart1
		,edsWaitingStart1
		,edsBeginSendDsAddr
		,edsWaitingSendDsAddr
		,edsBeginWriteRegAddr
		,edsWaitingWriteRegAddr
		,edsBeginStart2
		,edsWaitingStart2
		,edsBeginSendDsReadAddr
		,edsWaitingSendDsReadAddr
		,edsBeginReadReg
		,edsWaitingReadReg
		,edsStop
	};

	
protected:
	uint8_t	m_mode;
	uint8_t m_state;
	uint8_t m_h;
	uint8_t m_m;
};


#endif // _DS1307_H
