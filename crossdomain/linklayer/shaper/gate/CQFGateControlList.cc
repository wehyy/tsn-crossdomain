/*
 * CQFGateControlList.cc
 *
 *  Created on: 20210309
 *      Author: twq
 */

#include "crossdomain/linklayer/shaper/gate/CQFGateControlList.h"

#include <limits>

#include "core4inet/utilities/ConfigFunctions.h"
#include "core4inet/linklayer/shaper/IEEE8021Qbv/gate/IEEE8021QbvGate.h"
//Auto-generated Messages
#include "core4inet/scheduler/SchedulerMessageEvents_m.h"
#include "crossdomain/linklayer/shaper/CycleIndex.h"
#include "crossdomain/linklayer/ethernet/CustomizedEtherMAC.h"

namespace CoRE4INET {
    Define_Module(CQFGateControlList);

    CQFGateControlList::CQFGateControlList()
    {
        this->numGates = 0;
        this->configNo = 0;
    }

    void CQFGateControlList::setGateControlList(string gcl) {
        this->par("controlList").setStringValue(gcl);
        this->handleParameterChange("controlList");
    }

    void CQFGateControlList::initialize(int stage)
    {
        if (stage == 1)
        {
            Scheduled::initialize();
            this->handleParameterChange(nullptr);
            if (this->gateControlList.size() > 0)
            {
                this->scheduleCurrentGateControlElementTime(false);
            }
            WATCH(this->configNo);
            WATCH(this->timerEventName);
        }
    }

    int CQFGateControlList::numInitStages() const
    {
        return 2;
    }

    void CQFGateControlList::handleParameterChange(const char* parname)
    {
        Scheduled::handleParameterChange(parname);
        if (!parname || !strcmp(parname, "numGates"))
        {
            this->numGates = static_cast<unsigned int>(parameterULongCheckRange(par("numGates"), 1, std::numeric_limits<int>::max()));
        }
        if (!parname || !strcmp(parname, "controlList"))
        {
            gateControlList.clear();
            vector<string> controlRows = cStringTokenizer(par("controlList"), ";").asVector();
            for (vector<string>::const_iterator controlRow = controlRows.begin(); controlRow != controlRows.end(); ++controlRow)
            {
                vector<string> controlRowTupel = cStringTokenizer((*controlRow).c_str(), ":").asVector();
                vector<string> controlRowGates = cStringTokenizer(controlRowTupel[0].c_str(), ",").asVector();
                if (controlRowGates.size() != this->numGates)
                {
                    throw cRuntimeError("controlList row size is not %d", this->numGates);
                }
                for (size_t i=0; i<controlRowGates.size(); i++)
                {
                    if (strcmp(controlRowGates[i].c_str(), "o") && strcmp(controlRowGates[i].c_str(), "C"))
                    {
                        throw cRuntimeError("controlList contains unexpected character \'%s\'. Allowed are \'o' (OPEN) or \'C\' (CLOSED).", controlRowGates[i].c_str());
                    }
                }
                double controlRowTime = stod(controlRowTupel[1]);
                gateControlList.push_back(make_pair(controlRowGates, controlRowTime));
            }
            this->gateControlElement = this->gateControlList.begin();
            if (this->gateControlList.size() > 0)
            {
                this->configNo++;
                this->timerEventName = "Gate Control List Config " + to_string(this->configNo) + " Scheduler Event";
                this->scheduleCurrentGateControlElementTime(false);
            }
        }
    }

    void CQFGateControlList::handleMessage(cMessage *msg)
    {
        if (msg->arrivedOn("schedulerIn") && msg->getKind() == ACTION_TIME_EVENT)
        {
            if (!strcmp(this->timerEventName.c_str(), msg->getName()))
            {
                this->propagteGateControlElement((*(this->gateControlElement)).first);
                if (this->gateControlList.size() > 1)
                {
                    this->switchToNextGateControlElement();
                    this->scheduleCurrentGateControlElementTime(false);
                }
            }
        }
        delete msg;
    }

    void CQFGateControlList::propagteGateControlElement(vector<string> gateStates)
    {
        if (this->getParentModule()->getSubmodule("queueing")->findPar("ttSendingMode") != -1) {
            string ttSendingMode = this->getParentModule()->getSubmodule("queueing")->par("ttSendingMode");
            if (ttSendingMode.compare("tt") == 0) {
                //调用mac的函数中止传输
                inet::CustomizedEtherMAC * cusMac = dynamic_cast<inet::CustomizedEtherMAC* >(this->getParentModule()
                        ->getParentModule()->getSubmodule("mac"));
                cusMac->periodChange();
            }
        }
        else if (this->getParentModule()->getSubmodule("queueing")->findPar("sendingMode") != -1) {
            string sendingMode = this->getParentModule()->getSubmodule("queueing")->par("sendingMode");
            if (sendingMode.compare("tt") == 0) {
                //调用mac的函数中止传输
                inet::CustomizedEtherMAC * cusMac = dynamic_cast<inet::CustomizedEtherMAC*>(this->getParentModule()
                        ->getParentModule()->getSubmodule("mac"));
                cusMac->periodChange();
            }
        }

        int cycleIndex = this->gateControlElement - this->gateControlList.begin();
        CycleIndex * ci = dynamic_cast<CycleIndex *>(this->getParentModule()->getSubmodule("cycleIndex"));
        ci->setCycleIndex(cycleIndex);
        EV << "cycleIndex is " << ci->getCycleIndex() << "!!!" << endl;
        EV << "state [7] is " << this->gateControlElement->first[7] << endl;
        EV << "Controllist starttime is " << this->gateControlElement->second << endl;
        EV << "propagteGateControlElement(vector<string> gateStates) is called!!!" << endl;
        for (long i=static_cast<long>(this->numGates)-1; i>=0; i--)
        {
            IEEE8021QbvGate* tg = dynamic_cast<IEEE8021QbvGate*>(this->getParentModule()->getSubmodule("transmissionGate", i));
            if ( !strcmp(gateStates[i].c_str(), "o"))
            {
                tg->open();
            }
            if ( !strcmp(gateStates[i].c_str(), "C"))
            {
                tg->close();
            }
        }
    }

    void CQFGateControlList::scheduleCurrentGateControlElementTime(bool nextCycle)
    {
        SchedulerActionTimeEvent* actionTimeEvent = new SchedulerActionTimeEvent(this->timerEventName.c_str(), ACTION_TIME_EVENT);
        uint32_t actionTime = static_cast<uint32_t>(ceil((*(this->gateControlElement)).second / getOscillator()->getPreciseTick()));
        if (actionTime >= getPeriod()->getCycleTicks())
        {
            throw cRuntimeError("The send window (%d ticks) starts outside of the period (%d ticks)",
                    actionTime, getPeriod()->getCycleTicks());
        }
        actionTimeEvent->setAction_time(actionTime);
        actionTimeEvent->setNext_cycle(nextCycle);
        actionTimeEvent->setDestinationGate(this->gate("schedulerIn"));
        getPeriod()->registerEvent(actionTimeEvent);
    }

    void CQFGateControlList::switchToNextGateControlElement()
    {
        ++(this->gateControlElement);

        if (this->gateControlElement == this->gateControlList.end())
        {
            this->gateControlElement = this->gateControlList.begin();
        }

    }
}



