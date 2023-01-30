#ifndef RANGE_H
#define RANGE_H

#include <Arduino.h>

class Range {
  private:
    float lowerBound;
    float upperBound;
  public:
    Range();
    Range(float l, float u);
    void setUpperBound(float u);
    void setLowerBound(float l);
    float getLowerBound();
    float getUpperBound();
    boolean inRange(float elem);
    float convert(float new_lb, float new_ub, float input);
};

#endif
