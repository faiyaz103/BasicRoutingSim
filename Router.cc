#include "Router.h"

Define_Module(Router);

void Router::initialize()
{
    EV << "Router initialized." << endl;
}

void Router::handleMessage(cMessage *msg)
{
    SawPacket *pkt = check_and_cast<SawPacket *>(msg);

    int destAddr = pkt->getDestAddress();
    int arrivalGate = pkt->getArrivalGate()->getIndex();

    EV << "Router: Packet from " << pkt->getSrcAddress() << " for " << destAddr << " arrived on gate " << arrivalGate << endl;

    // Simple routing table:
    // Address 1 is on gate 0
    // Address 2 is on gate 1
    // Address 3 is on gate 2
    // Address 4 is on gate 3
    // We assume network.ned connects them in this order.

    int targetGate = -1;

    if (destAddr >= 1 && destAddr <= 4) {
        targetGate = destAddr - 1; // Address 1 -> gate 0, etc.
    }

    if (targetGate != -1 && targetGate != arrivalGate) {
        EV << "Forwarding packet to gate " << targetGate << endl;
        send(pkt, "out", targetGate);
    }
    else {
        EV << "Unknown or loopback address. Discarding packet." << endl;
        delete pkt;
    }
}
