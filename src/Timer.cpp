#include "defs.h"
#include "Timer.h"
#include "Amp.h"
#include "ir.h"

#define TIMER_COMP_VAL 80	// 8000000/80 == 100 us

MyTimer glTimer;
extern Amp gAmp;
extern Ir gIr;

void MyTimer::init()
{
		// no prescaler
	    TCCR0B &= (~((1<<CS01)|(1<<CS02)));
	    TCCR0B |= (1 << CS00);
	    
	    // Timer Mode CTC
	    TCCR0A |= (1 << WGM01);
	    // count to
	    OCR0A = TIMER_COMP_VAL;
	    TIMSK0 |= (1 << OCIE0A);
}


ISR(TIMER0_COMPA_vect)
{
	gIr.on_tick();
	gAmp.on_tick();
}
