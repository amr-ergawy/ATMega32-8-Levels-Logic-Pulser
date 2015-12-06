# ATMega32-8-Levels-Logic-Pulser

As an application of the my ATMega32 prototyping board, I developed an 8-levels logic pulse that can be used as a test tool.

It can be used to generate signals of: 1Hz, 2Hz, 4Hz, 8Hz, 242Hz, 484Hz, 986Hz,  and 1,936 kHz. 

I used it to test my ATTiny13 based frequency counter.

Every switch from the 8 configuration switches represents one frequency level. The enabled switch with the highest frequency level overrides the effect of other enabled switches, resulting on generating the output frequency associated with that switch.

Three of the four LEDs are used as indicators. They encode the currently generated frequency in a binary format. When generating the 1st level, 1Hz, the indicator LEDs are 000. When generating the 8th level, 1,936 kHz, the indicator LEDs are 111.
