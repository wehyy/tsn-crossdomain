/*
 * CQFQueueing.cc
 *
 *  Created on: 20210309
 *      Author: twq
 *
 *
 * note by weny in 2021.10.12
 * handle frame
 * end of note.
 */

#include "CQFQueueing.h"

#include <algorithm>
#include <limits>
#include <stdlib.h>
//CoRE4INET
#include "core4inet/base/CoRE4INET_Defs.h"
#include "core4inet/utilities/ConfigFunctions.h"
//Auto-generated Messages
#include "core4inet/linklayer/ethernet/base/EtherFrameWithQTag_m.h"
#include "crossdomain/linklayer/ethernet/CustomizedEtherIIFrame_m.h"
#include "crossdomain/linklayer/shaper/CycleIndex.h"

#include "inet/linklayer/common/MACAddress.h"

namespace CoRE4INET {

Define_Module(CQFQueueing);

void CQFQueueing::initialize()
{
    this->handleParameterChange(nullptr);
}

void CQFQueueing::handleParameterChange(const char* parname)
{
    if (!parname || !strcmp(parname, "untaggedVID"))
    {
        this->untaggedVID = static_cast<uint16_t>(parameterULongCheckRange(par("untaggedVID"), 0, MAX_VLAN_NUMBER));
    }
    if (!parname || !strcmp(parname, "taggedVIDs"))
    {
        this->taggedVIDs = cStringTokenizer(par("taggedVIDs"), ",").asIntVector();
        std::sort(taggedVIDs.begin(), taggedVIDs.end());
    }
    if (!parname || !strcmp(parname, "numPCP"))
    {
        this->numPCP = static_cast<unsigned int>(parameterULongCheckRange(par("numPCP"), 1, std::numeric_limits<int>::max()));
    }
    if (!parname || !strcmp(parname, "defaultPCP"))
    {
        this->defaultPCP = static_cast<unsigned int>(parameterULongCheckRange(par("defaultPCP"), 0, this->numPCP-1));
    }
}

void CQFQueueing::handleMessage(cMessage *msg)
{


    if (msg->arrivedOn("in",0) || msg->arrivedOn("in",1))
    {
        //if (EthernetIIFrameWithQTag* qframe = dynamic_cast<EthernetIIFrameWithQTag*>(msg))
        if (CustomizedEtherIIFrame* qframe = dynamic_cast<CustomizedEtherIIFrame*>(msg))
        {
            //VLAN untag if requested
            if (this->untaggedVID && this->untaggedVID == qframe->getVID())
            {
                qframe->setVID(0);
            }
            //VLAN check if port is tagged with VLAN
            bool found = false;
            for (std::vector<int>::iterator vid = this->taggedVIDs.begin(); vid != this->taggedVIDs.end(); ++vid)
            {
                //Shortcut due to sorted vector
                if ((*vid) > qframe->getVID())
                {
                    break;
                }
                if ((*vid) == qframe->getVID())
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                delete qframe;
                return;
            }

            string qframePathRecord = qframe->getPathRecord();
            EV << "CustomizedEtherIIFrame msg : "  << qframe->getPathRecord() << endl;
            //string qframePathRecord = " ";
            vector<string> fullPathElements = cStringTokenizer((qframe->getFullPath()).c_str(), ".").asVector();
            string localNode = fullPathElements[1];
            qframePathRecord += localNode + ",";
            simtime_t inTime = simTime();
            qframePathRecord += inTime.str() + ",";
            qframe->setPathRecord(qframePathRecord.c_str());
            EV << "CustomizedEtherIIFrame msg : "  << qframe->getPathRecord() << endl;


            string ttSendingMode = par("ttSendingMode");
            //if (string("IEEE 802.1Q Traffic").compare(qframe->getName()) == 0) {
            //if (ttSendingMode.compare("be") == 0) {
            //    this->send(qframe, "out", qframe->getPcp());
            //}
            //else {

            CycleIndex * ci = dynamic_cast<CycleIndex *>(this->getParentModule()->getSubmodule("cycleIndex"));
            EV << "Current cycle index is " << ci->getCycleIndex() << "!" << endl;

            //string displayStr = qframe->getDisplayString();
            string displayStr = qframe->getName();
            vector<string> displayStrElements = cStringTokenizer(displayStr.c_str(), ";").asVector();
            string transmitInfo = displayStrElements.back();
            vector<string> transmitInfoElements = cStringTokenizer(transmitInfo.c_str(), ",").asVector();
            int localIndex = atoi((transmitInfoElements[2]).c_str());

            //int upstreamIndex = atoi((displayStrElements[0]).c_str());
            //string srcAddr = qframe->getSrc()->str();
            //EV << "srcAddr: " << srcAddr << endl;

            //int localIndex = ci->queryLocalIndex(displayStrElements[1], upstreamIndex);
            EV << "local Index: " << localIndex  << "!!!!"<< endl;  //local index represent xml index
            //char c[8];
            //itoa(localIndex, c, 10);
            //vector<string> fullPathElements = cStringTokenizer((qframe->getFullPath()).c_str(), ".").asVector();
            //string localNode = fullPathElements[1];
            //qframe->setDisplayString((c + string(";") + localNode).c_str());

            //this->send(qframe, "out", 7-localIndex);

            displayStrElements.pop_back();
            if (displayStrElements.size() > 0) {
                string newDisplayStr = displayStrElements[0];
                for (int i = 1; i < displayStrElements.size(); i++) {
                    newDisplayStr += ";" + displayStrElements[i] ;
                }
                //qframe->setDisplayString(newDisplayStr.c_str());
                qframe->setName(newDisplayStr.c_str());
                if (ttSendingMode.compare("be") == 0) {
                    send(qframe, "out", qframe->getPcp());
                } else {
                    send(qframe, "out", 7-localIndex);
                }
            }
            else {
                if (ttSendingMode.compare("be") == 0) {
                    send(qframe, "out", qframe->getPcp());
                } else {
                    send(qframe, "out", 7-localIndex);
                }
                //send(qframe, "out", 7-localIndex);
            }


            /*
            if (qframe->getSenderModule()->getFullPath().compare("small_network.node1.phy[0].mac") == 0) {
                int localIndex = ci->queryLocalIndex("node1", upstreamIndex);
                char c[8];
                itoa(localIndex, c, 10);
                qframe->setDisplayString(c);
                this->send(qframe, "out", 7-localIndex);
            }
            else if (qframe->getSenderModule()->getFullPath().compare("small_network.switch1.phy[2].mac") == 0) {
                int localIndex = ci->queryLocalIndex("sw1", upstreamIndex);
                char c[8];
                itoa(localIndex, c, 10);
                qframe->setDisplayString(c);
                this->send(qframe, "out", 7-localIndex);
            }
            */

            //if (ci->getCycleIndex() == 0) {
            //    this->send(qframe, "out", 6);
            //}
            //else if (ci->getCycleIndex() == 1) {
            //    this->send(qframe, "out", 7);
            //}
            //this->send(qframe, "out", qframe->getPcp());
            //}
        }
        else
        {
            this->send(msg, "out", this->defaultPCP);
        }

    }
}

}



