#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx

bool failing = true; 

const int Q = 0;
const int P = 2;
const int timebound = 1000; //ms

boolean scopeOpen; 

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  link.begin(9600);
  link.listen();
  scopeOpen = false; 
  Serial.println("receiver starting");
}

void loop() {
  //link.write(Q);
  //delay(1000);
  //link.write(2);
  
  int r = random(10); 
  if(r <= 5){
    link.write(Q); //Q
    scopeOpen = true; 
  }

  r = random(timebound-50);
  delay(r);
  if(r > 50){ 
    link.write(P); //P
  }
  delay(timebound-r);

  if(failing){
    delay(250);
    link.write(P);
  }
  
  delay(timebound);
  delay(50);
  r = random(100);
  link.write(P);
}
