#ifndef INVARIANT_HPP
#define INVARIANT_HPP

#include <Arduino.h>

class Invariant {
  private:
    byte op; //<, <=, ==, >=, >

  public:
    unsigned int bound;
    Invariant(unsigned int bound, byte op);
    bool evaluate(unsigned int clock);
    int getNextChange(unsigned int clock);
};

#endif
