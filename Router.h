#ifndef __BASIC_ROUTING_SIM_ROUTER_H_
#define __BASIC_ROUTING_SIM_ROUTER_H_

#include <omnetpp.h>
#include "packet_m.h"

using namespace omnetpp;
using namespace BasicRoutingSim;

//
// Simple router module.
//
class Router : public cSimpleModule
{
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif

