#include "mc_serial.h"
#include <Arduino.h>


MCSerial mcSerial;

size_t  MCSerial::write(uint8_t val)  {
  Serial.write(val);
  Debug.write(val);
  return 1;
}
