//
//  JLex.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/16.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JLex_hpp
#define JLex_hpp

#include "JLog.hpp"
#include "JDFA.hpp"

class JLex {
public:
    

    
    
    void Test() {
        LOG_INFO("==============Hello world!==============");
        JDFA dfa("(a|b)*abb");
        JNetwork<int, char>& net = dfa.ObtainDFA();
        LOG_INFO(net);
        
        JString str("abababaabb");
        
        follow(str, net);
        
        LOG_INFO("==============Hello world!==============");
//        JDFA dfa2("(0|1|2|3|4|5|6|7|8|9|0)*");
        JDFA dfa2("(q|w)*");
        JNetwork<int, char>& net2 = dfa2.ObtainDFA();
        LOG_INFO(net2);
        
//        JString str2("12343215344325215");
//        
//        follow(str2, net2);
        
        LOG_INFO("==============Hello world!==============");
    }
    
    void follow(const JString& str, const JNetwork<int, char>& net) {
        
        int stat = 0;
//        for (int i = 0; i < str.Length(); i++) {
//            LOG_INFO("ch: ", str.Get(i));
//            
//            JNetworkVertex<int, char>& V = net.Get(stat);
//            LOG_INFO("V: ", V);
//            for (JMap<int, char>::Iterator it = V.arcs.ObtainIterator(); it.HasNext();) {
//                JMapPair<int, char>& arc = it.Next();
//                
//                
//                if (arc.value == str.Get(i)) {
//                    stat = arc.key;
//                    LOG_INFO("tran stat: ", stat);
//                    break;
//                }
//                
//            }
//            
//        }
        
        LOG_INFO("======= stat: ", stat);
        
//        JNetworkVertex<int, char>& V = net.Get(stat);
//        LOG_INFO("V: ", V);
//        for (JMap<int, char>::Iterator it = V.arcs.ObtainIterator(); it.HasNext();) {
//            JMapPair<int, char>& arc = it.Next();
//
//            if (arc.value == '\0') {
//                stat = arc.key;
//                break;
//            }
//        }
        
        LOG_INFO("======= stat: ", stat);
        
        LOG_INFO("ver: ", net.Get(stat).value);
        if (net.Get(stat).value == -1) {
            LOG_INFO("!!!!!!!!!ok: ", stat);
        }
        
        LOG_INFO("======= stat: ", stat);
        
    }
    
};

#endif /* JLex_hpp */
