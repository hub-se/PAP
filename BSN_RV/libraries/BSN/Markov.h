#ifndef MARKOV_H
#define MARKOV_H

#include <Arduino.h>
#include "Range.h"

class Markov {
  private:

  public:
    Markov();
    Markov(int initial, float *t, Range *s);
    void setInit(int initial);
    void setTrans(float *t);
    void setStates(Range* s);
    int currentState;
    float *transitions;
    Range *states;
};

#endif
