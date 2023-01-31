#include <Statemachine.hpp>
#include <SoftwareSerial.h>

Statemachine s;
SoftwareSerial link(10, 9);//Rx = 10

Transition* s2_s3; //s2_done if s3 is not there
Transition* s3_done; 
Transition* s3_error;

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");

  State* closed = s.addState("closed");
  s.setCurrentState("closed");
  
  State* open = s.addState("open");

  Invariant* i;
  
  Transition* t = s.addTransition(closed, open);
  t->setAction(true); //resets clock
  t->setSynchronisation(s.reached[0]); //Q, R, S1, S2, S3, ...

  State* s1 = s.addState("s1");
  State* s2 = s.addState("s2");
  State* done = s.addState("done");
  State* err = s.addState("ERROR");

  t = s.addTransition(open, s1); 
  t->setSynchronisation(s.reached[2]);
  t->setAction(true);
  
  t = s.addTransition(open, err); 
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(open, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s1, err); 
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s1, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s1, s2); 
  t->setSynchronisation(s.reached[3]);
  t->setAction(true);

  t = s.addTransition(s2, err); 
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s2, err);
  i = new Invariant(3000, 1);

  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s2, done); 
  t->setSynchronisation(s.reached[4]);

  t = s.addTransition(done, closed); 
  t->setSynchronisation(s.reached[1]);

  Serial.println("Setup done");
  display_freeram();

/*
  addSensorNode("1");
  removeSensorNode("1");
  Serial.println(s.states.getSize());
  addSensorNode("2");
  Serial.println(State::cnt);

  removeSensorNode("2");
  Serial.println(State::cnt);
*/
  /*
  for(int i =0; i < s1->incomingTransitions.getSize(); i++){
    Serial.println(i+1);
  }*/


  unsigned long time = millis();
  unsigned long reps = 500;
  for (unsigned int i = 0; i < reps; i++){
    addSensorNode();
    removeSensorNode();
  }
  unsigned long t_ = millis();
  Serial.println("Finished");
  Serial.print("Took ");
  Serial.print(t_ - time);
  Serial.print("ms for ");
  Serial.print(reps * 2);
  Serial.println(" adaptations");
  Serial.print("That is ");
  double x = t_-time;
  Serial.print(x / (reps * 2));
  Serial.println("ms per adaptation");

    
}

void loop() {delay(5000);}

void print(byte value){
  Serial.print("Value: ");
  Serial.println(value);
  Serial.print("Clock: ");
  Serial.println(s.clock);
  Serial.print("State: ");
  Serial.println(s.getCurrentState()->toString());
  s.setReached(value);
  Serial.print("State: ");
  Serial.println(s.getCurrentState()->toString());

  Serial.print("Clock: ");
  Serial.println(s.clock);
}
//add s3
void addSensorNode(){
  //Serial.println("Adaptation starting, adding Sensor");

  State* s3 = s.addState("s3");

  State* done = s.getState("done");
  Transition* t = done->incomingTransitions.getValue(0);
  done->removeIncomingTransition(t);
  t->setTarget(s3);
  s2_s3 = t; 
  s3->addIncomingTransition(t);

  t = s.addTransition(s3, done); 
  s3_done = t; 
  t->setSynchronisation(s.reached[5]);

  t = s.addTransition(s3, s.getState("ERROR")); 
  s3_error = t; 
  t->setSynchronisation(s.reached[1]);

  //Serial.println("Adapation finished, sensor added");
}
//remove s3
void removeSensorNode(){
  //Serial.println("Adaptation starting, removing Sensor");

  State* s2 = s.getState("s2");
  State* s3 = s.getState("s3");
  State* done = s.getState("done");

  s3->removeIncomingTransition(s2_s3);
  done->addIncomingTransition(s2_s3);


  State* current = s.getCurrentState();
  if(current == s3){
    s.setCurrentState("done");
  }
  s.removeState("s3");
  //delete s3;
  //Serial.println("Adapation finished, sensor removed");
}

void seperateStatemachines(Statemachine* newSM, byte sensor){
  Serial.println("Adaptation starting, seperating Sensor");

  State* closed = newSM->addState("closed");
  State* open = newSM->addState("open");
  State* done = newSM->addState("done");
  State* err = newSM->addState("ERROR");

  Transition* t = newSM->addTransition(closed, open); 
  t->setSynchronisation(newSM->reached[0]);
  t->setAction(true);

  t = newSM->addTransition(open, done); 
  t->setSynchronisation(newSM->reached[2]);

  t = newSM->addTransition(open, err); 
  t->setSynchronisation(newSM->reached[1]);

  t = newSM->addTransition(done, closed); 
  t->setSynchronisation(newSM->reached[1]);

  t = newSM->addTransition(open, err);
  Invariant* i = new Invariant(3000, 0);
  t->addTimedGuard(i, &newSM->clock);

  Serial.println("Adapation finished, sensor seperated");

}

void display_freeram() {
  Serial.print(F("- SRAM left: "));
  Serial.println(freeRam());
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0  
    ? (int)&__heap_start : (int) __brkval);  
}


