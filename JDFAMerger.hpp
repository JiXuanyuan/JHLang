////
////  JDFAMerger.hpp
////  JHLang
////
////  Created by 陈佳辉 on 2019/7/17.
////  Copyright © 2019 陈佳辉. All rights reserved.
////
//
//#ifndef JDFAMerger_hpp
//#define JDFAMerger_hpp
//
//#include "JLog.hpp"
//#include "JDFA.hpp"
//#include "JList.hpp"
//#include "JString.hpp"
//#include "JNetwork.hpp"
//#include "JGraph.hpp"
//#include "JSet.hpp"
//#include "JMap.hpp"
//
//class JDFAAccepter {
//public:
//    int priority;
//    JNetwork<int, char> adopter;
//    
//    friend std::ostream& operator << (std::ostream& os, const JDFAAccepter& ad) {
//        os << "{ priority: "<< ad.priority << "; adopter: "<< ad.adopter << " }";
//        return os;
//    }
//};
//
//class JDFAMerger {
//private:
//    class JDFAIntend {
//    public:
//        int priority;
//        JString lable;
//        JString regulation;
//        
//        friend std::ostream& operator << (std::ostream& os, const JDFAIntend& it) {
//            os << "{ priority: "<< it.priority << "; lable: "<< it.lable
//                << "; regulation: " << it.regulation << " }";
//            return os;
//        }
//    };
//    
//    JList<JDFAIntend> intends;
////    JList<JDFAAccepter> adopters;
//    
//public:
//    
//    void Intend(int priority, const char *label, const char *regulation) {
//        int i = intends.Create();
//        JDFAIntend& it = intends.Get(i);
//        it.priority = priority;
//        it.lable = label;
//        it.regulation = regulation;
//    }
//    
//    void Merger(int priority, JNetwork<int, char>& adopter) {
//        JGraph<char> NFA;
//        JSet<int> firstStatus;
//        JMap<int, int> empty2lable;
//        
//        int l = intends.Length();
//        for (int i = 0; i < l; i++) {
//            JDFAIntend& it = intends.Get(i);
//            
//            if (it.priority == priority) {
//                LOG_INFO("intends: ", it);
//                
//                JDFA::ObtainNFA(NFA, firstStatus, it.regulation);
//                // 每次取得新的NFA，末尾节点的标志都为'\0'
//                empty2lable.Add(NFA.Length() - 1, i);
//            }
//        }
//        LOG_INFO("empty2lable: ", empty2lable);
//        
//        // 将以整合的NFA转换为DFA
////        adopter.priority = priority;
//        
//        JDFA::TransformNFA2DFA(NFA, firstStatus, empty2lable, adopter);
//        
//        LOG_INFO("adopter: ", adopter);
//    }
//    
//};
//
//
//
//#endif /* JDFAMerger_hpp */
