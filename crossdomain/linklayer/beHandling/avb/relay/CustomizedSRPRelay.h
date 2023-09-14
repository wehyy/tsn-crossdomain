/*
 * CustomizedSRPRelay.h
 *
 *  Created on: 20210315
 *      Author: legendchef
 *
 * noted by weny in 2021.10.12
 * configure of the beswitch module
 */

#ifndef CROSSDOMAIN_LINKLAYER_BEHANDLING_AVB_RELAY_CUSTOMIZEDSRPRELAY_H_
#define CROSSDOMAIN_LINKLAYER_BEHANDLING_AVB_RELAY_CUSTOMIZEDSRPRELAY_H_

//#include "core4inet/linklayer/beHandling/avb/relay/SRPRelay.h"
#include "inet/linklayer/ethernet/EtherFrame_m.h"
#include "inet/linklayer/ieee8021d/relay/Ieee8021dRelay.h"
#include "core4inet/linklayer/ethernet/avb/SRPFrame_m.h"

namespace CoRE4INET {

    class CustomizedSRPRelay : public virtual inet::Ieee8021dRelay {
    protected:
            /**
             * Module initialization, only requires 1 stage
             */
            virtual void initialize(int stage) override;
            /**
             * @brief Number of initialization stages, at least 1!
             *
             * @return at least 1, more when parent moduile requires more stages.
             */
            virtual int numInitStages() const override
            {
                return 1 > inet::Ieee8021dRelay::numInitStages() ? 1 : inet::Ieee8021dRelay::numInitStages();
            }

            /**
             *
             */
            virtual void handleMessage(cMessage * msg) override;
        private:
            void dispatchSRP(SRPFrame * srp);

            void deliverSRP(inet::EtherFrame * frame);

        private:
            void newDispatch(inet::EtherFrame *frame, unsigned int portNum);
    };
}



#endif /* CROSSDOMAIN_LINKLAYER_BEHANDLING_AVB_RELAY_CUSTOMIZEDSRPRELAY_H_ */
