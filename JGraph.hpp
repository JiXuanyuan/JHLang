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
    T t;
    JSet<int> arcs;
    
    friend std::ostream& operator << (std::ostream& os, const JGraphVertex& v) {
        os << "{ vertex: "<< v.t << "; arcs: " << v.arcs << " }";
        return os;
    }
};

template<class T>
class JGraph : public JList<JGraphVertex<T>> {
public:
    int AddVerter(T t) {
        LOG_FUNCTION_ENTRY;
        int i = JList<JGraphVertex<T>>::Length();
        JList<JGraphVertex<T>>::Add();
        JList<JGraphVertex<T>>::GetTail().t = t;
        return i;
    }
    
    void AddArc(int start, int end) {
        LOG_FUNCTION_ENTRY;
        JList<JGraphVertex<T>>::Get(start).arcs.Add(end);
    }
    
//    Vertex& Get(int index) {
//        return vertexs._Get(index);
//    }
    
//    Vertex& Get(int index) {
//        return vertexs.Get(index);
//    }
    
//    void Echo() {
//        int l = JList<JGraphVertex<T>>::Length();
//        for (int i = 0; i < l; i++) {
//            JGraphVertex<T>& p = JList<JGraphVertex<T>>::Get(i);
//            LOG_INFO("v", i, ": ", p.t, ", arc: ", p.arcs);
//        }
//    }
    
};

#endif /* JGraph_hpp */
