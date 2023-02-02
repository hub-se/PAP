#include "Communication.h"

union share {
    float data;
    struct {
      byte b[4];
    } bytes;
  };

static void Communication::sendLongData(float data, SoftwareSerial *link) {

  union share st;
  st.data = data;
  for (byte i = 0; i < 4; i++) {
    link->write(st.bytes.b[i]);
  }
}
static float Communication::receiveLongData(SoftwareSerial *link) {

  union share st;
  for (byte i = 0; i < 4; i++) {
    st.bytes.b[i] = link->read();
    delay(50);
  }
  return st.data;
}
