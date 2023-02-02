#include "State.hpp"

State::State(String name){
  this->name = name;
  this->inv = NULL;
  State::cnt++;
}

State::~State(){
  State::cnt--;
  State::del();
  delete this->inv;
}

 int State::cnt = 0;


void State::addIncomingTransition(Transition* t){
  this->incomingTransitions.add(t);
}

void State::addOutgoingTransition(Transition* t){
  this->outgoingTransitions.add(t);
}

void State::removeIncomingTransition(Transition* t){
  int size = this->incomingTransitions.getSize();
  for(int i = 0; i < size; i++){
    if(this->incomingTransitions.getValue(i) == t){
      this->incomingTransitions.remove(i);
    }
  }
}
void State::removeOutgoingTransition(Transition* t){
  int size = this->outgoingTransitions.getSize();
  for(int i = 0; i < size; i++){
    if(this->outgoingTransitions.getValue(i) == t){
      this->outgoingTransitions.remove(i);
    }
  }
}
String State::toString(){
  return this->name;
}
List<Transition*> State::getOutgoingTransitions(){
  return this->outgoingTransitions;
}

void State::del(){
  for(int i = this->incomingTransitions.getSize() -1 ; i >= 0; i--){
    this->incomingTransitions.getValue(i)->getSource()->removeOutgoingTransition(this->incomingTransitions.getValue(i));
    delete this->incomingTransitions.getValue(i);
    //free(this->incomingTransitions.getValue(i));
  }

  for(int i = this->outgoingTransitions.getSize() -1 ; i >= 0; i--){
    this->outgoingTransitions.getValue(i)->getTarget()->removeIncomingTransition(this->outgoingTransitions.getValue(i));
    delete this->outgoingTransitions.getValue(i);
    //free(this->outgoingTransitions.getValue(i));
  }
}

void State::setInvariant(Invariant* inv){
  this->inv = inv;
}

bool State::checkInvariant(unsigned int clock){
  if(this->inv == NULL) return true;
  return this->inv->evaluate(clock);
}

unsigned int State::nextInvariantChange(unsigned int clock){
  if(this->inv == NULL) return 6000;
  return this->inv->getNextChange(clock);
}
