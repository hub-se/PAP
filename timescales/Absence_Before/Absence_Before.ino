#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx
bool failing = true; 
const int timebound = 1000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  link.begin(9600);
  link.listen();
  Serial.println("receiver starting");
}

void loop() {
  int r = random(100);
  if(r < 50) {
    delay(timebound);
    if(failing){
      link.write(2);
    }
    link.write(1);
  }
  link.write(2); //P
}
