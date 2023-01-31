#include <SoftwareSerial.h>
#include <Message.h>;
#include <Communication.h>


SoftwareSerial pulseSensor(51, 52); // Rx, Tx
const byte pRDY = A0;

SoftwareSerial temperatureSensor(53, 50); // Rx, Tx
const byte tRDY = A8;

SoftwareSerial glucoseeSensor(54, 55); // Rx, Tx
const byte gRDY = A4;

const int adapt = 14;


//SoftwareSerial adaptation(10, 11); //Rx, Tx //used to receive freq from adapt and send the intervals to the adaptation engine


Message pulseM;
Message temperatureM;
Message glucoseM;

byte patientRisk;


//we only receive data <255 here. If this ever changes, we need to use the Communication class
//since we can only transmit ONE byte here!!
void receive(SoftwareSerial *sensor, Message *m) {
  delay(100); //just if the hub is right on time
  //we read two bytes of data here, the first one is the data, then there is the risk
  float data[2];
  for (int i = 0; i < 2; i++) {
    data[i] = sensor->read();
    delay(25);
  }

  m->setData(data[0]);
  m->setRisk(data[1]);
  Serial.print("Data: ");
  Serial.println(data[0]);
  Serial.print("risk: ");
  Serial.println(data[1]);
  adaptation.listen();
}

void evaluateRisk() {
  int patientStatus = (temperatureM.getRisk() + pulseM.getRisk()) / 2;
  if (patientStatus >= 7)
    patientRisk = 4; //very low risk
  else if (patientStatus >= 5)
    patientRisk = 3; //low risk
  else if (patientStatus >= 3)
    patientRisk = 2; //moderate risk
  else if (patientStatus >= 2)
    patientRisk = 1; //critical risk
  else if (patientStatus >= 0)
    patientRisk = 0; //very critical risk
}

void setup() {
  // put your setup code here, to run once:
  pinMode(pRDY, INPUT);
  pinMode(tRDY, INPUT);
  pinMode(gRDY, INPUT);
  pinMode(adapt, INPUT);

  pulseSensor.begin(9600);
  glucoseSensor.begin(9600);
  temperatureSensor.begin(9600);
  Serial.begin(9600);
  Serial.println("Hub starting");
}

void loop() {

  if(digitalRead(adapt) == LOW){
    while(digitalRead(cycle) == LOW);
    float i = 0; 
    //request data

    if(pActive){
      i+=250;
      Communication::sendLongData(i, &pulse)
    }
    if(tActive){
      i+=250;
      Communication::sendLongData(i, &temp)
    }
    if(gActive){
      i+=250;
      Communication::sendLongData(i, &glucose)
    }
   
  }

  

  //receive data
  if (digitalRead(pRDY) == HIGH) {
    pulseSensor.listen();
    Serial.println("receiving data from pulse sensor");
    receive(&pulseSensor, &pulseM);
  }

  //and here we check the temperature sensor
  if (digitalRead(tRDY) == HIGH) {
    //temperatureSensor.flush();
    temperatureSensor.listen();
    Serial.println("receiving data from temp sensor");
    receive(&temperatureSensor, &temperatureM);
  }

  if (digitalRead(gRDY) == HIGH) {
    //temperatureSensor.flush();
    glucoseSensor.listen();
    Serial.println("receiving data from glucose sensor");
    receive(&glucoseSensor, &glucoseM);
  }

  evaluateRisk();
  Serial.print("Risk estimated: ");
  Serial.println(patientRisk);
}
