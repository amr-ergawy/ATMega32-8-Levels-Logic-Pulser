#include "controls.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void switch_LED(unsigned char led_index, unsigned char required_state) {
	// switch the LED itself.
	switch(led_index){
		case 0:
			switch(required_state){
				case OFF:
					PORTB &= ~(_BV(PB3));
					break;
				case ON:
					PORTB |= _BV(PB3);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 1:
			switch(required_state){
				case OFF:
					PORTD &= ~(_BV(PD5));
					break;
				case ON:
					PORTD |= _BV(PD5);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 2:
			switch(required_state){
				case OFF:
					PORTD &= ~(_BV(PD4));
					break;
				case ON:
					PORTD |= _BV(PD4);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		case 3:
			switch(required_state){
				case OFF:
					PORTD &= ~(_BV(PD7));
					break;
				case ON:
					PORTD |= _BV(PD7);
					break;
				default:
					// do no thing.
					break;
			}
			break;
		default:
			// do no thing.
			break;
	}
}

void update_pulser_timer() {
	// update the timer state.
	// TODO perhaps make it a global state value.
	enum REQUIRED_TIMER_STATE required_timer_state;

	// update the required interrupts.
	cli();
	if (pulser_cycle_multiplier == SWITCH_OFF_PULSER_MULTIPLIER) {
		TIMSK &= ~(_BV(OCIE2));
		required_timer_state = OFF;
	} else {
		TIMSK |= _BV(OCIE2);
		required_timer_state = ON;
	}
	sei();
		
	TCNT2 = 0x00; // reset the counter.
	if (required_timer_state == OFF) {
		TCCR2 &= ~(_BV(CS22) | _BV(CS21) | _BV(CS20));
	} else {
		// using prescaler 1.
		TCCR2 |= _BV(CS20);
		TCCR2 &= ~(_BV(CS22) | _BV(CS21));
	}
}

void update_status_LEDs(int index_configured_frequency) {
	int i;
	for (i=0; i<3; i++) {
		if (IS_BIT_SET(index_configured_frequency, i)) {
			switch_LED(2-i, ON);
		} else {
			switch_LED(2-i, OFF);
		}
	}
}

// TODO turn on and off the pulser indication LEDs.
void apply_pulser_configuration() {
	// if none of the pulser_configuration bits is enabled, 
	// i.e "not set" as all pins of PORTA are externally pulled up,
	// then the pulser timer will just switch_off.
	pulser_cycle_multiplier = SWITCH_OFF_PULSER_MULTIPLIER;
	
	// a set configuration bit overrides the configuration of the bits to its left.
	int configuration_bit_index=7;
	while (configuration_bit_index>=0) {
		// Note that all pins of PORTA are externally pulled up.
		if (!(IS_BIT_SET(pulser_configuration, configuration_bit_index))) {
			pulser_cycle_multiplier = OPTION_PULSE_MULTIPLIER[configuration_bit_index];
			break;
		}
		configuration_bit_index--;
	}
	
	// update the indication LEDs.
	update_status_LEDs(configuration_bit_index);
	
	// update the pulser timer.
	update_pulser_timer();
}

// TODO turn on and off the pulser indication LEDs.
void switch_off_pulser_timer_and_all_LEDs() {
	// Note that all POTRA pins are externally pulled up.
	pulser_configuration = 0xFF;
	pulser_cycle_multiplier = SWITCH_OFF_PULSER_MULTIPLIER;
	apply_pulser_configuration();
	
	// Switch off all LEDs.
	int i;
	for (i=0; i<4; i++) {
		switch_LED(i, OFF);
	}
}