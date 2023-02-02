#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <Arduino.h>
#include <List.hpp>
#include "State.hpp"
#include "CurrentStates.hpp"

class Statemachine {
  private:
    State* currentState;
    CurrentStates currentStates;

    bool _holds[6];//P, S, Q, R
    bool _reached[6];//P, S, Q, R

    unsigned long deltaFromMillis;
    unsigned int nextTimeGuard;

    void updateTimeGuard();

   public:
     List<State*> states;

    unsigned int clock;

    bool* holds[6];//P, S, Q, R
    bool* reached[6];//P, S, Q, R
   //to set up
    Statemachine();
    ~Statemachine();
    Statemachine(String initialState);
    State* addState(String name);
    Transition* addTransition(State* from, State* to);
    State* getState(String name);
    State* getCurrentState();
    void setCurrentState(String name);
  //to run
    void removeState(String name);
    void setReached(byte i);
    void setP_holds(boolean P_holds);
    bool switchState();
    void updateClock();
    void resetClock();
};

#endif
