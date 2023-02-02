#include "Statemachine.hpp"

Statemachine::Statemachine(){
  for(byte i = 0; i < 6; i++){
    this->_holds[i] = false;
    this->holds[i] = &(this->_holds[i]);

    this->_reached[i] = false;
    this->reached[i] = &(this->_reached[i]);
  }

  this->resetClock();
  this->nextTimeGuard = 60000;

}

Statemachine::~Statemachine(){
  for(int i = this->states.getSize() - 1; i >= 0; i--){
    delete this->states.getValue(i);
    this->states.remove(i);
    //this->removeState(this->states.getValue(i)->toString());
  }
}

Statemachine::Statemachine(String initialState){
  this->currentState = this->addState(initialState);

  for(byte i = 0; i < 6; i++){
    this->_holds[i] = false;
    this->holds[i] = &(this->_holds[i]);

    this->_reached[i] = false;
    this->reached[i] = &(this->_reached[i]);
  }
  this->resetClock();

}

State* Statemachine::addState(String name){
  State* s = new State(name);
  this->states.add(s);
  return s;
}

Transition* Statemachine::addTransition(State* from, State* to){
  Transition* t = new Transition(from, to);
  from->addOutgoingTransition(t);
  to->addIncomingTransition(t);
  //switchState()?
  return t;
}

State* Statemachine::getState(String name){
  int size = this->states.getSize();
  for(int i = 0; i < size; i++){
    if(this->states.getValue(i)->toString() == name){
      return this->states.getValue(i);
    }
  }
}

State* Statemachine::getCurrentState(){
  return this->currentState;
}

//CurrentStates* Statemachine::getCurrentStates(){
//  return this->currentStates;
//}


void Statemachine::removeState(String name){
  if(name == this->currentState->toString()){
    //TODO now we want to delete the current state
    //so we have to find a replacement
    //we also have to check currentStates and replace those
  }
  int size = this->states.getSize();
  for(int i = 0; i < size; i++){
    if(this->states.getValue(i)->toString() == name){
      //TODO
      // we have to redirect any transition that comes to this state
      // and find if there are unreachable states afterwards
      //this->states.getValue(i)->del();
      //free(this->states.getValue(i));
      delete this->states.getValue(i);
      this->states.remove(i);
    }
  }
}

void Statemachine::setReached(byte i){
  this->_reached[i] = true;
  switchState();
  this->_reached[i] = false;

}

void Statemachine::updateTimeGuard(){
  return;
  //update next time change
  this->nextTimeGuard = 600000;
  //check the next state invariant
  unsigned int potentialNew;
  potentialNew = this->currentState->nextInvariantChange(this->clock);
  if( potentialNew < this->nextTimeGuard){
    this->nextTimeGuard = potentialNew;
  }
  //iterate over all new transitions
  int size = this->currentState->getOutgoingTransitions().getSize();
  for(int i = 0; i < size; i++){
    Transition* t = this->currentState->outgoingTransitions[i];
    if(t->getTimeGuard() != NULL){
      potentialNew = t->getTimeGuard()->getNextChange(this->clock);
      if(potentialNew > 0 && potentialNew < this->nextTimeGuard){
        this->nextTimeGuard = potentialNew;
      }
    }
  }
}

bool Statemachine::switchState(){
  int s = this->currentState->outgoingTransitions.getSize();
  for(int i = 0; i < s; i++){
    Transition* t = this->currentState->outgoingTransitions.getValue(i);
    if(t->isEnabled()){
      //check if new state is allowed
      if(t->doAction()){
        if(t->getTarget()->checkInvariant(0)){
          this->currentState = this->currentState->outgoingTransitions.getValue(i)->getTarget();
          this->resetClock();
        }
        break;
      } else {
        if(t->getTarget()->checkInvariant(this->clock)){
          this->currentState = this->currentState->outgoingTransitions.getValue(i)->getTarget();
        }
        break;
      }

      this->updateTimeGuard();

      return true;
    }
  }
  return false;
  //TODO here we now have to iterate over all current states and compute the new set of current states
}

void Statemachine::setCurrentState(String name){
  int size = this->states.getSize();
  for(int i = 0; i < size; i++){
    if(this->states.getValue(i)->toString() == name){
      this->currentState = this->states.getValue(i);
      return;
    }
  }
}

void Statemachine::updateClock(){
  this->clock = millis() - this->deltaFromMillis;
  //if the clock update has enabled a new time guard
  this->switchState();
  /*if(this->nextTimeGuard <= this->clock){
    //then we check if a transition is enabled
    if(!this->switchState()){
      //if not, we have to update the next time guard
      this->updateTimeGuard();
    }
  }*/
}



void Statemachine::resetClock(){
  this->clock = 0;
  this->deltaFromMillis = millis();
}
