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
//        follow(str, net);
        
        LOG_INFO("==============Hello world!==============");
//        JDFA dfa2("(0|1|2|3|4|5|6|7|8|9|0)*");
////        JDFA dfa2("(q|w)*");
//        JNetwork<int, char>& net2 = dfa2.ObtainDFA();
//        LOG_INFO(net2);
//
//        JString str2("215eqwq");
//        follow(str2, net2);
        
//        JString str2("12343215344325215");
//        follow(str2, net2);
        
        
        LOG_INFO("==============Hello world!==============");
//
//        JDFAMerger merger;
//        JDFAAccepter adp1, adp2, adp3, adp4;
        JDFAMerger merger;
        merger.Intend(1, "if1", "qwe");
        merger.Intend(1, "if2", "asd");
        merger.Intend(1, "if3", "zxc");
        merger.Intend(2, "add1", "+");
        merger.Intend(3, "sub1", "=");
        
//        int i = networks.Create();
//        merger.Merger(1, networks.Get(i));
//        i = networks.Create();
//        merger.Merger(2, networks.Get(i));
//        i = networks.Create();
//        merger.Merger(3, networks.Get(i));
        merger.Merger(1, networks[0]);
        merger.Merger(2, networks[1]);
        merger.Merger(3, networks[2]);
        
        ReadSection("zxc");
    }
    
    JNetwork<int, char> networks[3];
    int neti = 0, netj = 0;
    JString section;
    int secs = 0, sece = 0;
    char peek;
    
    void ReadSection(const char *section) {
        this->section.Assign(section);
        ReadPeek();
    }
    
    void ReadPeek() {
        int l = section.Length();
        for (int i = 0; i < l; i++) {
            peek = section.Get(i);
            Follow();
        }
        
        if (AcceptEmpty()) {
            FollowEmpty();
            Export();
            return;
        }
        LOG_WARN("ERR!!!");
    }

    void Follow() {
        
//        for (int i = 0; i < neti; i++) {
//
//        }
        
        
        if (AcceptPeek()) {
            FollowPeek();
            return;
        }
        
        if (AcceptEmpty()) {
            FollowEmpty();
            Export();
            return;
        }
        
        LOG_WARN("ERR!!!");
    }
    
    bool AcceptPeek() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[neti].NextVertex(netj, peek) != JLIST_FALG_NOT_EXIST;
    }
    
    bool AcceptEmpty() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[neti].NextVertex(netj, '\0') != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowPeek() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        netj = networks[neti].NextVertex(netj, peek);
    }
    
    void FollowEmpty() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        netj = networks[neti].NextVertex(netj, '\0');
    }
    
    void Export() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        LOG_INFO("OK!!! value: ", networks[neti].Get(netj).value);
        neti = 0;
        netj = 0;
    }
    
//
//    bool Accept() {
//        return networks[neti].NextVertex(netj, peek) != JLIST_FALG_NOT_EXIST;
//    }
//
//    void FollowEmpty() {
//        int i = networks[neti].NextVertex(netj, '\0');
//        if(i == JLIST_FALG_NOT_EXIST) {
//            return;
//        }
//        netj = i;
//    }
    
    
    
//    bool Accept(int i) {
//        return networks[i].NextVertex(0, peek) == JLIST_FALG_NOT_EXIST;
//    }
//
//    bool FollowEmpty() {
//        return true;
//    }
//
//    void Change() {
//
//    }
    
//    bool AcceptAndFollowEmpty(const JNetwork<int, char>& net, int& status) {
//        bool ret = AcceptAndFollow(net, status, '\0');
//        if (ret) {
//            LOG_INFO("!!!!!accept, status: ", status, "; ver: ", net.Get(status).value);
//        }
//        return ret;
//    }
    
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
    
};

#endif /* JLex_hpp */
