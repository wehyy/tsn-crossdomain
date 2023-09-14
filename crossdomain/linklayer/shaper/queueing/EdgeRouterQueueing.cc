/*
 * EdgeRouterQueueing.cc
 *
 *  Created on: 20211012
 *      Author: weny
 *
 * function: 1、跨域的cycle mapping; 获取当前设备上的 currentcycle，匹配 shelduleInfo 文件
 */




#include "EdgeRouterQueueing.h"

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

Define_Module(EdgeRouterQueueing);

void EdgeRouterQueueing::initialize()
{
    this->handleParameterChange(nullptr);
}

void EdgeRouterQueueing::handleParameterChange(const char* parname)
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

void EdgeRouterQueueing::handleMessage(cMessage *msg)
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

            CycleIndex * ci = dynamic_cast<CycleIndex *>(this->getParentModule()->getSubmodule("cycleIndex"));
            EV << "Current cycle index is " << ci->getCycleIndex() << "!" << endl;

            char nextIndex[8];
            itoa(ci->getCycleIndex(), nextIndex, 10);
            string sendCycleName = "cycle" + string(nextIndex);
            EV << "sendCycleName is " + sendCycleName << endl;

            string backupStr = qframe->getName();
            EV << "backupStr: " << backupStr <<endl;
            vector<string> backupStrElements = cStringTokenizer(backupStr.c_str(), ";").asVector();
            backupStrElements.pop_back();

            cXMLElement* element = par("scheduleInfo").xmlValue();
            cXMLElementList scheduleInfoList = element->getElementsByTagName(sendCycleName.c_str());
            string displayStr = "";
            if (backupStrElements.size() > 0) {
                for (int i = 0; i < backupStrElements.size(); i++) {
                    displayStr += backupStrElements[i] + ';';
                }
            }
            for (cXMLElement * child = scheduleInfoList[0]->getLastChild(); child; child = child->getPreviousSibling()) {
                displayStr += child->getAttribute("node") + string(",") +
                        child->getAttribute("port") + string(",") +
                        child->getAttribute("cycle") + string(";");
            }
            EV << "displayStr: " << displayStr << endl;
            vector<string> displayStrElements = cStringTokenizer(displayStr.c_str(), ";").asVector();
            string TransmitInfo = displayStrElements.back();
            vector<string> transmitInfoElements = cStringTokenizer(TransmitInfo.c_str(), ",").asVector();
            int sendIndex = atoi((transmitInfoElements[2].c_str()));

            EV<< "send Index is: " << sendIndex << "!!!<EdgeRouterQueueing:handlemsg>" << endl;

            displayStrElements.pop_back();
            if (displayStrElements.size() > 0) {
                string newDisplayStr = displayStrElements[0];
                for (int i = 1; i < displayStrElements.size(); i++) {
                    newDisplayStr += ";" + displayStrElements[i] ;
                }
                qframe->setName(newDisplayStr.c_str());
                if (ttSendingMode.compare("be") == 0) {
                    send(qframe, "out", qframe->getPcp());
                } else {
                    send(qframe, "out", 7-sendIndex);
                }
            }
            else {
                if (ttSendingMode.compare("be") == 0) {
                    send(qframe, "out", qframe->getPcp());
                } else {
                    send(qframe, "out", 7-sendIndex);
                }
            }
        }
        else
        {
            this->send(msg, "out", this->defaultPCP);
        }

    }
}

}
