//
//  JGraph.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JGraph_hpp
#define JGraph_hpp

#include "JLog.hpp"
#include "JList.hpp"
#include "JSet.hpp"

template<class T>
class JGraphVertex {
public:
    T value;
    JSet<int> arcs;
    
    friend std::ostream& operator << (std::ostream& os, const JGraphVertex& v) {
        os << "{ vertex: "<< v.value << "; arcs: " << v.arcs << " }";
        return os;
    }
};

template<class T>
class JGraph : public JList<JGraphVertex<T>> {
public:
    int AddVerter(const T& value) {
        LOG_FUNCTION_ENTRY;
        int i = JList<JGraphVertex<T>>::Create();
        JGraphVertex<T>& ver = JList<JGraphVertex<T>>::Get(i);
        ver.value = value;
        return i;
    }
    
    void AddArc(int start, int end) {
        LOG_FUNCTION_ENTRY;
        JList<JGraphVertex<T>>::Get(start).arcs.Add(end);
    }
    
};

#endif /* JGraph_hpp */
