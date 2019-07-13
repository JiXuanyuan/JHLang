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
class JGraph {
private:
    class Vertex {
    public:
        T t;
        JSet<int> arcs;
    };
    JList<Vertex> vertexs;
public:
    JGraph() {
        
    }
    
    ~JGraph() {
        
    }
    
    void AddVerter(T t) {
        vertexs.Add();
        vertexs.GetTail().t = t;
    }
    
    void AddArc(int start, int end) {
        vertexs.Get(start).arcs.Add(end);
    }
    
    void Echo() {
        for (int i = 0; i < vertexs.Length(); i++) {
            LOG_INFO("v: ", vertexs.Get(i).t, ", arc: ", vertexs.Get(i).arcs);
        }
    }
    
};

#endif /* JGraph_hpp */
