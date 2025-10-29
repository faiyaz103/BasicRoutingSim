#ifndef __BASIC_ROUTING_SIM_SENDER_H_
#define __BASIC_ROUTING_SIM_SENDER_H_

#include <omnetpp.h>
#include "packet_m.h"

using namespace omnetpp;
using namespace BasicRoutingSim;

class Sender : public cSimpleModule
{
  private:
    // Parameters
    double timeout;
    int totalPacketsToSend;
    int myAddress;
    int destAddress;

    // State variables
    int sequenceNumber;
    int packetsSent;
    bool isWaitingForAck;
    cMessage *timer;
    SawPacket *currentPacket; // Store the packet we are waiting on

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

  private:
    void sendDataPacket();
};

#endif

