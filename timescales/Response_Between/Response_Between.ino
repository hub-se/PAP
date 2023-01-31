#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx
bool failing = true; 

boolean scopeOpen; 
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
    link.write(2);
    link.write(1);
  }
  int r = random(10); 
  if(!scopeOpen && r <= 5){
    link.write(Q); //Q
    scopeOpen = true; 
  } else {
    link.write(1); //R
    scopeOpen = false; 
  }

  r = random(100);
  if(r < 50){
    link.write(2); //P
    long w = random(5000);
    delay(w);
    link.write(3); // S
  }

  long w = random(2000);
  delay(w);
}
