#include "Range.h"

Range::Range() {

}
Range::Range(float l, float u) {
  this->lowerBound = l;
  this->upperBound = u;
}
void Range::setUpperBound(float u) {
  this->upperBound = u;
}
void Range::setLowerBound(float l) {
  this->lowerBound = l;
}

float Range::getLowerBound() {
  return this->lowerBound;
}
float Range::getUpperBound() {
  return this->upperBound;
}

boolean Range::inRange(float element) {
  return (element >= lowerBound) && (element <= upperBound);
}

float Range::convert(float new_lb, float new_ub, float input) {
  return ((input - lowerBound) / (upperBound - lowerBound)) *
         (new_ub - new_lb) + new_lb;
}
