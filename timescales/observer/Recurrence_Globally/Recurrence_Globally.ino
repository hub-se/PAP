#include <Statemachine.hpp>
#include <SoftwareSerial.h>

Statemachine s;
SoftwareSerial link(10, 9);//Rx = 10

int timebound = 500; //ms

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");
  Transition* t;
  Invariant* i;


  State* open = s.addState("open");
  s.setCurrentState("open");

  t = s.addTransition(open, open);
  t->setSynchronisation(s.reached[2]); //Q, R, S1, S2, S3, ...
  t->setAction(true); //resets clock

  State* err = s.addState("ERROR");
  t = s.addTransition(open, err);
  i = new Invariant(timebound, 0);
  t->addTimedGuard(i, &s.clock);

  Serial.println("Setup done");
  link.begin(9600);
  link.listen();
  display_freeram();
  s.resetClock();
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
