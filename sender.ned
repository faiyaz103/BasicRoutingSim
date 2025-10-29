package BasicRoutingSim;

//
// Sender for Stop-and-Wait
// Now includes address parameters
//
simple Sender
{
    parameters:
        double timeout @unit(s); // Timeout duration
        int totalPacketsToSend;
        int myAddress;          // This sender's address
        int destAddress;        // The destination's address
        @display("i=block/tx");
        @class(Sender);
    gates:
        input in;
        output out;
}
