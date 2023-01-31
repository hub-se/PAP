#include <SoftwareSerial.h>
#include <Pulse.h>
#include <Markov.h>;
#include <DataGenerator.h>;
#include <Temperature.h>
#include <Communication.h>

//here you can choose the sensor you want
//add new ones simply by adding the classes to the library
//Temperature model;
Pulse model;

//Communication:
SoftwareSerial link(10, 51)//Rx, Tx, receives from bodyhub, sends to bodyhub. After adaptation, it receives from scheduler and sends to bodyhub

//hubRDY is used to show the hub that we want to transmit data, so that it can listen(). Otherwise the data might be lost
const byte hubRDY = 53;

Markov* markov;
DataGenerator* dataGen;


//we calculate the risk by checking the data model, each state corresponds to a risk level
//this has to be adapted though I think, since the states do not match the risk levels
//TODO include a function in the model that calculates the risk level based on current state, i.e. byte calcRisk();
byte calcRisk(float data) {
  for (byte i = 0; i < 5; i++) {
    if (model.states[i].inRange(data))
      return i;
  }
  Serial.print("Invalid data: ");
  Serial.println(data);
  return -1;
}

void setup() {
  model.init();
  markov = new Markov(3, model.transitions, model.states);
  dataGen = new DataGenerator(markov);

  link.begin(9600);
  link.listen();
  Serial.begin(9600);
  pinMode(hubRDY, OUTPUT);
  Serial.println("Sensor starting now!");
}

void loop() {
  while(!link.available());//wait until data is requested 
  float wait; 
  while(link.available()){
    wait = Communication::receiveLongData(&link);
  }
  delay((wait/2);
  digitalWrite(hubRDY, HIGH);
  dataGen->nextState(); 
  float data = dataGen->calculateValue();
  delay(wait/2); 
  hub.write(data);
  digitalWrite(hubRDY, LOW);

  Serial.print("Sent Data: ");
  Serial.println(data);
  Serial.print("Sent Risk: ");
  Serial.println(risk);
}
