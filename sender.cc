#include "sender.h"

Define_Module(Sender);

void Sender::initialize()
{
    // Get parameters
    timeout = par("timeout");
    totalPacketsToSend = par("totalPacketsToSend");
    myAddress = par("myAddress");
    destAddress = par("destAddress");

    // Initialize state
    sequenceNumber = 0;
    packetsSent = 0;
    isWaitingForAck = false;
    timer = new cMessage("saw-timer");
    currentPacket = nullptr;

    EV << "Sender " << myAddress << " initialized, sending to " << destAddress << "." << endl;

    // Start sending the first packet
    sendDataPacket();
}

void Sender::sendDataPacket()
{
    if (packetsSent >= totalPacketsToSend) {
        EV << "All " << totalPacketsToSend << " packets sent." << endl;
        return;
    }

    // Create and send a data packet
    std::string payload = "Packet " + std::to_string(packetsSent) + " from " + std::to_string(myAddress);
    currentPacket = new SawPacket("data");
    currentPacket->setKind(DATA_PACKET);
    currentPacket->setSequenceNumber(sequenceNumber);
    currentPacket->setPayload(payload.c_str());
    currentPacket->setSrcAddress(myAddress);
    currentPacket->setDestAddress(destAddress);

    EV << "Sending DATA (seq=" << sequenceNumber << ") to " << destAddress << endl;
    send(currentPacket->dup(), "out"); // Send a copy
    isWaitingForAck = true;
    scheduleAt(simTime() + timeout, timer);
}

void Sender::handleMessage(cMessage *msg)
{
    // Check for corrupted packets
    cPacket *pkt = dynamic_cast<cPacket *>(msg);
    if (pkt && pkt->hasBitError()) {
        EV << "Corrupted packet received. Discarding." << endl;
        delete pkt;
        return;
    }

    if (msg == timer) {
        // Timeout occurred
        if (isWaitingForAck) {
            EV << "Timeout! Resending DATA (seq=" << sequenceNumber << ")" << endl;
            send(currentPacket->dup(), "out"); // Resend a copy
            scheduleAt(simTime() + timeout, timer);
        }
    }
    else {
        // Must be an ACK packet
        SawPacket *ack = check_and_cast<SawPacket *>(msg);

        // Check if it's for us
        if (ack->getDestAddress() != myAddress) {
            EV << "ACK not for me. Discarding." << endl;
            delete ack;
            return;
        }

        if (ack->getKind() == ACK_PACKET && isWaitingForAck) {
            if (ack->getSequenceNumber() == sequenceNumber) {
                // Correct ACK received
                EV << "Correct ACK (seq=" << ack->getSequenceNumber() << ") received from " << ack->getSrcAddress() << endl;

                isWaitingForAck = false;
                packetsSent++;
                sequenceNumber = 1 - sequenceNumber; // Flip sequence
                cancelEvent(timer);

                // Send the next packet
                delete currentPacket; // We are done with the original
                currentPacket = nullptr;
                sendDataPacket();
            }
            else {
                // Duplicate or wrong ACK
                EV << "Duplicate/Wrong ACK (seq=" << ack->getSequenceNumber() << ") received. Ignoring." << endl;
            }
        }
        delete ack; // We are done with the ACK
    }
}

void Sender::finish()
{
    EV << "Sender " << myAddress << " simulation finished. Total packets sent: " << packetsSent << endl;
    if (timer->isScheduled()) {
        cancelEvent(timer);
    }
    delete timer;
    if (currentPacket) {
        delete currentPacket;
    }
}
