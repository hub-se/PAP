#ifndef MESSAGE_H
#define MESSAGE_H

#include <Arduino.h>

class Message {
  private:
    float data;
    String header;
    float risk;
    float batt;
  public:
    Message();
    void setData(float d);
    void setRisk(float r);
    void setBatt(float b);
    //setHeader?

    float getData();
    float getBatt();
    float getRisk();
};

#endif
