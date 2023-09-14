//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package crossdomain.linklayer.shaper.queueing;

import core4inet.linklayer.shaper.IEEE8021Qbv.queueing.IIEEE8021QbvQueueing;

simple CQFQueueing like IIEEE8021QbvQueueing
{
    parameters:
        @class(CoRE4INET::CQFQueueing);
        @display("i=block/fork");
        // Number of priorities.
        int numPCP = default(8);
        // Default priority for frames without PCP.
        int defaultPCP = default(0);  
        // Untagged VLAN. Outgoing frames with this VLAN get untagged
        int untaggedVID = default(0);
        // Interface allows frames with this VLAN to leave the port
        string taggedVIDs = default("0");
        
        string ttSendingMode = default("tt");        
        
    gates:
        // Input for incoming inet::EtherFrame messages.
        input in[2] @labels(EtherFrame);
        // Ouput to the corresponding queues.
        output out[numPCP] @labels(EtherFrame);
}