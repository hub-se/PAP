#include "Pulse.h";


void Pulse::init() {
  //state0
  this->transitions[0] = 72;
  this->transitions[1] = 21;
  this->transitions[2] = 4;
  this->transitions[3] = 2;
  this->transitions[4] = 1;
  //state1
  this->transitions[5] = 14;
  this->transitions[6] = 61;
  this->transitions[7] = 19;
  this->transitions[8] = 4;
  this->transitions[9] = 2;
  //state2
  this->transitions[10] = 1;
  this->transitions[11] = 17;
  this->transitions[12] = 60;
  this->transitions[13] = 20;
  this->transitions[14] = 2;
  //state3
  this->transitions[15] = 0;
  this->transitions[16] = 2;
  this->transitions[17] = 15;
  this->transitions[18] = 70;
  this->transitions[19] = 13;
  //state4
  this->transitions[20] = 0;
  this->transitions[21] = 1;
  this->transitions[22] = 2;
  this->transitions[23] = 20;
  this->transitions[24] = 77;

  this->states[0].setLowerBound(0);
  this->states[0].setUpperBound(70);
  this->states[1].setLowerBound(70);
  this->states[1].setUpperBound(85);
  this->states[2].setLowerBound(85);
  this->states[2].setUpperBound(97);
  this->states[3].setLowerBound(97);
  this->states[3].setUpperBound(115);
  this->states[4].setLowerBound(115);
  this->states[4].setUpperBound(300);
}
