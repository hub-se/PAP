#include <SoftwareSerial.h>
#include "Communication.h"
SoftwareSerial pulse(50, 12); //Rx, Tx
SoftwareSerial temp(51, 11);
SoftwareSerial glucose(52,10); //Rx, Tx
SoftwareSerial hub(53, 9); //Rx, Tx

const int CYCLE = 13;
const int noCycle = 8;
const int adapt = A5;

//this is the actual frq of the modules
//this value is determiend by the central hub
float actual[3] = {0, 0.5, 0.5};
//these values can be used to shut down a component
//right now they are used to reference if a component is online or offline
bool tActive = true;
const int tPower;

bool pActive = true;
const int pPower = 2;

bool gActive = true;
const int gPower;

//these are the parameters for the adaptation
//the goal
float setpoint = 1;
//float offset = 0.25;
//the stepsize
float gain = 0.0001;
//and the tolerance we allow +/- the setpoint
float tolerance = 0.15;

//functionality of the cost engine

//the strategy features the frq of the components, first the hub, then the pulse sensor, then the temp sensor
float strategy[3] = {0, 0.5, 0.5};
//here we save the last strategy for comparison purpose
float oldStrategy[] = {0, 0.5, 0.5};
float calculate_qos(float s[3]) {
  float a = 0;
  float b = 0;
  if (pActive)
    //we derive how much the actual strategy differs from the calculated strategy
    //and multiply this with the new strategy
    a = (oldStrategy[1] / actual[1]) * s[1];
  if (tActive)
    b = (oldStrategy[2] / actual[2]) * s[2];
  //if both sensors are offline, we can't find a good strategy
  if (!pActive && !tActive) return -1;
  return a + b;
}
/*
void plan() {
  //float c_curr = calculate_qos(actual);
  float c_curr = calculate_qos(strategy);
  float error = setpoint - c_curr;

  byte sensor_num = 0;
  if (pActive) sensor_num++;
  if (tActive) sensor_num++;

  //Divide error and setpoint by the number of sensors
  error /= sensor_num;
  setpoint /= sensor_num;
  //this is pretty similar to the ROS-BSN
  float c_new = c_curr; // set offset
  c_new /= sensor_num;
  float prev[3];
  float c_prev = 0;
  if (error > 0 ) {
    do {
      //here we just loop over all components at the same time
      for (byte a = 0; a < 3; a++) prev[a] = strategy[a];
      c_prev = c_new;
      //for each sensor we set the new strategy
      for (byte i = 0; i < 3; i++) {
        if (i != 0) {
          strategy[i] += gain * error;
        } else {
          strategy[i] = 0;
        }
      }
      //we then check if the new strategy is adequate
      c_new = calculate_qos(strategy);
      c_new /= sensor_num;
    } while (c_new < setpoint  && c_prev < c_new && strategy[1] > 0 && strategy[2] > 0);
  } else if (error < 0) {
    do {
      for (byte a = 0; a < 3; a++) prev[a] = strategy[a];
      c_prev = c_new;
      //for each sensor we set the new strategy
      for (byte i = 0; i < 3; i++) {
        if (i != 0) {
          strategy[i] += gain * error;
        } else {
          strategy[i] = 0;
        }
      }
      //we then check if the new strategy is adequate
      c_new = calculate_qos(strategy);
      c_new /= sensor_num;
    } while (c_new > setpoint  && c_prev > c_new && strategy[1] > 0 && strategy[2] > 0);
  }

  //since we overshot a little bit, here we go one step back again
  for (byte a = 0; a < 3; a++) strategy[a] = prev[a];
  c_new = calculate_qos(strategy);
  c_new /= sensor_num;

  //now we optimize again for every component itself
  for (byte i = 1; i < 3; i++) {
    if (error > 0 ) {
      do {
        for (byte a = 0; a < 3; a++) prev[a] = strategy[a];
        c_prev = c_new;
        //for each sensor we set the new strategy

        if (i != 0) {
          strategy[i] += gain * error;
        } else {
          strategy[i] = 0;
        }

        //we then check if the new strategy is adequate
        c_new = calculate_qos(strategy);
        c_new /= sensor_num;
      } while (c_new < setpoint  && c_prev < c_new && strategy[i] > 0);
    } else if (error < 0) {
      do {
        for (byte a = 0; a < 3; a++) prev[a] = strategy[a];
        c_prev = c_new;
        //for each sensor we set the new strategy

        if (i != 0) {
          strategy[i] += gain * error;
        } else {
          strategy[i] = 0;
        }

        //we then check if the new strategy is adequate
        c_new = calculate_qos(strategy);
        c_new /= sensor_num;
      } while (c_new > setpoint  && c_prev > c_new && strategy[i] > 0 );
    }
  }


  //since we overshot a little bit, here we go one step back again
  for (byte a = 0; a < 3; a++) strategy[a] = prev[a];
  c_new = calculate_qos(strategy);
  c_new /= sensor_num;

  //obviously we can't have a negative frq at any point
  bool negative_cost = false;
  for (byte j = 1; j < 3; j++)
    //if(strategy[j] is cost
    if (strategy[j] < 0 ) {
      negative_cost = true;
      break;
    }
  //if there is no negative frq in the strategy, it's okay to use it
  if (negative_cost) {
    Serial.println("We have a negative frq in our strategy.. :/");
    for (byte a = 0; a < 3; a++) strategy[a] = oldStrategy[a];
  }
  else {
    Serial.println("New strategy was found:");
    Serial.println(strategy[0]);
    Serial.println(strategy[1]);
    Serial.println(strategy[2]);
    execute();
    for (byte a = 0; a < 3; a++) {
      oldStrategy[a] += strategy[a];
      oldStrategy[a] /= 2;
    }
    //for (byte a = 0; a < 3; a++) actual[a] = strategy[a] + 100;

  }
  setpoint *= sensor_num;
}
*/



