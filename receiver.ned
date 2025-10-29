package BasicRoutingSim;

//
// Receiver for Stop-and-Wait
// Now includes an address parameter
//
simple Receiver
{
    parameters:
        int myAddress; // This receiver's address
        @display("i=block/rx");
        @class(Receiver);
    gates:
        input in;
        output out;
}
