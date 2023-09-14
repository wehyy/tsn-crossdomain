/*
 * CustomizedEtherMAC.h
 *
 *  Created on: 2021Äê4ÔÂ6ÈÕ
 *      Author: twq
 */

#ifndef CROSSDOMAIN_LINKLAYER_ETHERNET_CUSTOMIZEDETHERMAC_H_
#define CROSSDOMAIN_LINKLAYER_ETHERNET_CUSTOMIZEDETHERMAC_H_

#include "inet/linklayer/ethernet/EtherMACFullDuplex.h"

namespace inet {

class CustomizedEtherMAC : public EtherMACFullDuplex {
public:
    void periodChange();

protected:
    virtual void handleMessage(cMessage* msg);
};

}


#endif /* IEEE8021QCQF_LINKLAYER_ETHERNET_CUSTOMIZEDETHERMAC_H_ */
