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
#include "JDFAMerger.hpp"

class JLex {
public:
    
    void Test() {
        LOG_INFO("==============Hello world!==============");
        JDFA dfa("(a|b)*abb");
        JNetwork<int, char>& net = dfa.ObtainDFA();
        LOG_INFO(net);
//
        JString str("abababaabb");
        follow(str, net);
        
        LOG_INFO("==============Hello world!==============");
        JDFA dfa2("(0|1|2|3|4|5|6|7|8|9|0)*");
//        JDFA dfa2("(q|w)*");
        JNetwork<int, char>& net2 = dfa2.ObtainDFA();
        LOG_INFO(net2);

        JString str2("215eqwq");
        follow(str2, net2);
        
//        JString str2("12343215344325215");
//        follow(str2, net2);
        
        LOG_INFO("==============Hello world!==============");
        
        JDFAMerger merger;
        JDFAAdopter adp1, adp2, adp3, adp4;
        merger.Intend(1, "qif1", "iweqf");
        merger.Intend(2, "wiwf3", "ireqf");
        merger.Intend(3, "eif", "if123");
        merger.Intend(1, "rif4r", "2312if");
        merger.Intend(1, "tif", "iffew");
        merger.Intend(1, "yif", "iffwefq");
        merger.Intend(1, "uif", "iweqwf");
        merger.Intend(9, "iif", "i123f");
        merger.Intend(9, "oif", "i123f");
        merger.Intend(1, "pif", "i213f");
        
        merger.Merger(1, adp1);
        merger.Merger(2, adp2);
        merger.Merger(3, adp3);
        merger.Merger(9, adp4);
        
    }
    
    void follow(const JString& str, const JNetwork<int, char>& net) {
        
        int status = 0;
        
        // 检测字符
        for (int i = 0; i < str.Length(); i++) {
            char ch = str.Get(i);
            LOG_INFO("ch: ", ch);
            
            if (!AcceptAndFollow(net, status, ch)) {
                AcceptAndFollowEmpty(net, status);
                status = 0;
//                i--;
            }
            
            LOG_INFO("status: ", status);
        }
        
        AcceptAndFollowEmpty(net, status);
    }
    
    bool AcceptAndFollow(const JNetwork<int, char>& net, int& status, char ch) {
        
        LOG_INFO("ch: ", ch);
        
        int outDegree = net.NextVertex(status, ch);
        LOG_INFO("outDegree: ", outDegree);
        if (outDegree == JLIST_FALG_NOT_EXIST) {
            LOG_INFO("not accept, status: ", status, ", ch: ", ch);
            return false;
        }
        
        status = outDegree;
        LOG_INFO("accept, status: ", status, "; ver: ", net.Get(status).value);
        
        return true;
    }
    
    bool AcceptAndFollowEmpty(const JNetwork<int, char>& net, int& status) {
//        char ch = '\0';
//        LOG_INFO("ch: ", ch);
//
//        int outDegree = net.NextVertex(status, ch);
//        LOG_INFO("outDegree: ", outDegree);
//        if (outDegree == JLIST_FALG_NOT_EXIST) {
//            LOG_INFO("not accept, status: ", status, ", ch: ", ch);
//            return false;
//        }
//
//        status = outDegree;
//        LOG_INFO("!!!!!accept, status: ", status, "; ver: ", net.Get(status).value);
//
        bool ret = AcceptAndFollow(net, status, '\0');
        if (ret) {
            LOG_INFO("!!!!!accept, status: ", status, "; ver: ", net.Get(status).value);
        }
        return ret;
    }
    
};

#endif /* JLex_hpp */
