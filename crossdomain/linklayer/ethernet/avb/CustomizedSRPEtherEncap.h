/*
 * CustomizedSRPEtherEncap.h
 *
 *  Created on: 20210630
 *      Author: legendchef
 */

#ifndef CROSSDOMAIN_LINKLAYER_ETHERNET_AVB_CUSTOMIZEDSRPETHERENCAP_H_
#define CROSSDOMAIN_LINKLAYER_ETHERNET_AVB_CUSTOMIZEDSRPETHERENCAP_H_

#include "core4inet/linklayer/ethernet/avb/SRPEtherEncap.h"

namespace CoRE4INET {
    class CustomizedSRPEtherEncap : public SRPEtherEncap
    {
    protected:
        void handleMessage(cMessage *msg);
    };
}



#endif /* CROSSDOMAIN_LINKLAYER_ETHERNET_AVB_CUSTOMIZEDSRPETHERENCAP_H_ */
