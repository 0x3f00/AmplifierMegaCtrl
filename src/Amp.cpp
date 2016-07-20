
#include "defs.h"
#include "Amp.h"
#include "Ds1307.h"
#include "Tm1623.h"

Amp gAmp;

extern Tm1623 gTm;
extern Ds1307 gDs;

void Amp::init()
{
	// initialize the digital pin as an input
	DDRC &= ~(1 << PIN_SRC_DETECTOR_1);
	// initialize the digital pin as an output.
	DDRC |= (1 << PIN_SRC_RELAY_0);
	DDRC |= (1 << PIN_SRC_RELAY_1);
	DDRC |= (1 << PIN_SRC_RELAY_2);
	DDRD |= (1 << PIN_VOL_DOWN);
	DDRD |= (1 << PIN_VOL_UP);
	DDRB |= (1 << PIN_AMP_POWER);
	DDRB |= (1 << PIN_AMP_SPEAKERS);

	PORTB = 0;	
	PORTC = 0;
	PORTD = 0;
	m_power_on = 0;
	m_speakers_on = 0;
	m_sourse_mode = eeprom_read_byte(AUDIO_SRC_OFFSET) & 0x03;
	m_sourse_curr = 0;
	sync_source();
}

void Amp::on_power_pressed()
{
	m_power_on = !m_power_on;
	if(m_power_on)
	{
		PORTB |= (1<<PIN_AMP_POWER);
		_delay_ms(30);	// TODO remake
		PORTB |= (1<<PIN_AMP_SPEAKERS);
		m_speakers_on = 1;
	}
	else
	{
		PORTB &= ~(1<<PIN_AMP_SPEAKERS);
		PORTB &= ~(1<<PIN_AMP_POWER);
	}		
}

void Amp::on_mute_pressed()
{
	if(!m_power_on)
		return;
		
	m_speakers_on = !m_speakers_on;
	if(m_speakers_on)
		PORTB |= (1<<PIN_AMP_SPEAKERS);
	else
		PORTB &= ~(1<<PIN_AMP_SPEAKERS);
}

void Amp::on_vol_minus()
{
	PORTD |= (1<<PIN_VOL_DOWN);
	gTm.print4(0x11, 0x11, 0x11, 0x11);
	_delay_ms(5);	// TODO remake
	uint8_t h = 0;
	uint8_t m = 0;
	gDs.GetShort(&h, &m);
	gTm.print4(h>>4, h&0xf, m>>4, m&0xf);
	PORTD &= ~(1<<PIN_VOL_DOWN);
}

void Amp::on_vol_plus()
{
	PORTD |= (1<<PIN_VOL_UP);
	gTm.print4(0x11, 0x11, 0x11, 0x11);
	_delay_ms(5);	// TODO remake
	uint8_t h = 0;
	uint8_t m = 0;
	gDs.GetShort(&h, &m);
	gTm.print4(h>>4, h&0xf, m>>4, m&0xf);
	PORTD &= ~(1<<PIN_VOL_UP);
}


void Amp::on_source_pressed()
{
	switch(m_sourse_mode)
	{
	case easmSrc0: m_sourse_mode = easmSrc1; break;
	case easmSrc1: m_sourse_mode = easmSrc2; break;
	case easmSrc2: m_sourse_mode = easmSrcAuto; break;
	case easmSrcAuto: m_sourse_mode = easmSrc0; break;
	};
	
	sync_source();
	eeprom_write_byte(AUDIO_SRC_OFFSET, m_sourse_mode);
}

void Amp::on_source_pin()
{
	sync_source();
	eeprom_write_byte(AUDIO_SRC_OFFSET, m_sourse_mode);
}

void Amp::sync_source()
{
	switch(m_sourse_mode)
	{
		case easmSrc0: 
			m_sourse_curr = 0; 
			break;
		case easmSrc1: 
			m_sourse_curr = 1; 
			break;
		case easmSrc2: 
			m_sourse_curr = 2;
			break;
		case easmSrcAuto: 
			{
				if(PORTC & (1<<PIN_SRC_DETECTOR_1))
					m_sourse_curr = 1;
				else
					m_sourse_curr = 0;
			}				
			break;
	}
	
	if(m_sourse_curr == 0)
		PORTC |= (1<<PIN_SRC_RELAY_0);
	else
		PORTC &= ~(1<<PIN_SRC_RELAY_0);
		
	if(m_sourse_curr == 1)
		PORTC |= (1<<PIN_SRC_RELAY_1);
	else
		PORTC &= ~(1<<PIN_SRC_RELAY_1);

	if(m_sourse_curr == 2)
		PORTC |= (1<<PIN_SRC_RELAY_2);
	else
		PORTC &= ~(1<<PIN_SRC_RELAY_2);
}

void Amp::on_tick()
{
	
}