/**
  this is used to transmit the strategy to the components
  in ROS this is done by the enactor
  we only publish strategies that are >150, otherwise we might mess up a component
  so there may be problems if a stretegy features a strategy <150!! TODO
*/



void setup() {
  pinMode(CYCLE, OUTPUT);
  pinMode(noCycle, OUTPUT);
  digitalWrite(CYCLE, LOW);
  digitalWrite(noCycle, HIGH);
  pinMode(adapt, INPUT);
  pinMode(pPower, INPUT);
  pinMode(tPower, INPUT);
  pinMode(gPower, INPUT);
  /*if(digitalRead(pPower) == HIGH)
    pActive = true;
  else
    pActive = false;
  if(digitalRead(tPower) == HIGH)
    tActive = true;
  else
    tActive = false;
  if(digitalRead(gPower) == HIGH)
    gActive = true;
  else
    gActive = false;
*/
  temp.begin(9600);
  pulse.begin(9600);
  glucose.begin(9600);
  hub.begin(9600);
  Serial.begin(9600);
  Serial.println("Adaptation Engine starting");
  hub.listen();
}

void afterAdapt() {
  delay(1000);
  //scehduler cycle starts
  digitalWrite(noCycle, LOW);
  digitalWrite(CYCLE, HIGH);
  float i = 0;
  if(pActive){
    i+=250;
    Communication::sendLongData(i, &pulse);
    Serial.println("Temp sensor requested");
  }
  if(tActive){
    i+=250;
    Communication::sendLongData(i, &temp);
  }
  if(gActive){
    i+=250;
    Communication::sendLongData(i, &glucose);
  }
  delay((int) i + 500);
  digitalWrite(CYCLE, LOW);
  digitalWrite(noCycle, HIGH);
}

void loop(){
  delay(1000);
  digitalWrite(noCycle, LOW);
  digitalWrite(CYCLE, HIGH);
  delay(2000);
  digitalWrite(CYCLE, LOW);
  digitalWrite(noCycle, HIGH);
  if(digitalRead(adapt) == HIGH){
    Serial.println("Adapation noticed");
    while(true) afterAdapt();
  }
}
