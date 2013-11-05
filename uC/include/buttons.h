#ifndef __BUTTONS_H__
#define __BUTTONS_H__

#include <avr/io.h>

#if defined(__AVR_ATtiny45__)

#define BUTTONS_PORT_LETTER B
#define HORN_PIN_NR PB3
#define EARTH_PIN_NR PB4

#elif defined(__AVR_ATmega16A__) || defined(__AVR_ATmega16__)

#define BUTTONS_PORT_LETTER D
#define HORN_PIN_NR PD5
#define EARTH_PIN_NR PD1

#endif

#define BUTTONS_PORT GETPORT(BUTTONS_PORT_LETTER)
#define BUTTONS_PIN GETPIN(BUTTONS_PORT_LETTER)
#define HORN_PIN_MASK (1 << HORN_PIN_NR)
#define EARTH_PIN_MASK (1 << EARTH_PIN_NR)

#include "telephoneData.h"

#include <stdint.h>

void setupButtons(void);
void updateButtons(void);

#endif
