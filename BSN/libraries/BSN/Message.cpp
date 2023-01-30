#include "Message.h";

Message::Message() {
  this->data = 0;
  this->header = "";
  this->risk = 0;
  this->batt = 0;
}
void Message::setData(float d) {
  this->data = d;
}
void Message::setRisk(float r) {
  this->risk = r;
}
void Message::setBatt(float b) {
  this->batt = b;
}
//setHeader?

float Message::getData() {
  return this->data;
}
float Message::getBatt() {
  return this->batt;
}
float Message::getRisk() {
  return this->risk;
}
