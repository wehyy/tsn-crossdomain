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

package simulation.cross_domain_demo2;


import inet.node.ethernet.Eth1G;	//chanel
import inet.node.ethernet.Eth10G;	//chanel
import inet.node.ethernet.Eth100G;	//chanel
import crossdomain.node.CQFHost;	//TTsenders
import crossdomain.node.CQFSwitch;	//Routers
import crossdomain.node.CQFEdgeSwitch;	//EdgeSwitch
import core4inet.nodes.ethernet.tsn.TSNEtherHost;	//TTReceivers and BE Hosts

network small_network
{
    parameters:
        @display("bgb=2657.715,1200.495,white");
    submodules:
        TTSender1: CQFHost {
            @display("p=330.075,239.61");
        }
        TTReceiver1: TSNEtherHost {
            @display("p=2271.405,704.16");
        }
        TTSender2: CQFHost {
            @display("p=330.075,540.34503");
        }
        TTReceiver2: TSNEtherHost {
            @display("p=2273.85,337.41");
        }
        TTSender3: CQFHost {
            @display("p=973.11,980.445");
        }
        TTReceiver3: TSNEtherHost {
            @display("p=1880.205,1034.235");
        }
        TTSender4: CQFHost {
            @display("p=1266.51,980.445");
        }
        TTReceiver4: TSNEtherHost {
            @display("p=2273.85,195.6");
        }
        BESender1: TSNEtherHost {
            @display("p=330.075,391.2");
        }
        BEReceiver1: TSNEtherHost {
            @display("p=2271.405,569.685");
        }
        BESender2: TSNEtherHost {
            @display("p=838.635,640.59");
        }
        BEReceiver2: TSNEtherHost {
            @display("p=1662.6,286.065");
        }
        N1: CQFSwitch {
            parameters:
                @display("p=838.635,462.105");
        }
        N2: CQFSwitch {
            parameters:
                @display("p=1117.365,462.105");
        }
        N3: CQFSwitch {
            parameters:
                @display("p=1391.205,462.105");
        }
        N4: CQFSwitch {
            parameters:
                @display("p=1662.6,462.105");
        }
        N5: CQFSwitch {
            parameters:
                @display("p=1662.6,696.825");
        }
        EN1: CQFEdgeSwitch {
            parameters:
                @display("p=643.035,371.64");
        }
        EN2: CQFEdgeSwitch {
            parameters:
                @display("p=652.815,523.23");
        }
        EN3: CQFEdgeSwitch {
            parameters:
                @display("p=1914.435,540.34503");
        }
        EN4: CQFEdgeSwitch {
            parameters:
                @display("p=1914.435,354.525");
        }
        EN5: CQFEdgeSwitch {
            parameters:
                @display("p=1117.365,674.82");
        }
        EN6: CQFEdgeSwitch {
            parameters:
                @display("p=1662.6,863.085");
        }
        S1: CQFSwitch {
            parameters:
                @display("p=466.995,303.18");
        }
        S2: CQFSwitch {
            parameters:
                @display("p=2112.48,618.585");
        }

        S3: CQFSwitch {
            parameters:
                @display("p=466.995,581.91");
        }
        S4: CQFSwitch {
            parameters:
                @display("p=2112.48,286.065");
        }
        S5: CQFSwitch {
            parameters:
                @display("p=1009.78503,843.525");
        }
        S6: CQFSwitch {
            parameters:
                @display("p=1217.61,843.525");
        }
        S7: CQFSwitch {
            parameters:
                @display("p=1662.6,1034.235");
        }


    connections:

        N1.ethg++ <--> Eth100G {  length = 50000m; } <--> N2.ethg++;
        N2.ethg++ <--> Eth100G {  length = 50000m; } <--> N3.ethg++;
        N3.ethg++ <--> Eth100G {  length = 50000m; } <--> N4.ethg++;
        N1.ethg++ <--> Eth10G {  length = 2000m; } <--> EN1.ethg++;
        N1.ethg++ <--> Eth10G {  length = 2000m; } <--> EN2.ethg++;
        N2.ethg++ <--> Eth10G {  length = 2000m; } <--> EN5.ethg++;
        N4.ethg++ <--> Eth10G {  length = 2000m; } <--> EN3.ethg++;
        N4.ethg++ <--> Eth10G {  length = 2000m; } <--> EN4.ethg++;
        N4.ethg++ <--> Eth100G {  length = 50000m; } <--> N5.ethg++;
        N5.ethg++ <--> Eth10G {  length = 2000m; } <--> EN6.ethg++;

        S1.ethg++ <--> Eth10G {  length = 1000m; } <--> EN1.ethg++;
        S2.ethg++ <--> Eth10G {  length = 1000m; } <--> EN3.ethg++;
        S3.ethg++ <--> Eth10G {  length = 1000m; } <--> EN2.ethg++;
        S4.ethg++ <--> Eth10G {  length = 1000m; } <--> EN4.ethg++;
        EN5.ethg++ <--> Eth10G {  length = 1000m; } <--> S5.ethg++;
        EN5.ethg++ <--> Eth10G {  length = 1000m; } <--> S6.ethg++;
        EN6.ethg++ <--> Eth10G {  length = 1000m; } <--> S7.ethg++;

        TTSender1.ethg <--> Eth1G {  length = 600m; } <--> S1.ethg++;
        TTSender2.ethg <--> Eth1G {  length = 600m; } <--> S3.ethg++;
        TTSender3.ethg <--> Eth1G {  length = 600m; } <--> S5.ethg++;
        TTSender4.ethg <--> Eth1G {  length = 600m; } <--> S6.ethg++;
        TTReceiver1.ethg <--> Eth1G {  length = 600m; } <--> S2.ethg++;
        TTReceiver2.ethg <--> Eth1G {  length = 600m; } <--> S4.ethg++;
        TTReceiver3.ethg <--> Eth1G {  length = 600m; } <--> S7.ethg++;
        TTReceiver4.ethg <--> Eth1G {  length = 600m; } <--> S4.ethg++;

        BESender1.ethg <--> Eth1G {  length = 600m; } <--> S1.ethg++;
        BEReceiver1.ethg <--> Eth1G {  length = 600m; } <--> S2.ethg++;
        BESender2.ethg <--> Eth10G {  length = 1000m; } <--> N1.ethg++;
        BEReceiver2.ethg <--> Eth10G {  length = 1000m; } <--> N4.ethg++;


}


