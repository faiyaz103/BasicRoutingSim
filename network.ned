package BasicRoutingSim;

import ned.DatarateChannel;

//
// Network with 2 Senders, 2 Receivers, and 1 Router.
//
network StopAndWaitNetwork
{
    parameters:
        double channelDelay @unit(s) = 0.05s;
        double channelDatarate @unit(bps) = 1Mbps;
        double channelError = 0.1;
        double senderTimeout @unit(s) = 0.5s;
        int totalPackets = 10;
    submodules:
        sender1: Sender {
            parameters:
                myAddress = 1;
                destAddress = 3; // Send to receiver1
                timeout = parent.senderTimeout;
                totalPacketsToSend = parent.totalPackets;
                @display("p=100,100;i=device/pc");
        }
        sender2: Sender {
            parameters:
                myAddress = 2;
                destAddress = 4; // Send to receiver2
                timeout = parent.senderTimeout;
                totalPacketsToSend = parent.totalPackets;
                @display("p=100,300;i=device/pc");
        }
        router: Router {
            parameters:
                @display("p=300,200;i=device/router");
        }
        receiver1: Receiver {
            parameters:
                myAddress = 3;
                @display("p=500,100;i=device/server");
        }
        receiver2: Receiver {
            parameters:
                myAddress = 4;
                @display("p=500,300;i=device/server");
        }
    connections:
        // Senders to Router
        sender1.out --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> router.in++;
        router.out++ --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> sender1.in;

        sender2.out --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> router.in++;
        router.out++ --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> sender2.in;

        // Router to Receivers
        router.out++ --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> receiver1.in;
        receiver1.out --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> router.in++;

        router.out++ --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> receiver2.in;
        receiver2.out --> DatarateChannel { delay=parent.channelDelay; datarate=parent.channelDatarate; per=parent.channelError; } --> router.in++;
}
