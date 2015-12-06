// All timer and LED controls are put in controls.c

#ifndef _CONTROLS_H_
#define _CONTROLS_H_

// configurations of pulser multiplier.
extern const unsigned int SWITCH_OFF_PULSER_MULTIPLIER;
extern const unsigned int OPTION_PULSE_MULTIPLIER[8];
extern unsigned int pulser_cycle_multiplier;

/*	Initially all configurations pins are turned off.
	Note that PORTA is externally pulled-up.	*/
extern unsigned char previous_PINA;
extern unsigned char pulser_configuration;

// possible required timer or LED state.
enum REQUIRED_TIMER_STATE {OFF, ON};

// a utility check macro to check if a bit is set.
#define IS_BIT_SET(value, pos) (value & (1<<pos))

// switches a status or pulser LED.
void switch_LED(unsigned char led_index, unsigned char required_state);

void update_pulser_timer();

void apply_pulser_configuration();

void switch_off_pulser_timer_and_all_LEDs();

#endif