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

class JDFAAdopter {
public:
    int priority;
    JNetwork<int, char> adopter;
    
    friend std::ostream& operator << (std::ostream& os, const JDFAAdopter& ad) {
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
//    JList<JDFAAdopter> adopters;
    
public:
    
    void Intend(int priority, const char *label, const char *regulation) {
        int i = intends.Create();
        JDFAIntend& it = intends.Get(i);
        it.priority = priority;
        it.lable = label;
        it.regulation = regulation;
    }
    
    void Merger(int priority, JDFAAdopter& adopter) {
        
        JGraph<char> NFA;
        JSet<int> firstStatus;
        
        for (JList<JDFAIntend>::Iterator i = intends.ObtainIterator(); i.HasNext();) {
            JDFAIntend& it = i.Next();
            if (it.priority == priority) {
                LOG_INFO("intends: ", it);
            }
        }
        
        adopter.priority = priority;
        LOG_INFO("adopter: ", adopter);
    }
};



#endif /* JDFAMerger_hpp */
