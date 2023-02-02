#include "Invariant.hpp"

Invariant::Invariant(unsigned int bound, byte op){
  this->bound = bound;
  this->op = op;
}

bool Invariant::evaluate(unsigned int clock){
  if(this->op == 0){
    return this->bound < clock;
  } else if(this->op == 1){
    return this->bound <= clock;
  } else if(this->op == 2) {
    return this->bound == clock;
  } else if(this->op == 3) {
    return this->bound >= clock;
  } else {
    return this->op > clock;
  }
}

int Invariant::getNextChange(unsigned int clock){
  return this->bound - clock;
}
