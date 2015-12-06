/*
 * ATmega32TestBoard.c
 *
 * Created: 2014-12-18 20:27:54
 * Author: amr
 *
 * This is a simple logic pulser.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay_basic.h>

#include "intializations.h"
#include "controls.h"
#include "configurations.h"

// This approach of toggling the pin, using multipliers and without connecting to OCn, 
// seems to be accurate according to the calculation of the CTC resulting frequencies, 
// using its equation from the uC data-sheet.
// TODO investigate replacing with connecting OCn.
unsigned int pulser_counter = 0x00;

ISR (TIMER2_COMP_vect) {
	pulser_counter += 0x01;
	if (pulser_counter == pulser_cycle_multiplier) {
		PORTD ^= _BV(PD7);
		pulser_counter = 0x00;
	}
}

void reset_pulser_counter(void) {
	pulser_counter = 0x00;
}

// This is a test button, it switches the timers off and then turns on all the LEDs.
// So you can shut off all the LEDs from the emergency button.
// TODO investigate with more complex test logic.
ISR (INT0_vect) {
	// for protection and consistency, switch off every thing.
	switch_off_pulser_timer_and_all_LEDs();
	reset_pulser_counter();
	// control the LEDs based on the required test state.
	int i;
	for (i=0; i<4; i++) {
		switch_LED(i, ON);
	}
}

// This is an emergency button, switch off the pulser timers and LEDs immediately.
ISR (INT1_vect) {
	switch_off_pulser_timer_and_all_LEDs();
	reset_pulser_counter();
}

// Reads and applies the timer pulser configurations.
ISR (INT2_vect) {
	
	// Only when PINA changes, read and apply configurations.
	if ((pulser_configuration ^ PINA) > 0) {
		// For protection and consistency, switch off every thing
		switch_off_pulser_timer_and_all_LEDs();
		reset_pulser_counter();
		
		pulser_configuration = PINA;
		
		// Apply the configurations.
		apply_pulser_configuration();
	}
}

int main(void) {

	initiate_global_settings();

	initiate_pulser_timer();
	
	initiate_io_ports();
	
	initiate_external_interrupts();

	while(1){}

	return 0;
}