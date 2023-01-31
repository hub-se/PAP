#include <Statemachine.hpp>

Statemachine s;
const byte numberOfEvents = 6;
const int events[numberOfEvents] = {}//scheduler starts, ends, request, pulse, thermometer, glucose
bool eventsHigh[numberOfEvents]; 

void setup() {
  for(byte i = 0; i < numberOfEvents; i++){
    pinMode(events[i], INPUT);
  }
  setUpSM();
  Serial.begin(9600);
  Serial.println("Starting to monitor");
}
void setUpSM(){

  State* closed = s.addState("closed");
  s.setCurrentState("closed");

  State* open = s.addState("open");

  Invariant* i;

  //Invariant* i = new Invariant(3000, 3);
  //open->setInvariant(i); //not really necessary here, forces progress towards error state

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
  //t->setAction(true);

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
}

void loop() {
  while(Serial.available()){
    String x = Serial.readString();
    if(x.equals("add")){
      addSensorNode();
    } else if(x.equals("remove")){
      removeSensorNode();
    } else if(x.equals("seperate")){
      Statemachine sm1;
      seperateStatemachines(&sm1, 2);
      if(s.getCurrentState()->toString().equals("closed")){
        sm1.setCurrentState("closed");
        //sm2.setCurrentState("closed");
        //sm3.setCurrentState("closed");
      } else if(s.getCurrentState()->toString().equals("open")){
        sm1.setCurrentState("open");
        //sm2.setCurrentState("open");
        //sm3.setCurrentState("open");
      } else if(s.getCurrentState()->toString().equals("done")){
        sm1.setCurrentState("done");
        //sm2.setCurrentState("done");
        //sm3.setCurrentState("done");
      } else if(s.getCurrentState()->toString().equals("s1")){
        sm1.setCurrentState("done");
        //sm2.setCurrentState("open");
        //sm3.setCurrentState("open");

      } else if(s.getCurrentState()->toString().equals("s3")){
        sm1.setCurrentState("done");
        //sm2.setCurrentState("done");
        //sm3.setCurrentState("open");

      } else if(s.getCurrentState()->toString().equals("ERROR")){
        sm1.setCurrentState("ERROR");
        //sm2.setCurrentState("ERROR");
        //sm3.setCurrentState("ERROR");
      } else {
        Serial.println("Something's wrong with s' current state which is ");
        Serial.println(s.getCurrentState()->toString());
      }
      s = sm1;
    }
  }

  s.updateClock();
  for(byte i = 0; i < numberOfEvents; i++){
    if(digitalRead(events[i]) == HIGH)){
      if(!eventsHigh[i]){
        eventsHigh[i] = true; 
        print(i);
      }
    } else if(eventsHigh[i]){
      eventsHigh[i] = false; 
    }
  }
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

void addSensorNode(){
  Serial.println("Adaptation starting, adding Sensor");
  State* s3 = s.addState("s3");
  State* done = s.getState("done");
  Transition* t = done->incomingTransitions.getValue(0);
  done->removeIncomingTransition(t);
  t->setTarget(s3);
  s3->addIncomingTransition(t);

  t = s.addTransition(s3, done);
  t->setSynchronisation(s.reached[5]);

  t = s.addTransition(s3, s.getState("ERROR"));
  t->setSynchronisation(s.reached[1]);

  //TODO add transition here that goes from newly added state to the error state
  //not checked yet!
  t = s.addTransition(s3, s.getState("ERROR"));
  Invariant* i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s.clock);

  Serial.println("Adapation finished, sensor added");
}

void removeSensorNode(){
  Serial.println("Adaptation starting, removing Sensor");

  State* s1 = s.getState("s1");
  Transition* t = s1->outgoingTransitions.getValue(0);
  s1->removeOutgoingTransition(t);

  t = s.getState("s3")->incomingTransitions.getValue(0);
  t->setSource(s1);
  s1->addOutgoingTransition(t);
  State* s2 = s.getState("s2");
  t = s2->outgoingTransitions.getValue(1);
  s.getState("ERROR")->removeIncomingTransition(t);

  State* current = s.getCurrentState();
  if(current == s2){
    s.setCurrentState("s1");
  }

  s.removeState("s2");
  free(s2);

  Serial.println("Adapation finished, sensor removed");

}

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