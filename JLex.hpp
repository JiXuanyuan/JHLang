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
//        JDFA dfa2("(q|w)*");
//        JNetwork<int, char>& net2 = dfa2.ObtainDFA();
//        LOG_INFO(net2);
        
//        JString str2("12343215344325215");
//        
//        follow(str2, net2);
        
        LOG_INFO("==============Hello world!==============");
    }
    
    void follow(const JString& str, const JNetwork<int, char>& net) {
        
        int status = 0;
        
        // 检测字符
        for (int i = 0; i < str.Length(); i++) {
            char ch = str.Get(i);
            LOG_INFO("ch: ", ch);
            
            int outDegree = net.NextVertex(status, ch);
            LOG_INFO("outDegree: ", outDegree);
            if (outDegree == JLIST_FALG_NOT_EXIST) {
                LOG_INFO("err, status: ", status, ", ch: ", ch);
                return;
            }
            status = outDegree;
        }
        
        // 检测空标志
        char ch = '\0';
        LOG_INFO("ch: ", ch);
        
        int outDegree = net.NextVertex(status, ch);
        LOG_INFO("outDegree: ", outDegree);
        if (outDegree == JLIST_FALG_NOT_EXIST) {
            LOG_INFO("err, status: ", status, ", ch: ", ch);
            return;
        }
        status = outDegree;
        
        // 判断是否为终止状态
        LOG_INFO("ver: ", net.Get(status).value);
        if (net.Get(status).value == -1) {
            LOG_INFO("!!!!!!!!!ok, status: ", status, ", str: ", str);
        }
        
    }
    
};

#endif /* JLex_hpp */
