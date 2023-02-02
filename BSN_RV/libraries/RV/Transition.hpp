#ifndef TRANSITION_HPP
#define TRANSITION_HPP

#include <Arduino.h>
#include <List.hpp>
#include "Invariant.hpp"
class State;

typedef struct {
    unsigned long timestamp;
    uint8_t value;
  }event;

class Transition {
  private:
    State* source;
    State* target;
    boolean* synchronisation;
    List<boolean*> guards;
    boolean __default = true;
    double prob;
    unsigned int* clock; // refers to the clock that is used in temporal guard
    Invariant* tempGuard;
    //unsigned int* tempGuard;
    //bool lowerTimeBound;
    bool action;
  public:
    Transition(State* source, State* target);
    ~Transition();
    static int cnt;
    State* getSource();
    State* getTarget();
    void setSource(State* s);
    void setTarget(State* s);
    void addGuard(boolean* guard);
    void addTimedGuard(Invariant* t, unsigned int* c);
    void setSynchronisation(boolean* sync);
    boolean isEnabled();
    double isEnabledWP();
    void setProb(double prob);
    Invariant* getTimeGuard();
    void setAction(bool a);
    bool doAction();
};

#endif
