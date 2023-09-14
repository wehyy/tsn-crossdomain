/*
 * CustomizedSourceApp.h
 *
 *  Created on: 20210417
 *      Author: twq
 */

#ifndef CROSSDOMAIN_APPLICATIONS_TRAFFICSOURCE_CUSTOMIZEDSOURCEAPP_H_
#define CROSSDOMAIN_APPLICATIONS_TRAFFICSOURCE_CUSTOMIZEDSOURCEAPP_H_

//#include "core4inet/applications/trafficsource/IEEE8021Q/IEEE8021QTrafficSourceApp.h"

//CoRE4INET
#include "core4inet/applications/trafficsource/base/TrafficSourceAppBase.h"

//INET
#include "inet/linklayer/common/MACAddress.h"

namespace CoRE4INET {

class CustomizedSourceApp : public virtual TrafficSourceAppBase
{
private:
        /**
         * @brief caches sendInterval parameter
         */
        simtime_t sendInterval;

        /**
         * @brief caches destAddress parameter
         */
        inet::MACAddress destAddress;

        /**
         * @brief caches priority parameter
         */
        uint8_t priority;
        /**
         * @brief caches vid parameter
         */
        uint16_t vid;

        /**
         * @brief Number of priorities.
         */
        unsigned int numPCP;

    public:
        /**
         * @brief Constructor of IEEE8021QTrafficSourceApp
         */
        CustomizedSourceApp();

    protected:
        /**
         * @brief Signals that are emitted when a frame is send.
         */
        std::vector<simsignal_t> txQPcpPkSignals;

        /**
         * @brief Initialization of the module. Sends activator message
         */
        virtual void initialize() override;

        /**
         * @brief handle self messages to send frames
         *
         *
         * @param msg incoming self messages
         */
        virtual void handleMessage(cMessage *msg) override;

        /**
         * @brief Generates and sends a new Message.
         *
         * The message is sent to the buffer.
         * The size is defined by the payload parameter of the module.
         */
        virtual void sendMessage();

        /**
         * @brief Indicates a parameter has changed.
         *
         * @param parname Name of the changed parameter or nullptr if multiple parameter changed.
         */
        virtual void handleParameterChange(const char* parname) override;
};

}



#endif /* CROSSDOMAIN_APPLICATIONS_TRAFFICSOURCE_CUSTOMIZEDSOURCEAPP_H_ */
