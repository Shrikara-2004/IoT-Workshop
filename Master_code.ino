// master  code

#include <Wire.h>
#include <string.h>
void setup() {
  Wire.begin(8);                 // join i2c bus with address #8
  Wire.onRequest(requestEvent);  // register event
}

void loop() {
  delay(100);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent() {
  char* msg = "HELLO ";
  int n = strlen(msg);
  for (int i = 0; i < n; i++) {
    Wire.write(msg[i]);  // respond with message of 6 bytes
  }
  // as expected by master
}