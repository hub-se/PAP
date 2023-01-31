#include <Statemachine.hpp>
#include <SoftwareSerial.h>



Statemachine s;
SoftwareSerial link(10, 9);//Rx = 10

const int timebound_1 = 1000; //ms
const int timebound_2 = 1000; //ms

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");

  State* closed = s.addState("closed");
  s.setCurrentState("closed");
  
  State* open = s.addState("open");

  Transition* t = s.addTransition(closed, open);
  t->setSynchronisation(s.reached[0]); //Q, R, S1, S2, S3, ...
  t->setAction(true); //resets clock

  State* waiting = s.addState("waiting");
  t = s.addTransition(open, waiting);
  Invariant* i;
  i = new Invariant(timebound_1, 0);
  t->addTimedGuard(i, &s.clock);
  t->setAction(true);

  State* done = s.addState("done");
  t = s.addTransition(waiting, done);
  i = new Invariant(timebound_2, 0);
  t->addTimedGuard(i, &s.clock);

  State* err = s.addState("ERROR");
  t = s.addTransition(waiting, err);
  t->setSynchronisation(s.reached[2]); //Q, R, S1, S2, S3, ...

  Serial.println("Setup done");
  link.begin(9600);
  link.listen();
  display_freeram();
}

void loop() {
  s.updateClock();
  s.switchState();

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
