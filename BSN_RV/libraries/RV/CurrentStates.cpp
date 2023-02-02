#include "CurrentStates.hpp"

CurrentStates::CurrentStates(){
}
/*
this function is used to set up a new current states object. 
when a state already has a probability, the new one is *ADDED* because
this represents multiple paths that lead to the same state.
*NOT* the probability of staying in that state (which would be a multiplication)
*/
void CurrentStates::updateState(State* state, double prob){
  int s = this->states.getSize();
  for(int i = 0; i < s; i++){
    if(this->states.getValue(i).state == state){
      this->states.getPointer(i)->prob += prob;
      return;
    }
  }
  //if we get here, then the state is currently not in our list
  currentStateWithProb t = {state, prob};
  //TODO check if t is free'd after this function
  this->states.add(t);
}

CurrentStates::~CurrentStates(){
  this->states.clear();
}