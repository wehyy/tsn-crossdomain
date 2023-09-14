/*
 * CycleIndex.h
 *
 *  Created on: 2021��3��9��
 *      Author: twq
 */

#ifndef CROSSDOMAIN_LINKLAYER_SHAPER_CYCLEINDEX_H_
#define CROSSDOMAIN_LINKLAYER_SHAPER_CYCLEINDEX_H_

#include <omnetpp.h>

using namespace omnetpp;
using namespace std;

namespace CoRE4INET {

    class CycleIndex : public cSimpleModule {
        private:
            int cycleIndex = 0;
            vector<pair<string, vector<int>>> cycleMappingTable;     // cycle mapping table

        public:
            void setCycleIndex(int cycleIndex);
            int getCycleIndex();
            virtual void initialize() override;
            int queryLocalIndex(string upstream, int upIndex);
    };
}



#endif /* IEEE8021QCQF_LINKLAYER_SHAPER_CYCLEINDEX_H_ */
