#ifndef MCSerial_h
#define MCSerial_h

#include <Print.h>
#include "Arduino.h"
#include <RemoteDebug.h> 

extern RemoteDebug Debug;

class MCSerial: public Print{
    using Print::Print; 
    public: 
    size_t write(uint8_t) override;
};

extern MCSerial mcSerial;
#endif


