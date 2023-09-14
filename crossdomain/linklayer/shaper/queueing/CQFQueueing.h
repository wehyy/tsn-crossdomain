/*
 * CQFQueueing.h
 *
 *  Created on: 20210309
 *      Author: twq
 */

#ifndef CROSSDOMAIN_LINKLAYER_SHAPER_QUEUEING_CQFQUEUEING_H_
#define CROSSDOMAIN_LINKLAYER_SHAPER_QUEUEING_CQFQUEUEING_H_


#include <omnetpp.h>

#include "core4inet/linklayer/shaper/IEEE8021Qbv/selection/IEEE8021QbvSelection.h"
//#include "core4inet/linklayer/shaper/IEEE8021Qbv/queueing/IEEE8021QbvQueueing.h"

using namespace std;
using namespace omnetpp;

namespace CoRE4INET {

class CQFQueueing : public virtual cSimpleModule {
private:
    /**
     * @brief Number of priorities.
     */
    unsigned int numPCP;
    /**
     * @brief Default priority for frames without PCP.
     */
    unsigned int defaultPCP;
    /**
     * @brief Untagged VLAN.
     * Untagged incoming frames get tagged with this VLAN. Outgoing frames with this VLAN get untagged.
     */
    uint16_t untaggedVID;
    /**
     * @brief Tagged VLANs.
     * Only outgoing frames with one of the VLANs in this list are transmitted. Default is "0" to allow for untagged frames
     */
    std::vector<int> taggedVIDs;

  protected:
    /**
     * @brief Initializes the module.
     */
    virtual void initialize();
    /**
     * @brief Indicates a parameter has changed.
     *
     * @param parname Name of the changed parameter or nullptr if multiple parameter changed.
     */
    virtual void handleParameterChange(const char* parname);
    /**
     * @brief Handles the incoming message of the IEEE 802.1Qbv transmission selection.
     *
     * @param msg incoming inet::EtherFrame.
     */
    virtual void handleMessage(cMessage *msg);
};

}


#endif /* IEEE8021QCQF_LINKLAYER_SHAPER_QUEUEING_CQFQUEUEING_H_ */
