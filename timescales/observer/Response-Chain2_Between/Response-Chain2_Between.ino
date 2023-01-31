#include <Statemachine.hpp>
#include <SoftwareSerial.h>

Statemachine s;
SoftwareSerial link(10, 9);//Rx = 10

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");

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
  i = new Invariant(5000, 0);
  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s1, err);
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s1, err);
  i = new Invariant(5000, 0);
  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s1, s2);
  t->setSynchronisation(s.reached[3]);
  //t->setAction(true);

  t = s.addTransition(s2, err);
  t->setSynchronisation(s.reached[1]);

  t = s.addTransition(s2, err);
  i = new Invariant(5000, 1);

  t->addTimedGuard(i, &s.clock);

  t = s.addTransition(s2, done);
  t->setSynchronisation(s.reached[4]);

  t = s.addTransition(done, closed);
  t->setSynchronisation(s.reached[1]);

  Serial.println("Setup done");
  link.begin(9600);
  link.listen();
  display_freeram();

}

void loop() {
  s.updateClock();

  if(link.available()){
    byte value = link.read();
    print(value);
  }
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



