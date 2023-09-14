/*
 * CQFGateControlList.h
 *
 *  Created on: 20210309
 *      Author: twq
 */

#ifndef CROSSDOMAIN_LINKLAYER_SHAPER_GATE_CQFGATECONTROLLIST_H_
#define v_LINKLAYER_SHAPER_GATE_CQFGATECONTROLLIST_H_

#include <omnetpp.h>
#include "core4inet/utilities/classes/Scheduled.h"

using namespace std;
using namespace omnetpp;

namespace CoRE4INET {
class CQFGateControlList: public virtual Scheduled
{
protected:
    /**
     * @brief Number of gates for control.
     */
    unsigned int numGates;

    //simsignal_t periodChange = registerSignal("periodChange");

  private:
    /**
     * @brief Vector containing the initial gate states.
     */
    vector<string> initalGateStates;
    /**
     * @brief Data structure containing the complete gate control list.
     */
    vector<pair<vector<string>, double>> gateControlList;
    /**
     * @brief Iterator of control list data structure. Stores a pointer to the current control list element.
     */
    vector<pair<vector<string>, double>>::iterator gateControlElement;
    /**
     * @brief TODO
     */
    unsigned int configNo;
    /**
     * @brief TODO
     */
    string timerEventName;

  public:
    /**
     * @brief Constructor
     */
    CQFGateControlList();

    /**
     * Exchange the current gate control list with a new one.
     * This function calls handle parameter change to perform the changes.
     * @param gcl the new gate control list to set.
     */
    virtual void setGateControlList (string gcl);

  protected:
    /**
     * @brief Initializes the module.
     *
     * @param stage The stages. Module initializes when stage==0
     */
    virtual void initialize(int stage);
    /**
     * @brief Returns the number of initialization stages this module needs.
     *
     * @return returns 2
     */
    virtual int numInitStages() const;
    /**
     * @brief Indicates a parameter has changed.
     *
     * @param parname Name of the changed parameter or nullptr if multiple parameter changed.
     */
    virtual void handleParameterChange(const char* parname);
    /**
     * @brief Receives SchedulerTimerEvent messages from scheduler indicating the expiration of the current control element.
     *
     * Switches to next control element, propagate its gate states and schedule the next SchedulerTimerEvent message.
     *
     * @param msg the incoming message
     */
    virtual void handleMessage(cMessage *msg);
    /**
     * @brief Propagate the gate states of the current control element.
     */
    virtual void propagteGateControlElement(vector<string> gateStates);

  private:
    /**
     * @brief Schedule the next SchedulerTimerEvent message.
     */
    void scheduleCurrentGateControlElementTime(bool nextCycle);
    /**
     * @brief Switch to next control element of the control list.
     */
    void switchToNextGateControlElement();
};
}




#endif /* IEEE8021QCQF_LINKLAYER_SHAPER_GATE_CQFGATECONTROLLIST_H_ */
