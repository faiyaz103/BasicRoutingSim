#ifndef __BASIC_ROUTING_SIM_RECEIVER_H_
#define __BASIC_ROUTING_SIM_RECEIVER_H_

#include <omnetpp.h>
#include "packet_m.h"

using namespace omnetpp;
using namespace BasicRoutingSim;

class Receiver : public cSimpleModule
{
  private:
    int myAddress;
    int sequenceExpected;
    int packetsReceived;
    void sendAck(int seqNum, int destAddr);

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif

