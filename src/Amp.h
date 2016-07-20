#ifndef _AMP_H
#define _AMP_H


class Amp {	
public:

	enum { easmSrc0, easmSrc1, easmSrc2, easmSrcAuto };

	void init();
	void on_power_pressed();
	void on_mute_pressed();
	void on_source_pressed();
	void on_source_pin();	// TODO

	void on_vol_minus();
	void on_vol_plus(); 
	
	void on_tick(); 

protected:
	void sync_source();
	
protected:
	uint8_t	m_power_on;
	uint8_t	m_speakers_on;
	uint8_t	m_sourse_mode;
	uint8_t	m_sourse_curr;
};

#endif // _AMP_H