package BasicRoutingSim;

//
// A simple router that forwards based on address.
// Address 1 -> gate 0
// Address 2 -> gate 1
// Address 3 -> gate 2
// Address 4 -> gate 3
//
simple Router
{
    parameters:
        @display("i=block/switch");
        @class(Router);
    gates:
        input in[];  // Vector gates
        output out[]; // Vector gates
}
