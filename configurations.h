#ifndef _CONFIGURATIONS_H_
#define _CONFIGURATIONS_H_

/*	
	initially all configuration pins are turned off.
	note that PORTA is externally pulled-up.
*/
unsigned char pulser_configuration = 0xFF;

// configurations of pulser multiplier.
const unsigned int SWITCH_OFF_PULSER_MULTIPLIER = 0;
// a multiplier value, m, is calculated given:
// f = the resulting CTC frequency from the configured clock and prescaler.
// 1950 = f/1, toggling the pulser pin, giving a single pulse,
// 1 = f/f
// ((1950/2 ~= 975) /2 ~= 485) /2 ~= 240, 
// approximating to smaller values to ensure the target frequency within a second.
// ((((1*2) = 2) *2) = 4) *2 = 8.
const unsigned int OPTION_PULSE_MULTIPLIER[8] = {1950 , 975, 488, 244, 8, 4, 2, 1};
unsigned int pulser_cycle_multiplier = 0;

#endif
