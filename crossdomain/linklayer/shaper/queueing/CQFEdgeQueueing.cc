/*
 * CQFEdgeQueueing.cc
 *
 *  Created on: 20210312
 *      Author: legendchef
 */

#include "CQFEdgeQueueing.h"

#include <algorithm>
#include <limits>
//CoRE4INET
#include "core4inet/base/CoRE4INET_Defs.h"
#include "core4inet/utilities/ConfigFunctions.h"
//Auto-generated Messages
#include "core4inet/linklayer/ethernet/base/EtherFrameWithQTag_m.h"
#include "crossdomain/linklayer/shaper/CycleIndex.h"

namespace CoRE4INET {
    Define_Module(CQFEdgeQueueing);

    void CQFEdgeQueueing::initialize()
    {
        EV << "CQFEdgeQueueing::initialize() is called!! <tag1>" << endl;
        this->handleParameterChange(nullptr);
    }

    void CQFEdgeQueueing::handleParameterChange(const char* parname)
    {
        EV << "CQFEdgeQueueing::handleParameterChange(const char* parname) is called!! <tag2>" << endl;
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

    void CQFEdgeQueueing::handleMessage(cMessage *msg) {

        EV << "CQFEdgeQueueing::handleMessage(cMessage *msg) is called!! <tag3>" << endl;
        EV << "untaggedVID:" <<this->untaggedVID << endl;
        if (msg->arrivedOn("in",0) || msg->arrivedOn("in",1))
        {
            EV<< "msg arrive on 'in' 0 or 1" << endl;
            if (EthernetIIFrameWithQTag* qframe = dynamic_cast<EthernetIIFrameWithQTag*>(msg))
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

                string sendingMode = par("sendingMode");
                EV << "sendingMode: " << sendingMode << endl;
                //if (sendingMode.compare("tt") == 0)
                //{
                    CycleIndex* ci = dynamic_cast<CycleIndex*>(this->getParentModule()->getSubmodule("cycleIndex"));
                    EV << "Current cycle index is " << ci->getCycleIndex() << "!!!!!!!!!!!"<< endl;

                    //vector<string> fullPathElements = cStringTokenizer((qframe->getFullPath()).c_str(), ".").asVector();
                    //string localNode = fullPathElements[1];

                    char sendIndex[8];
                    itoa((ci->getCycleIndex() + 1) % 3,sendIndex, 10);
                    string sendCycleName = "cycle" + string(sendIndex);
                    EV << "sendCycleName is " + sendCycleName << endl;

                    cXMLElement* element = par("scheduleInfo").xmlValue();
                    EV << "element->getTagName(): " << element->getTagName() << endl;
                    cXMLElementList scheduleInfoList = element->getElementsByTagName(sendCycleName.c_str());
                    EV << "scheduleInfoList size: " << scheduleInfoList.size() << endl;
                    EV << "scheduleInfoList[0]->getXML(): " << scheduleInfoList[0]->getXML() << endl;
                    EV << "scheduleInfoList[0]->getTagName(): " << scheduleInfoList[0]->getTagName() << endl;
                    EV << "scheduleInfoList[0]->getFirstChild()->getTagName(): " <<scheduleInfoList[0]->getFirstChild()->getTagName() << endl;
                    string displayStr = "";
                    for (cXMLElement * child = scheduleInfoList[0]->getLastChild(); child; child = child->getPreviousSibling()) {
                        displayStr += child->getAttribute("node") + string(",") +
                                child->getAttribute("port") + string(",") +
                                child->getAttribute("cycle") + string(";");
                    }
                    EV << "displayStr: " << displayStr << endl;
                    displayStr.pop_back();

                    qframe->setName(displayStr.c_str());

                    if (sendingMode.compare("tt") == 0) {
                        int sendingIndex = (ci->getCycleIndex() + 1) % 3;
                        this->send(qframe, "out", 7-sendingIndex);
                    }
                    else if (sendingMode.compare("be") == 0) {
                        this->send(qframe, "out", qframe->getPcp());
                    }

                //}
                //else if (sendingMode.compare("be") == 0) {
                //    this->send(qframe, "out", qframe->getPcp());
                //}

                //this->send(qframe, "out", qframe->getPcp());
            }

            else
            {
                this->send(msg, "out", this->defaultPCP);
            }
        }
    }

}



