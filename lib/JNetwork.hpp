//
//  JNetwork.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/15.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JNetwork_hpp
#define JNetwork_hpp

#include "JLog.hpp"
#include "JList.hpp"
#include "JMap.hpp"

template<class Ver, class Arc>
class JNetworkVertex {
public:
    Ver ver;
    JMap<int, Arc> arcs;

    friend std::ostream& operator << (std::ostream& os, const JNetworkVertex& v) {
        os << "{ vertex: "<< v.ver << "; arcs: " << v.arcs << " }";
        return os;
    }
};

template<class Ver, class Arc>
class JNetwork : public JList<JNetworkVertex<Ver, Arc>> {
public:
    int AddVertex(Ver v) {
        LOG_FUNCTION_ENTRY;
        int i = JList<JNetworkVertex<Ver, Arc>>::Length();
        JList<JNetworkVertex<Ver, Arc>>::Add();
        JList<JNetworkVertex<Ver, Arc>>::GetTail().ver = v;
        return i;
    }
    
    void AddArc(int start, int end, Arc a) {
        LOG_FUNCTION_ENTRY;
        JList<JNetworkVertex<Ver, Arc>>::Get(start).arcs.Add(end, a);
    }
};

#endif /* JNetwork_hpp */
