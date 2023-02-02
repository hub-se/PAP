#include "Transition.hpp"

Transition::Transition(State* source, State* target){
  this->source = source;
  this->target = target;
  this->synchronisation = NULL;
  this->tempGuard = NULL;
  this->action = false;
  Transition::cnt++;
}

Transition::~Transition(){
  Transition::cnt--;
  delete this->tempGuard;
}

 int Transition::cnt = 0;

State* Transition::getSource(){
  return this->source;
}
State* Transition::getTarget(){
  return this->target;
}
boolean Transition::isEnabled(){
  if(this->synchronisation != NULL){
    if(!*(this->synchronisation)){
      return false;
    }
  }

  int size = this->guards.getSize();
  for(int i = 0; i < size; i++){
    if(!*(this->guards.getValue(i))){
      return false;
    }
  }

  if(this->tempGuard != NULL){
    return this->tempGuard->evaluate(*this->clock);
  }

  return true;
}
  /*
  if(*(this->synchronisation)){
    int size = this->guards.getSize();
    for(int i = 0; i < size; i++){
      if(!*(this->guards.getValue(i))){
        return false;
      }
    }
    if(this->tempGuard != NULL){
      return this->tempGuard->evaluate(*this->clock);
    }
    return true;
  }
  return false;
}*/

/*double Transition::isEnabledWP(){
  if(*(this->synchronisation)){
    int size = this->guards.getSize();
    for(int i = 0; i < size; i++){
      if(!*(this->guards.getValue(i))){
        return 0;
      }
    }
    return this->prob;

  }
  return 0;
}*/

void Transition::addGuard(boolean* guard){
  this->guards.add(guard);
}

void Transition::addTimedGuard(Invariant* t, unsigned int* c){
  this->tempGuard = t;
  this->clock = c;
}

void Transition::setProb(double prob){
  this->prob = prob;
}

void Transition::setSynchronisation(boolean* sync){
  this->synchronisation = sync;
}

void Transition::setTarget(State* s){
  this->target = s;
}
void Transition::setSource(State* s){
  this->source = s;
}

Invariant* Transition::getTimeGuard(){
  return this->tempGuard;
}


void Transition::setAction(bool action){
  this->action = action;
}

bool Transition::doAction(){
  return this->action;
}
