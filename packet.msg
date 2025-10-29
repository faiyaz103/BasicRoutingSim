//
// Defines the packet structure for our protocol.
// Now includes source and destination addresses.
//
cplusplus {{
#include <omnetpp.h>
}}

namespace BasicRoutingSim;

// Define packet types
enum PacketKind {
    DATA_PACKET = 0;
    ACK_PACKET = 1;
};

packet SawPacket {
    int kind; // Use PacketKind enum
    int sequenceNumber;
    string payload;
    int srcAddress;  // New
    int destAddress; // New
}
