#include "Temperature.h"


void Temperature::init() {
  //state0
  this->transitions[0] = 25;
  this->transitions[1] = 51;
  this->transitions[2] = 21;
  this->transitions[3] = 3;
  this->transitions[4] = 0;
  //state1
  this->transitions[5] = 5;
  this->transitions[6] = 50;
  this->transitions[7] = 43;
  this->transitions[8] = 2;
  this->transitions[9] = 0;
  //state2
  this->transitions[10] = 0;
  this->transitions[11] = 4;
  this->transitions[12] = 85;
  this->transitions[13] = 11;
  this->transitions[14] = 0;
  //state3
  this->transitions[15] = 0;
  this->transitions[16] = 1;
  this->transitions[17] = 32;
  this->transitions[18] = 67;
  this->transitions[19] = 0;
  //state4
  this->transitions[20] = 0;
  this->transitions[21] = 0;
  this->transitions[22] = 0;
  this->transitions[23] = 0;
  this->transitions[24] = 0;

  this->states[0].setLowerBound(0);
  this->states[0].setUpperBound(31.99);
  this->states[1].setLowerBound(32);
  this->states[1].setUpperBound(35.99);
  this->states[2].setLowerBound(36);
  this->states[2].setUpperBound(37.99);
  this->states[3].setLowerBound(38);
  this->states[3].setUpperBound(40.99);
  this->states[4].setLowerBound(41);
  this->states[4].setUpperBound(50);
}
