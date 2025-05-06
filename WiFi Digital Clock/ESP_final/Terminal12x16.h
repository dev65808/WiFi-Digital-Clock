
#ifndef TERMINAL12X16_H
#define TERMINAL12X16_H

#include <stdint.h>

const uint8_t Terminal12x16[] PROGMEM = {
  0x0C, 0x10, // Width: 12, Height: 16
  0x20,       // First char (space)
  0x60,       // Number of chars (96 total)
  // Placeholder for font data (actual data needs a tool like TheDotFactory)
};

#endif // TERMINAL12X16_H
