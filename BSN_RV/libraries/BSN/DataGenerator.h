#ifndef DATAGENERATOR_H
#define DATAGENERATOR_H

#include <Arduino.h>
#include "Markov.h";

class DataGenerator {
  private:
    
  public:
    DataGenerator(Markov* m);
    int nextState();
    float DataGenerator::calculateValue();
    Markov* markovChain;
};

#endif
