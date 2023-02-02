#include "Statemachine.hpp"

Statemachine s;
const byte numberOfEvents = 6;
const int events[numberOfEvents] = {A0, A1, A2, A3, A4, A5};//scheduler starts, ends, request, pulse, thermometer, glucose
bool eventsHigh[numberOfEvents] = {false, false, false, false, false, false};
const int adaptEvent = 13;

Invariant* iS1;
Invariant* iS2;
Invariant* iS3;


void setup() {
  Serial.begin(9600);
  for(byte i = 0; i < numberOfEvents; i++){
    pinMode(events[i], INPUT);
  }
  setUpSM();
  pinMode(adaptEvent, OUTPUT);
  digitalWrite(adaptEvent, LOW);
  Serial.println("Starting to monitor");
}
void setUpSM(){

  State* closed = s.addState("closed");
  s.setCurrentState("closed");

  State* open = s.addState("open");

  Transition* t = s.addTransition(closed, open);
  t->setSynchronisation(s.reached[0]); //Q, R, S1, S2, S3, ...

  State* s1 = s.addState("s1");
  State* s2 = s.addState("s2");
  State* err = s.addState("ERROR");

  t = s.addTransition(open, s1);
  t->setSynchronisation(s.reached[2]);
  t->setAction(true);

  t = s.addTransition(open, closed);
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s1, err);
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s1, err);
  iS1 = new Invariant(3000, 0);
  t->addTimedGuard(iS1, &s.clock);

  t = s.addTransition(s1, s2);
  t->setSynchronisation(s.reached[3]);

  t = s.addTransition(s2, err);
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s2, err);
  iS2 = new Invariant(3000, 0);
  t->addTimedGuard(iS2, &s.clock);

  t = s.addTransition(s2, open);
  t->setSynchronisation(s.reached[4]);

  Serial.println("Setup done");
}

void loop() {
  while(Serial.available()){
    String x = Serial.readString();
    if(x.equals("add")){
      addSensorNode();
    } else if(x.equals("event")){
      digitalWrite(adaptEvent, HIGH);
    } else if(x.equals("time")){
      adaptTimeBound();
    } else if(x.equals("remove")){
      removeSensorNode();
    } else if(x.equals("separate")){
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
      //s = sm1;
    }
  }

  s.updateClock();
  for(byte i = 0; i < numberOfEvents; i++){
    if(digitalRead(events[i]) == HIGH){
      if(!eventsHigh[i]){
        eventsHigh[i] = true;
        print(i);
      }
    } else if(eventsHigh[i]){
      eventsHigh[i] = false;
    }
  }
}

void adaptTimeBound(){
  iS1->bound = 3000;
  iS2->bound = 3000;
  iS3->bound = 3000;
}

void seperateStatemachines(Statemachine* newSM, byte sensor){
  Serial.println("Adaptation starting, seperating Sensor");

  State* closed = newSM->addState("closed");
  State* open = newSM->addState("open");
  State* s1 = newSM->addState("s1");
  State* err = newSM->addState("ERROR");

  Transition* t = newSM->addTransition(closed, open);
  t->setSynchronisation(newSM->reached[0]);

  t = newSM->addTransition(open, s1);
  t->setSynchronisation(newSM->reached[2]);
  t->setAction(true);


  t = newSM->addTransition(s1, err);
  t->setSynchronisation(newSM->reached[1]);

  t = newSM->addTransition(s1, err);
  Invariant* i = new Invariant(3000, 0);
  t->addTimedGuard(i, &newSM->clock);

  t = newSM->addTransition(s1, open);
  t->setSynchronisation(newSM->reached[3]);

  Serial.println("Adapation finished, sensor seperated");

}

void addSensorNode(){
  Serial.println("Adaptation starting, adding Sensor");

  State* s3 = s.addState("s3");
  State* open = s.getState("open");
  Transition* t = open->incomingTransitions.getValue(1);

  open->removeIncomingTransition(t);
  t->setTarget(s3);
  s3->addIncomingTransition(t);

  t = s.addTransition(s3, open);
  t->setSynchronisation(s.reached[5]);

  t = s.addTransition(s3, s.getState("ERROR"));
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s3, s.getState("ERROR"));
  iS3 = new Invariant(3000, 0);
  t->addTimedGuard(iS3, &s.clock);


  Serial.println("Adapation finished, sensor added");
}

void removeSensorNode(){
  Serial.println("Adaptation starting, removing Sensor");

  State* s2 = s.getState("s2");
  State* s3 = s.getState("s3");
  State* open = s.getState("open");
  Transition* t = s2->outgoingTransitions.getValue(0);
  s3->removeIncomingTransition(t);

  t->setTarget(open);
  open->addIncomingTransition(t);


  State* current = s.getCurrentState();
  if(current == s3){
    s.setCurrentState("open");
  }

  s.removeState("s3");
  free(s3);

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
