#include "DataGenerator.h";

DataGenerator::DataGenerator(Markov* m) {
  this->markovChain = m;
}

int DataGenerator::nextState() {
  int randomNumber = random(101);
  //float randomNumber = 26;
  int offset = markovChain->currentState * 5;
//this next part is weird, since there should be higher values for transitions into higher states,
//but the data provided for temp does not do that. 
//there can't be a state other than s \in {0, 1} reached 
  if (randomNumber <= markovChain->transitions[offset]) {
    markovChain->currentState = 0;
  }
  else if (randomNumber <= markovChain->transitions[offset + 1]) {
    markovChain->currentState = 1;
  }
  else if (randomNumber <= markovChain->transitions[offset + 2]) {
    markovChain->currentState = 2;
  }
  else if (randomNumber <= markovChain->transitions[offset + 3]) {
    markovChain->currentState = 3;
  }
  else if (randomNumber <= markovChain->transitions[offset + 4]) {
    markovChain->currentState = 4;
  }
  else return -1;
  return randomNumber;
}

float DataGenerator::calculateValue() { //a.k.a. getValue
  Range range = markovChain->states[markovChain->currentState];
  int rnd = random(range.getLowerBound()*100, range.getUpperBound()*100+1);
  return ((float) rnd )/100;//heavy adjustments here, hope this works
}
