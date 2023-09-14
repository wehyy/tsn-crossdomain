/*
 * CustomizedSourceApp.cc
 *
 *  Created on: 20210417
 *      Author: twq
 */

#include "CustomizedSourceApp.h"
#include "crossdomain/linklayer/ethernet/CustomizedEtherIIFrame_m.h"

#include "core4inet/base/CoRE4INET_Defs.h"
#include "core4inet/buffer/base/BGBuffer.h"
#include "core4inet/utilities/ConfigFunctions.h"

namespace CoRE4INET{

Define_Module(CustomizedSourceApp);

CustomizedSourceApp::CustomizedSourceApp()
{
    this->sendInterval = 0;
    this->priority = 0;
    this->vid = 0;
}

void CustomizedSourceApp::initialize()
{
    TrafficSourceAppBase::initialize();
    for (unsigned int i = 0; i < this->numPCP; i++)
    {
        simsignal_t signalPk = registerSignal(("txQPcp" + std::to_string(i) + "Pk").c_str());
        cProperty* statisticTemplate = getProperties()->get("statisticTemplate", "txQPcpPk");
        getEnvir()->addResultRecorders(this, signalPk, ("txQPcp" + std::to_string(i) + "Pk").c_str(), statisticTemplate);
        statisticTemplate = getProperties()->get("statisticTemplate", "txQPcpBytes");
        getEnvir()->addResultRecorders(this, signalPk, ("txQPcp" + std::to_string(i) + "Bytes").c_str(), statisticTemplate);

        this->txQPcpPkSignals.push_back(signalPk);
    }
}




void CustomizedSourceApp::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
        {
            if (getEnvir()->isGUI())
            {
                getDisplayString().removeTag("i2");
            }
            sendMessage();
            scheduleAt(simTime() + this->sendInterval, msg); //TODO: Use scheduler of the node to support its modeled inaccuracy
        }
        else
        {
            delete msg;
        }
}

void CustomizedSourceApp::sendMessage()
{
    for (std::list<BGBuffer*>::const_iterator buf = bgbuffers.begin(); buf != bgbuffers.end(); ++buf)
        {
            CustomizedEtherIIFrame *frame = new CustomizedEtherIIFrame("IEEE 802.1Q Traffic");
            //CustomizedEtherIIFrame *frame = new CustomizedEtherIIFrame("Customized IEEE 8021Q Traffic");
            frame->setDest(this->destAddress);
            cPacket *payload_packet = new cPacket();
            payload_packet->setByteLength(static_cast<int64_t>(getPayloadBytes()));
            frame->encapsulate(payload_packet);
            frame->setPcp(priority);
            frame->setVID(this->vid);
            frame->setSchedulingPriority(static_cast<short>(SCHEDULING_PRIORITY_OFFSET_8021Q - priority));
            //Padding
            if (frame->getByteLength() < MIN_ETHERNET_FRAME_BYTES)
            {
                frame->setByteLength(MIN_ETHERNET_FRAME_BYTES);
            }
            emit(this->txQPcpPkSignals[priority], frame);
            sendDirect(frame, (*buf)->gate("in"));
        }
}

void CustomizedSourceApp::handleParameterChange(const char* parname)
{
    TrafficSourceAppBase::handleParameterChange(parname);

    if (!parname || !strcmp(parname, "sendInterval"))
    {
        this->sendInterval = parameterDoubleCheckRange(par("sendInterval"), 0, SIMTIME_MAX.dbl(), true);
    }
    if (!parname || !strcmp(parname, "destAddress"))
    {
        if (par("destAddress").stdstringValue() == "auto")
        {
            // assign automatic address
            this->destAddress = inet::MACAddress::generateAutoAddress();

            // change module parameter from "auto" to concrete address
            par("destAddress").setStringValue(this->destAddress.str());
        }
        else
        {
            this->destAddress.setAddress(par("destAddress").stringValue());
        }
    }
    if (!parname || !strcmp(parname, "priority"))
    {
        this->priority = static_cast<uint8_t>(parameterLongCheckRange(par("priority"), 0, MAX_Q_PRIORITY));
    }
    if (!parname || !strcmp(parname, "vid"))
    {
        this->vid = static_cast<uint16_t>(parameterLongCheckRange(par("vid"), 0, MAX_VLAN_ID));
    }
    if (!parname || !strcmp(parname, "numPCP"))
    {
        this->numPCP = static_cast<unsigned int>(parameterULongCheckRange(par("numPCP"), 1, std::numeric_limits<int>::max()));
    }
}

}



