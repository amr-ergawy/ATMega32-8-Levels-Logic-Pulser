#include "intializations.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#include "controls.h"

void initiate_global_settings(void)	{
	// interrupts at Flash start, ATMega32 data-sheet, page 49.
	GICR |= _BV(IVCE);
	GICR &= ~(_BV(IVSEL));
	
	// disable watch-dog timer, ATMega32 data-sheet, page 44.
	cli();
	WDTCR |= _BV(WDTOE) | _BV(WDE);
	WDTCR = 0x00;
	sei();

	// disable ADC, ATMega32 data-sheet, page 224.
	ADCSRA &= ~(_BV(ADEN));

	// disable the analog comparator, ATMega32 data-sheet, page 206.
	ACSR |= _BV(ACD);
}

void initiate_pulser_timer(void) {
	// for the pulser timer, use CTC mode.
	TCCR2 |= _BV(WGM21);
	TCCR2 &= _BV(WGM20);

	/* 
		For the pulser timer, the compare-match happens depending on the TOP, in OCR2,
		and the prescaler of the clk_IO, in TCCR2. See the uC data-sheet on:
			8-bit Timer/Counter2 with PWM and Asynchronous Operation.
	*/
	OCR2 = 0xFF;
	
	// for the pulser timer, disconnect OCn.
	TCCR2 &= ~(_BV(COM21) | _BV(COM20));

	// apply the initial pulser cycle multiplier timers.
	update_pulser_timer();
}

void initiate_io_ports(void) {
	// enable pull-up resistors.
	SFIOR &= ~(_BV(PUD));

	// configure PORTA:
	DDRA = 0x00;	// all pins are inputs.

	// configure PORTB:
	DDRB = _BV(DDB3);	// all pins are inputs except pin 3.
	PORTB = 0xF3;		// set pin 3 to low, recovering from "reset tri-state"
						// enable internal pull-ups for 0-1, and 4-7.

	// configure PORTC:
	DDRC = 0x00;	// all pins are inputs.
	PORTC = 0xFF;	// enable internal pull-ups for all.

	// configure PORTD:
	DDRD = _BV(DDD4) | _BV(DDD5) | _BV(DDD7);	// all pins are inputs except pins 4 and 5.
	PORTD = _BV(PD0) | _BV(PD1) | _BV(PD6);		// enable internal pull-ups for pins 0-1, and 6.
}

void initiate_external_interrupts(void) {
	// this procedure is recommended in ATMega32 datasheet, section 13.1.2
	// if it is not followed, at least INT2 occurs while setting
	// then, the program counter jumps searching for INT2_vect
	// INT1 and INT2 are enabled as rising edge based
	// INT0 is enabled at any logic change.
	
	cli();

	GICR &= ~(_BV(INT0)) | ~(_BV(INT1)) | ~(_BV(INT2));
	
	MCUCR |= _BV(ISC11) | _BV(ISC10) | _BV(ISC00);
	MCUSR &= ~(_BV(ISC01));
	MCUCSR |= _BV(ISC2); 
	
	GIFR &= ~(_BV(INTF0)) | ~(_BV(INTF1)) | ~(_BV(INTF2));
	
	GICR |= _BV(INT0) | _BV(INT1) | _BV(INT2);
	
	sei();	
}
