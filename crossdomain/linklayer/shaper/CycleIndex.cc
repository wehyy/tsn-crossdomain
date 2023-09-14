/*
 * CycleIndex.cc
 *
 *  Created on: 20210309
 *      Author: twq
 */

#include "crossdomain/linklayer/shaper/CycleIndex.h"
#include "core4inet/base/NotifierConsts.h"
#include <omnetpp.h>

using namespace std;

namespace CoRE4INET {
    Define_Module(CycleIndex);

    int CycleIndex::getCycleIndex() {
        Enter_Method("getCycleIndex()");
        //return par("cycle_index");
        return this->cycleIndex;
    }

    void CycleIndex::setCycleIndex(int cycleIndex) {
        Enter_Method("setCycleIndex(%d)", cycleIndex);
        this->cycleIndex = cycleIndex;
        //par("cycle_index") = this->cycle_index;
    }

//  staticMappingTable could be modify,default empty
    void CycleIndex::initialize() {
        cXMLElement* element = par("staticMappingTable").xmlValue();
//        EV << "CycleIndex initialize() is called!" << endl;
//        EV << "GetSourceLocation: " << element->getSourceLocation() << endl;
//        EV << "GetXMLContent: " << element->getXML() << endl;
        for (cXMLElement * child = element->getFirstChild(); child; child = child->getNextSibling()) {
            string upstreamName = child->getAttribute("upstream");
            int upIndex = atoi(child->getAttribute("upIndex"));
            int localIndex = atoi(child->getAttribute("localIndex"));
            vector<int> upAndLocalIndex;
            upAndLocalIndex.push_back(upIndex);
            upAndLocalIndex.push_back(localIndex);
            this->cycleMappingTable.push_back(make_pair(upstreamName, upAndLocalIndex));
        }
//        for (int i = 0; i < this->cycleMappingTable.size(); i++){
//            EV << "cycleMappingTable: " << this->cycleMappingTable[i].first << endl;
//        }
    }

    int CycleIndex::queryLocalIndex(string upstream, int upIndex) {
        for (int i = 0; i < this->cycleMappingTable.size(); i++){
            if (this->cycleMappingTable[i].first.compare(upstream) == 0) {
                int upCycleIndex = this->cycleMappingTable[i].second[0];
                if (upCycleIndex == upIndex) {
                    return this->cycleMappingTable[i].second[1];
                }
            }
        }
        return -1;
    }
}



