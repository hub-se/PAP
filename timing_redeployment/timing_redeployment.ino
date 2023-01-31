#include <Statemachine.hpp>

Statemachine* s;

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");

/*
  s = new Statemachine();
  State* state = s->addState("name");
  State* state_ = s->addState("name_");
  Transition* t = s->addTransition(state, state_);
  Serial.println(Transition::cnt);

  delete s; 
  Serial.println(Transition::cnt);
*/
  removeSensorNode();

  for(int j = 0; j < 10; j++){
    unsigned long time = millis();
    unsigned long reps = 1000;
    for (unsigned int i = 0; i < reps; i++){
      addSensorNode();
      removeSensorNode();
      switchEvent();
      //Serial.println(State::cnt);
      //Serial.println(Transition::cnt);
    }
    unsigned long t_ = millis();
    Serial.print("Took ");
    Serial.print(t_ - time);
    Serial.print("ms for ");
    Serial.print(reps * 3);
    Serial.println(" redeployment");
    Serial.print("That is ");
    double x = t_-time;
    Serial.print(x / (reps * 3));
    Serial.println("ms per redeployment");
  }
  Serial.println("Finished");


    
}

void loop() {}

void addSensorNode(){
  delete s; 
  s = new Statemachine();
  State* closed = s->addState("closed");
  s->setCurrentState("closed");
  State* open = s->addState("open");
  Invariant* i;
  Transition* t = s->addTransition(closed, open);
  t->setAction(true); //resets clock
  t->setSynchronisation(s->reached[0]); //Q, R, S1, S2, S3, ...
  State* s1 = s->addState("s1");
  State* s2 = s->addState("s2");
  State* done = s->addState("done");
  State* err = s->addState("ERROR");

  t = s->addTransition(open, s1); 
  t->setSynchronisation(s->reached[2]);
  t->setAction(true);
  
  t = s->addTransition(open, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(open, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s1, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, s2); 
  t->setSynchronisation(s->reached[3]);
  t->setAction(true);

  t = s->addTransition(s2, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s2, err);
  i = new Invariant(3000, 1);

  t->addTimedGuard(i, &s->clock);

  State* s3 = s->addState("s3");

  t = s->addTransition(s2, s3); 
  t->setSynchronisation(s->reached[4]);

  t = s->addTransition(s3, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s3, done); 
  t->setSynchronisation(s->reached[5]);

  t = s->addTransition(done, closed); 
  t->setSynchronisation(s->reached[1]);
}

void removeSensorNode(){
  delete s; 
  s = new Statemachine();
  
  State* closed = s->addState("closed");
  s->setCurrentState("closed");
  
  State* open = s->addState("open");

  Invariant* i;
  
  Transition* t = s->addTransition(closed, open);
  t->setAction(true); //resets clock
  t->setSynchronisation(s->reached[0]); //Q, R, S1, S2, S3, ...

  State* s1 = s->addState("s1");
  State* s2 = s->addState("s2");
  State* done = s->addState("done");
  State* err = s->addState("ERROR");

  t = s->addTransition(open, s1); 
  t->setSynchronisation(s->reached[2]);
  t->setAction(true);
  
  t = s->addTransition(open, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(open, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s1, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, s2); 
  t->setSynchronisation(s->reached[3]);
  t->setAction(true);

  t = s->addTransition(s2, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s2, err);
  i = new Invariant(3000, 1);

  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s2, done); 
  t->setSynchronisation(s->reached[4]);

  t = s->addTransition(done, closed); 
  t->setSynchronisation(s->reached[1]);
}

void switchEvent(){
  delete s; 
  s = new Statemachine();
  State* closed = s->addState("closed");
  s->setCurrentState("closed");
  State* open = s->addState("open");
  Invariant* i;
  Transition* t = s->addTransition(closed, open);
  t->setAction(true); //resets clock
  t->setSynchronisation(s->reached[0]); //Q, R, S1, S2, S3, ...
  State* s1 = s->addState("s1");
  State* s2 = s->addState("s2");
  State* done = s->addState("done");
  State* err = s->addState("ERROR");

  t = s->addTransition(open, s1); 
  t->setSynchronisation(s->reached[2]);
  t->setAction(true);
  
  t = s->addTransition(open, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(open, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s1, err);
  i = new Invariant(3000, 0);
  t->addTimedGuard(i, &s->clock);

  t = s->addTransition(s1, s2); 
  t->setSynchronisation(s->reached[3]);
  t->setAction(true);

  t = s->addTransition(s2, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s2, err);
  i = new Invariant(3000, 1);

  t->addTimedGuard(i, &s->clock);

  State* s3 = s->addState("s3");

  t = s->addTransition(s2, s3); 
  t->setSynchronisation(s->reached[4]);

  t = s->addTransition(s3, err); 
  t->setSynchronisation(s->reached[1]);

  t = s->addTransition(s3, done); 
  t->setSynchronisation(s->reached[4]);

  t = s->addTransition(done, closed); 
  t->setSynchronisation(s->reached[1]);
}

