#include <SoftwareSerial.h>

SoftwareSerial link(10, 6); // Rx, Tx
bool failing = true; 


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  link.begin(9600);
  link.listen();
  Serial.println("receiver starting");
}

void loop() {


//response pattern
  link.write(2); //P
  if(failing){
    delay(5000);
  }

  long w = random(5000);
  delay(w);

  link.write(3); // S
}
