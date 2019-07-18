//
//  JDFAMerger.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/17.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JDFAMerger_hpp
#define JDFAMerger_hpp

#include "JLog.hpp"
#include "JDFA.hpp"
#include "JList.hpp"
#include "JString.hpp"
#include "JNetwork.hpp"
#include "JGraph.hpp"
#include "JSet.hpp"

class JDFAAccepter {
public:
    int priority;
    JNetwork<int, char> adopter;
    
    friend std::ostream& operator << (std::ostream& os, const JDFAAccepter& ad) {
        os << "{ priority: "<< ad.priority << "; adopter: "<< ad.adopter << " }";
        return os;
    }
};

class JDFAMerger {
private:
    class JDFAIntend {
    public:
        int priority;
        JString lable;
        JString regulation;
        
        friend std::ostream& operator << (std::ostream& os, const JDFAIntend& it) {
            os << "{ priority: "<< it.priority << "; lable: "<< it.lable
                << "; regulation: " << it.regulation << " }";
            return os;
        }
    };
    
    JList<JDFAIntend> intends;
//    JList<JDFAAccepter> adopters;
    
public:
    
    void Intend(int priority, const char *label, const char *regulation) {
        int i = intends.Create();
        JDFAIntend& it = intends.Get(i);
        it.priority = priority;
        it.lable = label;
        it.regulation = regulation;
    }
    
    void Merger(int priority, JDFAAccepter& adopter) {
        
        JGraph<char> NFA;
        JSet<int> firstStatus;
        
        int l = intends.Length();
        for (int i = 0; i < l; i++) {
            JDFAIntend& it = intends.Get(i);
            
            if (it.priority == priority) {
                LOG_INFO("intends: ", it);
                MergerObtainNFA(NFA, firstStatus, it.regulation, i);
            }
        }
        
        // 再整合
        adopter.priority = priority;
        
        
        
        
        
        LOG_INFO("adopter: ", adopter);
    }
    
    void MergerObtainNFA(JGraph<char>& NFA, JSet<int>& firstStatus, JString& regulation, int flag) {
        
        
        
        
        
    }
    
};



#endif /* JDFAMerger_hpp */
