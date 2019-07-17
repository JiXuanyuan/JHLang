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

template<class Arc>
class JNetworkArcs {
public:
    Arc value;
    int outDegree;
//    int inDegree;
    
    friend std::ostream& operator << (std::ostream& os, const JNetworkArcs& a) {
        os << "{ value: "<< a.value << "; outDegree: " << a.outDegree << " }";
        return os;
    }
};

template<class Ver, class Arc>
class JNetworkVertex {
public:
    Ver value;
    JList<JNetworkArcs<Arc>> arcs;

    friend std::ostream& operator << (std::ostream& os, const JNetworkVertex& v) {
        os << "{ vertex: "<< v.value << "; arcs: " << v.arcs << " }";
        return os;
    }
};

template<class Ver, class Arc>
class JNetwork : public JList<JNetworkVertex<Ver, Arc>> {
public:
    int AddVertex(const Ver& value) {
        LOG_FUNCTION_ENTRY;
        int i = JList<JNetworkVertex<Ver, Arc>>::Add();
        JNetworkVertex<Ver, Arc>& ver = JList<JNetworkVertex<Ver, Arc>>::Get(i);
        ver.value = value;
        return i;
    }
    
    void AddArc(int start, int end, const Arc& value) {
        LOG_FUNCTION_ENTRY;
        JList<JNetworkArcs<Arc>>& arcs = JList<JNetworkVertex<Ver, Arc>>::Get(start).arcs;
        int i = arcs.Add();
        JNetworkArcs<Arc>& arc = arcs.Get(i);
        arc.outDegree = end;
        arc.value = value;
    }
    
    bool HasNextVertex(int start, const Arc& value) {
        return true;
    }
    
    bool NextVertex(int start, const Arc& value) {
        return true;
    }
    
};

#endif /* JNetwork_hpp */
