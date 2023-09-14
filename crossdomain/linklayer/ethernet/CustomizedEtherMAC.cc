/*
 * CustomizedEtherMAC.cc
 *
 *  Created on: 20210416
 *      Author: twq
 */

#include "crossdomain/linklayer/ethernet/CustomizedEtherMAC.h"
#include "crossdomain/linklayer/ethernet/CustomizedEtherIIFrame_m.h"

namespace inet {

Define_Module(CustomizedEtherMAC);

void CustomizedEtherMAC::periodChange() {
    Enter_Method("periodChange()");
    if (transmitState == TRANSMITTING_STATE) {
        EV << "periodChange() is called!" << endl;
        transmissionChannel->forceTransmissionFinishTime(simTime());
        cancelEvent(endTxMsg);
        scheduleAt(transmissionChannel->getTransmissionFinishTime(), endTxMsg);
    }
    //EV << "periodChange() is called!" << endl;
    //transmissionChannel->forceTransmissionFinishTime(simTime());
    //cancelEvent(endTxMsg);
    //scheduleAt(transmissionChannel->getTransmissionFinishTime(), endTxMsg);
}

void CustomizedEtherMAC::handleMessage(cMessage* msg) {
    if (!isOperational) {
            handleMessageWhenDown(msg);
            return;
        }

        if (channelsDiffer)
            readChannelParameters(true);

        if (msg->isSelfMessage())
            handleSelfMessage(msg);
        else if (msg->getArrivalGate() == upperLayerInGate) {
            if (std::string("Best-Effort Traffic").compare(msg->getName()) != 0 &&
                    std::string("IEEE 802.1Q Traffic").compare(msg->getName()) != 0) {
                CoRE4INET::CustomizedEtherIIFrame* qframe = dynamic_cast<CoRE4INET::CustomizedEtherIIFrame*>(msg);
                std::string qframePathRecord = qframe->getPathRecord();
                simtime_t outTime = simTime();
                qframePathRecord += outTime.str() + ";";
                qframe->setPathRecord(qframePathRecord.c_str());
            }

            processFrameFromUpperLayer(check_and_cast<EtherFrame *>(msg));
        }
        else if (msg->getArrivalGate() == physInGate)
            processMsgFromNetwork(PK(msg));
        else
            throw cRuntimeError("Message received from unknown gate!");
}

}


