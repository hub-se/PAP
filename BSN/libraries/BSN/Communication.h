#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <SoftwareSerial.h>
#include <Arduino.h>

class Communication {
  public:
    static void sendLongData(float data, SoftwareSerial *link);
    static float receiveLongData(SoftwareSerial *link);
};

#endif
