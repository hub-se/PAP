#ifndef PULSE_H
#define PULSE_H

#include <Arduino.h>
#include "Range.h"

class Pulse {
  public:
    float transitions[25];
    Range states[5];
    void init();
};

#endif
