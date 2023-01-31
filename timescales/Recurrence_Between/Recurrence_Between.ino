#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx

bool failing = true; 

boolean scopeOpen; 
unsigned long timebound = 500; 
const int Q = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  link.begin(9600);
  link.listen();
  scopeOpen = false; 
  Serial.println("receiver starting");
}

void loop() {
  if(failing){
    link.write(Q);
    delay(timebound + 250);
    link.write(1);
  }
  int r = random(10); 
  if(r < 7){
    link.write(2); //P
  } else {
    link.write(1);  //R
    scopeOpen = false; 
  }
  long w = random(timebound);
  delay(w);

  r = random(10);
  if(r < 5){
    link.write(Q); //Q
    scopeOpen = true;
  }
}
