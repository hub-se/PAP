#include <Statemachine.hpp>
#include <SoftwareSerial.h>

SoftwareSerial link(10, 9);//Rx = 10

int timebound = 5000; //ms

void setup() {
  Serial.begin(9600);
  Serial.println("Monitor setup");
  link.begin(9600);
  link.listen();
  display_freeram();
}

void loop() {
  
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
