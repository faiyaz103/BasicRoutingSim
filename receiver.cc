#include "receiver.h"

Define_Module(Receiver);

void Receiver::initialize()
{
    myAddress = par("myAddress");
    sequenceExpected = 0;
    packetsReceived = 0;
    EV << "Receiver " << myAddress << " initialized, waiting for seq 0." << endl;
}

void Receiver::sendAck(int seqNum, int destAddr)
{
    SawPacket *ack = new SawPacket("ack");
    ack->setKind(ACK_PACKET);
    ack->setSequenceNumber(seqNum);
    ack->setSrcAddress(myAddress);
    ack->setDestAddress(destAddr);
    EV << "Sending ACK (seq=" << seqNum << ") to " << destAddr << endl;
    send(ack, "out");
}

void Receiver::handleMessage(cMessage *msg)
{
    // Check for self-messages
    if (msg->isSelfMessage()) {
        delete msg;
        return;
    }

    // Check for corrupted packets
    cPacket *pkt = dynamic_cast<cPacket *>(msg);
    if (pkt && pkt->hasBitError()) {
        EV << "Corrupted DATA packet received. Discarding." << endl;
        delete pkt;
        return;
    }

    // We only expect DATA packets
    SawPacket *data = check_and_cast<SawPacket *>(msg);

    // Check if packet is for us
    if (data->getDestAddress() != myAddress) {
        EV << "DATA packet not for me (for " << data->getDestAddress() << "). Discarding." << endl;
        delete data;
        return;
    }

    if (data->getKind() == DATA_PACKET) {
        if (data->getSequenceNumber() == sequenceExpected) {
            // Correct packet received
            EV << "DATA (seq=" << data->getSequenceNumber() << ") received from " << data->getSrcAddress() << ". OK." << endl;

            sendAck(sequenceExpected, data->getSrcAddress()); // Send ACK to original sender

            // Update state
            sequenceExpected = 1 - sequenceExpected; // Flip expected sequence
            packetsReceived++;
        }
        else {
            // Duplicate packet received
            EV << "Duplicate DATA (seq=" << data->getSequenceNumber() << ") received. Resending ACK for (seq=" << (1 - sequenceExpected) << ")" << endl;

            // Resend ACK for the *last* correctly received packet
            sendAck(1 - sequenceExpected, data->getSrcAddress());
        }
    }

    delete data; // We are done with the data packet
}

void Receiver::finish()
{
    EV << "Receiver " << myAddress << " simulation finished. Total new packets received: " << packetsReceived << endl;
}
