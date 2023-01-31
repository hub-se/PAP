#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <Arduino.h>
#include "Range.h";

class Temperature {
  public:
    float transitions[25];
    Range states[5];
    void init();
};

#endif
