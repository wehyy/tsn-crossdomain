/*
 * CustomizedSRPRelay.cc
 *
 *  Created on: 20210315
 *      Author: legendchef
 */

#include "crossdomain/linklayer/beHandling/avb/relay/CustomizedSRPRelay.h"
#include "inet/common/LayeredProtocolBase.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"

//CoRE4INET
#include "core4inet/base/avb/AVBDefs.h"
//Auto-generated Messages
#include "core4inet/linklayer/contract/ExtendedIeee802Ctrl_m.h"

//#include <stdlib.h>

using namespace inet;
using namespace std;

namespace CoRE4INET {
    Define_Module(CoRE4INET::CustomizedSRPRelay);

    void CustomizedSRPRelay::initialize(int stage)
    {
        inet::Ieee8021dRelay::initialize(stage);
    }

    void CustomizedSRPRelay::handleMessage(cMessage * msg) {
        if (!isOperational)
            {
                EV_ERROR << "Message '" << msg << "' arrived when module status is down, dropped it." << endl;
                delete msg;
                return;
            }

            if (msg && !msg->isSelfMessage())
            {
                // messages from STP process /output
                if (msg->arrivedOn("srpIn"))
                {
                    SRPFrame * srpFrame = check_and_cast<SRPFrame*>(msg);
                    dispatchSRP(srpFrame);

                }
                // handle msg from phy      /input
                else if (msg->arrivedOn("ifIn"))
                {
                    inet::EtherFrame * frame = check_and_cast<inet::EtherFrame*>(msg);
                    if ((frame->getDest() == SRP_ADDRESS || frame->getDest() == bridgeAddress))
                    {
                        EV_DETAIL << "Deliver SRPFrame to the SRP module" << endl;
                        deliverSRP(frame); // deliver to the SRP module
                    }
                    else if (string("Best-Effort Traffic").compare(frame->getName()) == 0 ||
                            string("IEEE 802.1Q Traffic").compare(frame->getName()) == 0) {
                        EV << "getName is " << frame->getName() << endl;
                        inet::Ieee8021dRelay::handleMessage(msg);
                    }
                    else
                    {
                        //inet::Ieee8021dRelay::handleMessage(msg);

                        string displayStr = frame->getName();
                        EV << "**displayStr is " << displayStr << endl;
                        vector<string> displayStrElements = cStringTokenizer(displayStr.c_str(), ";").asVector();
                        string transmitInfo = displayStrElements.back();
                        vector<string> transmitInfoElements = cStringTokenizer(transmitInfo.c_str(), ",").asVector();
                        unsigned int portNum = atoi((transmitInfoElements[1]).c_str());

                        newDispatch(frame, portNum);
                    }
                }
            }
            else
                throw cRuntimeError("This module doesn't handle self-messages!");
    }

    void CustomizedSRPRelay::dispatchSRP(SRPFrame * srp)
    {
        ExtendedIeee802Ctrl * controlInfo = dynamic_cast<ExtendedIeee802Ctrl *>(srp->removeControlInfo());
        if (!controlInfo)
        {
            throw cRuntimeError("SRP is missing ControlInfo");
        }
        int portNum = controlInfo->getSwitchPort();
        int notPortNum = controlInfo->getNotSwitchPort();
        inet::MACAddress address = controlInfo->getDest();

        if (portNum >= static_cast<int>(portCount))
            throw cRuntimeError("Output port %d doesn't exist!", portNum);

        inet::EthernetIIFrame * frame = new inet::EthernetIIFrame(srp->getName());
        frame->setSrc(bridgeAddress);
        frame->setDest(address);
        frame->setByteLength(ETHER_MAC_FRAME_BYTES);
        frame->setEtherType(MSRP_ETHERTYPE);
        frame->encapsulate(srp);

        delete controlInfo;

        if (frame->getByteLength() < MIN_ETHERNET_FRAME_BYTES)
            frame->setByteLength(MIN_ETHERNET_FRAME_BYTES);

        //Broadcast
        if (portNum < 0)
        {
            for (size_t i = 0; i < portCount; ++i)
            {
                if (static_cast<int>(i) != notPortNum)
                {
                    send(frame->dup(), "ifOut", static_cast<int>(i));
                    EV_INFO << "Sending SRP frame " << frame << " with destination = " << frame->getDest() << ", port = "
                            << i << endl;
                }
            }
            delete frame;
        }
        else
        {
            send(frame, "ifOut", portNum);
            EV_INFO << "Sending SRP frame " << frame << " with destination = " << frame->getDest() << ", port = " << portNum
                    << endl;
        }
    }

    void CustomizedSRPRelay::deliverSRP(inet::EtherFrame * frame)
    {
        SRPFrame * srp = check_and_cast<SRPFrame *>(frame->decapsulate());

        inet::Ieee802Ctrl * controlInfo = new inet::Ieee802Ctrl();
        controlInfo->setSrc(frame->getSrc());
        controlInfo->setSwitchPort(frame->getArrivalGate()->getIndex());
        controlInfo->setDest(frame->getDest());

        srp->setControlInfo(controlInfo);

        delete frame; // we have the SRP packet, so delete the frame

        EV_INFO << "Sending SRP frame " << srp << " to the SRP module" << endl;
        send(srp, "srpOut");
    }

    void CustomizedSRPRelay::newDispatch(EtherFrame *frame, unsigned int portNum) {

        EV_INFO << "Sending frame " << frame << " on output port " << portNum << "." << endl;

        if (portNum >= portCount)
            throw cRuntimeError("Output port %d doesn't exist!", portNum);

        EV_INFO << "Sending " << frame << " with destination = " << frame->getDest() << ", port = " << portNum << endl;

        numDispatchedNonBPDUFrames++;
        emit(LayeredProtocolBase::packetSentToLowerSignal, frame);
        send(frame, "ifOut", portNum);
    }
}


