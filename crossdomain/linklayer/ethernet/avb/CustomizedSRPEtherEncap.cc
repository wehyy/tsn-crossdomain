/*
 * CustomizedSRPEtherEncap.cc
 *
 *  Created on: 20210630
 *      Author: legendchef
 *
 * notes by weny in 2021.10.12
 * function: do not handle the massage from the gate "srpIn" and "lowerLayerIn"
 *
 */

#include "crossdomain/linklayer/ethernet/avb/CustomizedSRPEtherEncap.h"

namespace CoRE4INET {

Define_Module(CustomizedSRPEtherEncap);

void CustomizedSRPEtherEncap::handleMessage(cMessage *msg)
{
    if (msg && msg->arrivedOn("srpIn"))
    {
//        SRPFrame * srpFrame = check_and_cast<SRPFrame*>(msg);
//        dispatchSRP(srpFrame);
    }
    else if (msg && msg->arrivedOn("lowerLayerIn"))
    {
//        inet::EtherFrame * frame = check_and_cast<inet::EtherFrame*>(msg);
//        if (frame->getDest() == SRP_ADDRESS)
//        {
//            EV_DETAIL << "Deliver SRPFrame to the SRP module" << endl;
//            deliverSRP(frame); // deliver to the SRP module
//        }
//        else
//        {
//            BGEtherEncap::handleMessage(msg);
//        }
    }
    else
    {
        //EthernetIIFrameWithQTag* qframe = dynamic_cast<EthernetIIFrameWithQTag*>(msg);
        //qframe->setDisplayString("hello");
        //BGEtherEncap::handleMessage(qframe);
        BGEtherEncap::handleMessage(msg);
    }
}

}


