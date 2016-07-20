
#include "defs.h"
#include "Ds1307.h"

#define F_SCL 100000L // I2C clock speed 100 k Hz

#define TW_START 0xA4 // send start condition (TWINT,TWSTA,TWEN)
#define TW_READY (TWCR & 0x80) // ready when TWINT returns to logic 1.
#define TW_STATUS (TWSR & 0xF8) // returns value of status register

#define DS1307 0xD0 // I2C bus address of DS1307 RTC
#define TW_SEND 0x84 // send data (TWINT,TWEN)

#define TW_STOP 0x94 // send stop condition (TWINT,TWSTO,TWEN)
#define I2C_Stop() do {TWCR = TW_STOP; } while(0) // inline macro for stop condition

#define TW_NACK 0x84	// read with NACK (no more data needed)
#define READ	1


#define SECONDS_REGISTER	0x00
#define MINUTES_REGISTER	0x01
#define HOURS_REGISTER		0x02
#define DAYOFWEEK_REGISTER	0x03
#define DAYS_REGISTER		0x04
#define MONTH_REGISTER		0x05
#define YEARS_REGISTER		0x06



Ds1307 gDs;


void Ds1307::init()
{
	m_mode = edsmHours;
	m_state = edsNone;
	m_h = 0;
	m_m = 0;
}

void Ds1307::GetShort(uint8_t * h, uint8_t *m)
{
	*m = m_m;
	*h = m_h;
	*h &= 0x3F;	
}

void Ds1307::I2C_Init()
{
	TWSR = 0;
	TWBR = ((F_CPU/F_SCL) - 16)/2; 
}

uint8_t Ds1307::I2C_Start()
{
	TWCR = TW_START;
	while(!TW_READY) {};
	return (TW_STATUS == 0x08);
}

uint8_t Ds1307::I2C_SendAddr(uint8_t addr)
{
	TWDR = addr;
	TWCR = TW_SEND;
	while(!TW_READY) {};
	return (TW_STATUS==0x18);		// return 1 if found; 0 otherwise
}

uint8_t Ds1307::I2C_Write(uint8_t data)
{
	// almost the same
	TWDR = data;
	TWCR = TW_SEND;	
	while(!TW_READY) {};
	return (TW_STATUS==0x28);		// return 1 if found; 0 otherwise
}

uint8_t Ds1307::I2C_ReadNack()
{
	TWCR = TW_NACK;
	while(!TW_READY) {};
	return TWDR;
}


void Ds1307::I2C_WriteRegister(uint8_t addr, uint8_t data)
{
	I2C_Start();
	I2C_SendAddr(DS1307);
	I2C_Write(addr);
	I2C_Write(data);
	I2C_Stop();
}


uint8_t Ds1307::I2C_ReadRegister(uint8_t addr)
{
	uint8_t data = 0;
	I2C_Start();
	I2C_SendAddr(DS1307);
	I2C_Write(addr);
	I2C_Start();
	I2C_SendAddr(DS1307 + READ);
	data = I2C_ReadNack();
	I2C_Stop();
	return data;
}




void Ds1307::step()
{
	switch(m_state)
	{
		case edsNone:
			m_state = edsBeginStart1;
			break;
		case edsBeginStart1:
			m_mode = !m_mode;
			TWCR = TW_START;
			m_state = edsWaitingStart1;
			break;
		case edsWaitingStart1:
			if(TW_READY)
			{
				m_state = ((TW_STATUS == 0x08) ? edsBeginSendDsAddr : edsNone);
			}				
			break;
		case edsBeginSendDsAddr:
			TWDR = DS1307;
			TWCR = TW_SEND;
			m_state = edsWaitingSendDsAddr;
			break;
		case edsWaitingSendDsAddr:
			if(TW_READY)
			{
				m_state = ( (TW_STATUS==0x18) ? edsBeginWriteRegAddr : edsNone);
			}				
			break;
		case edsBeginWriteRegAddr:
			TWDR = ((m_mode == edsmHours) ? HOURS_REGISTER : MINUTES_REGISTER);
			TWCR = TW_SEND;
			m_state = edsWaitingWriteRegAddr;
			break;
		case edsWaitingWriteRegAddr:
			if(TW_READY)
			{
				m_state = ( (TW_STATUS==0x28) ? edsBeginStart2 : edsNone);
			}				
			break;
		case edsBeginStart2:
			TWCR = TW_START;
			m_state = edsWaitingStart2;
			break;
		case edsWaitingStart2:
			if(TW_READY)
			{
				m_state = edsBeginSendDsReadAddr;
			}
			break;
		case edsBeginSendDsReadAddr:
			TWDR = DS1307 + READ;
			TWCR = TW_SEND;
			m_state = edsWaitingSendDsReadAddr;
			break;
		case edsWaitingSendDsReadAddr:
			if(TW_READY)
			{
				m_state = edsBeginReadReg;
			}
			break;
		case edsBeginReadReg:
			TWCR = TW_NACK;
			m_state = edsWaitingReadReg;
			break;
		case edsWaitingReadReg:
			if(TW_READY)
			{
				m_state = edsStop;
				if(m_mode == edsmHours) 
					m_h = TWDR;
				else
					m_m = TWDR;
			}				
			break;
		case edsStop:
			I2C_Stop();
			m_state = edsBeginStart1;
			break;
	}	
}




