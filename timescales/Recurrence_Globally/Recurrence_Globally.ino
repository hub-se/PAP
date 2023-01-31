#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx
bool failing = true; 

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
  if(!failing){
    link.write(2); 
  } else {
    link.write(1); //just to get logging from the observer
  }
  int w = random(500);
  delay(w);
}
