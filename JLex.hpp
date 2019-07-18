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
//#include "JDFAMerger.hpp"

class JLex {
public:
    
    class JDFAIntend {
    public:
        int priority;
        JString lable;
        JString regulation;
        
        friend std::ostream& operator << (std::ostream& os, const JDFAIntend& it) {
            os << "{ priority: " << it.priority << "; lable: " << it.lable
                << "; regulation: " << it.regulation << " }";
            return os;
        }
    };
    
    class JToken {
    public:
        JString lable;
        JString value;
        
        friend std::ostream& operator << (std::ostream& os, const JToken& tk) {
            os << "< " << tk.lable << " , " << tk.value << " >";
            return os;
        }
    };
    
    void Test() {
        LOG_INFO("==============Hello world!==============");
        JDFA dfa("(a|b)*abb");
//        JDFA dfa2("(0|1|2|3|4|5|6|7|8|9|0)*");
        JNetwork<int, char>& net = dfa.ObtainDFA();
        LOG_INFO(net);
//
        JString str("abababaabb");
//        follow(str, net);
        
        LOG_INFO("==============Hello world!==============");
//        JDFAMerger merger;
//        merger.Intend(1, "if1", "qwe");
//        merger.Intend(1, "if2", "asd");
//        merger.Intend(1, "if3", "zxc");
//        merger.Intend(2, "add1", "+");
//        merger.Intend(3, "sub1", "=");
//
//
//        merger.Merger(1, networks[0]);
//        merger.Merger(2, networks[1]);
//        merger.Merger(3, networks[2]);
        

        Intend(1, "(", "\\(");
        Intend(1, ")", "\\)");
        Intend(1, "+", "+");
        Intend(1, "-", "-");
        Intend(1, "*", "\\*");
        Intend(1, "/", "/");
        Intend(1, "=", "=");
        Intend(2, "number", "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
        Intend(2, "if", "if");
        Intend(2, "while", "while");
        Intend(2, "for", "for");
        Intend(2, "id", "(q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m)(q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m)*");
        Intend(4, "换行", "\n\n*");
        Intend(4, "tab", "\t\t*");
        Intend(3, "空白", "  *");
        
        Merger(1, networks[0]);
        Merger(2, networks[1]);
        Merger(3, networks[2]);
        
//        Merger(1, networks[0], empty2lable[0]);
//        Merger(2, networks[1], empty2lable[1]);
//        Merger(3, networks[2], empty2lable[2]);
        
        ReadSection("zxc = if(qwe + asd) + 1231       \n24432");
        
        
        
        LOG_INFO("tokens: ", tokens);
    }
    
    
    
    
private:
    
    JList<JDFAIntend> intends;
    JNetwork<int, char> networks[3];
    int neti = 0, netj = 0;
    JString section;
//    int secl = 0, secr = 0;
    char peek;
    
    JString value;
    JList<JToken> tokens;
    
    void ReadSection(const char *section) {
        this->section.Assign(section);
        ReadPeek();
    }
    
    void ReadPeek() {
        int l = section.Length();
        
        
        LOG_INFO("networks[0]: ", networks[0]);
        LOG_INFO("networks[1]: ", networks[1]);
        LOG_INFO("networks[2]: ", networks[2]);
//        LOG_INFO("empty2lable[0]: ", empty2lable[0]);
//        LOG_INFO("empty2lable[1]: ", empty2lable[1]);
//        LOG_INFO("empty2lable[2]: ", empty2lable[2]);
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        
        
        for (int i = 0; i < l; i++) {
            peek = section.Get(i);
//            secr = i;
            Follow();
        }
        
//        secr++;
        
//        value.Merge(peek);
        if (AcceptEmpty()) {
            FollowEmpty();
            Export();
//            value.Clean();
        } else {
           LOG_WARN("ERR!!!");
//            value.Clean();
        }
    }
    
    
    void Follow() {
        
        for (int i = 2; i > neti; i--) {
            if (AcceptBetter(i)) {
                if (AcceptEmpty()) {
                    FollowEmpty();
                    Export();
//                    value.Clean();
                } else {
                    LOG_WARN("ERR!!!");
//                    value.Clean();
                }
                
                
                FollowBetter(i);
                
                return;
            }
        }
        
        if (AcceptPeek()) {
            FollowPeek();
            return;
        }
        
        for (int i = neti - 1; i >= 0; i--) {
            
            if (AcceptBetter(i)) {
                if (AcceptEmpty()) {
                    FollowEmpty();
                    Export();
                } else {
                    LOG_WARN("ERR!!!");
                }
                
                FollowBetter(i);
                
                return;
            }
        }
        
        
        if (AcceptEmpty()) {
            FollowEmpty();
            Export();
        } else {
            LOG_WARN("ERR!!!");
        }
        
    }
    
    bool AcceptBetter(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[i].NextVertex(0, peek) != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowBetter(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        value.Merge(peek);
        neti = i;
        netj = networks[neti].NextVertex(0, peek);
    }
    
    bool AcceptPeek() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[neti].NextVertex(netj, peek) != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowPeek() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        value.Merge(peek);
        netj = networks[neti].NextVertex(netj, peek);
    }
    
    bool AcceptEmpty() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[neti].NextVertex(netj, '\0') != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowEmpty() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
//        value.Merge(peek);
        netj = networks[neti].NextVertex(netj, '\0');
    }
    
    void Export() {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        int v = networks[neti].Get(netj).value;
        LOG_INFO("OK!!! value: ", v);
        
        JDFAIntend& it = intends.Get(v);
        LOG_INFO("OK!!! intends: ", it);
        
        int i = tokens.Create();
        JToken& tk = tokens.Get(i);
        tk.lable.Assign(it.lable);
        tk.value.Assign(value);
        value.Clean();
        LOG_INFO("OK!!! tokens: ", tk);
        
    }
    
    void Token(const JDFAIntend& intend, const char *regulation) {
        
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
//
//    bool AcceptAndFollow(const JNetwork<int, char>& net, int& status, char ch) {
//
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
//        LOG_INFO("accept, status: ", status, "; ver: ", net.Get(status).value);
//
//        return true;
//    }
    
    


    
    
    void Intend(int priority, const char *label, const char *regulation) {
        int i = intends.Create();
        JDFAIntend& it = intends.Get(i);
        it.priority = priority;
        it.lable = label;
        it.regulation = regulation;
    }
    
    void Merger(int priority, JNetwork<int, char>& adopter) {
//    void Merger(int priority, JNetwork<int, char>& adopter) {
        JGraph<char> NFA;
        JSet<int> firstStatus;
        JMap<int, int> empty2lable;
        
        int l = intends.Length();
        for (int i = 0; i < l; i++) {
            JDFAIntend& it = intends.Get(i);
            
            if (it.priority == priority) {
                LOG_INFO("intends: ", it);
                
                JDFA::ObtainNFA(NFA, firstStatus, it.regulation);
                // 每次取得新的NFA，末尾节点的标志都为'\0'
                empty2lable.Add(NFA.Length() - 1, i);
            }
        }
        LOG_INFO("empty2lable: ", empty2lable);
        
        // 将以整合的NFA转换为DFA
        //        adopter.priority = priority;
        
        JDFA::TransformNFA2DFA(NFA, firstStatus, empty2lable, adopter);
        
        LOG_INFO("adopter: ", adopter);
    }
    
};

#endif /* JLex_hpp */
