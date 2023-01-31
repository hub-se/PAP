#include <Statemachine.hpp>

Statemachine s;


void setup() {
  // put your setup code here, to run once:
  State* s1 = s.addState("s1");
  s.setCurrentState("s1");
  State* s2 = s.addState("s2");
  State* s3 = s.addState("s3");
  State* s4 = s.addState("s4");
  State* s5 = s.addState("s5");

  addTransitions(s1);
  addTransitions(s2);
  addTransitions(s3);
  addTransitions(s4);
  addTransitions(s5);
  Serial.begin(9600);
  Serial.println("Starting");
  for(int j = 0; j < 10; j++){
    unsigned long t = millis();
    unsigned long reps = 50000;
    for (unsigned int i = 0; i < reps; i++){
      //random(5); 
      s.setReached(random(5));
    }
    unsigned long t_ = millis();
    Serial.println("Finished");
    Serial.print("Took ");
    Serial.print(t_ - t);
    Serial.print("ms for ");
    Serial.print(reps);
    Serial.println(" events");
    Serial.print("That is ");
    double x = t_-t;
    Serial.print(x / reps);
    Serial.println("ms per event");
  }
  Serial.println("Finished");

  
}
void addTransitions(State* state){
  Transition* t = s.addTransition(state, s.getState("s1"));
  t->setSynchronisation(s.reached[0]); 

  t = s.addTransition(state, s.getState("s2"));
  t->setSynchronisation(s.reached[1]); 

  t = s.addTransition(state, s.getState("s3"));
  t->setSynchronisation(s.reached[2]); 

  t = s.addTransition(state, s.getState("s4"));
  t->setSynchronisation(s.reached[3]); 

  t = s.addTransition(state, s.getState("s5"));
  t->setSynchronisation(s.reached[4]); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
