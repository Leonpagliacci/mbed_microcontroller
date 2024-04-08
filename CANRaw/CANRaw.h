#include "mbed.h"
#ifndef CANRAW_H
#define CANRAW_H

class CANRaw : public CAN {
    public:
        CANRaw (PinName rd, PinName td):CAN(rd, td){};
        CANRaw (PinName rd, PinName td, int hz):CAN(rd, td, hz){};
    protected:
    //STUBS
    virtual void lock(){}
    virtual void unlock(){}
};

#endif    // CANRAW_H

