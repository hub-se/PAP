#ifndef CURRENTSTATES_HPP
#define CURRENTSTATES_HPP

#include <Arduino.h>
#include <List.hpp>
#include "State.hpp"

typedef struct { 
    State* state;
    double prob;
  }currentStateWithProb;

class CurrentStates {

  

  private:
    List<currentStateWithProb> states;
  public:
    CurrentStates();
    ~CurrentStates();
    void updateState(State* name, double prob);

};

#endif
