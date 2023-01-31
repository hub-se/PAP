#include "Markov.h"

Markov::Markov();
Markov::Markov(int initial, float *t, Range *s) {
  this->currentState = initial;
  this->transitions = t;
  this->states = s;
  }
void Markov::setInit(int initial) {
  this->currentState = initial;
}
void Markov::setTrans(float *t) {
  //transitions = t;
}
void Markov::setStates(Range* s){
  this->states = s;
  }
