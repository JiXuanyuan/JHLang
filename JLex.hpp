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
    
    class JIntend {
    public:
        int priority;
        JString lable;
        JString regulation;
        
        friend std::ostream& operator << (std::ostream& os, const JIntend& it) {
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
    
 /*   void Test() {
//        LOG_INFO("==============Hello world!==============");
//        JDFA dfa;
//        dfa.Regulation("(a|b)*abb");
//        JNetwork<int, char>& net = dfa.ObtainDFA();
//        LOG_INFO(net);
//        JString str("abababaabb");
//
//        LOG_INFO("==============Hello world!==============");
//        JDFA dfa2;
//        dfa2.Regulation("q(\0*)w", 6);
//        JNetwork<int, char>& net2 = dfa2.ObtainDFA();
//        LOG_INFO(net2);
        LOG_INFO("==============Hello world!==============");

        Intend(0, "空白", "( |\n|\t)( |\n|\t)*");
        Intend(1, "左括号", "\\(");
        Intend(1, "右括号", "\\)");
        Intend(1, "加", "+");
        Intend(1, "减", "-");
        Intend(1, "乘", "\\*");
        Intend(1, "除", "/");
        Intend(1, "等于", "=");
        Intend(1, "相等", "==");
        Intend(1, "分隔符", ";");
        Intend(2, "数字", "(1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
        Intend(2, "如果", "if");
        Intend(2, "条件循环", "while");
        Intend(2, "循环", "for");
        Intend(2, "关键词", "(q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m)(q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m)*");
        
        int i = intends.Create();
        JIntend& it = intends.Get(i);
        it.priority = 3;
        it.lable = "字符串";
        it.regulation.Assign("\"(\0*)\"", 6);
        
        Merger(0, networks[0]);
        Merger(1, networks[1]);
        Merger(2, networks[2]);
        Merger(3, networks[3]);
        
//        Merger(3, networks[0]);

        ReadSection("zxc = if(qwe + asd) + 12==31       \n\t24432    dqwcqwv = \"2gggggqqq1\"  = dw");
     
        LOG_PRINT("tokens: ", tokens);
    }*/
    
    
    void Input(const char *input) {
        Intend(0, "空白", "( |\n|\t)( |\n|\t)*");
        
        Intend(1, "数字", "(0|1|2|3|4|5|6|7|8|9)(0|1|2|3|4|5|6|7|8|9)*");
        Intend(1, "关键词", "(_|q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m|Q|W|E|R|T|Y|U|I|O|P|A|S|D|F|G|H|J|K|L|Z|X|C|V|B|N|M)(_|q|w|e|r|t|y|u|i|o|p|a|s|d|f|g|h|j|k|l|z|x|c|v|b|n|m|Q|W|E|R|T|Y|U|I|O|P|A|S|D|F|G|H|J|K|L|Z|X|C|V|B|N|M|0|1|2|3|4|5|6|7|8|9)*");
        Intend(1, "如果", "if");
        Intend(1, "条件循环", "while");
        Intend(1, "循环", "for");
        Intend(1, "类型定义", "typedef");
        Intend(1, "整型", "int");
        Intend(1, "长整型", "long");
        Intend(1, "字符型", "char");
        Intend(1, "浮点型", "float");
        Intend(1, "双精度浮点型", "double");
        Intend(1, "静态", "static");
        Intend(1, "常量", "const");
        Intend(1, "结构体", "struct");
        Intend(1, "联合体", "union");
        Intend(1, "枚举", "enum");
        
        Intend(2, "加", "+");
        Intend(2, "减", "-");
        Intend(2, "乘", "\\*");
        Intend(2, "除", "/");
        Intend(2, "等于", "=");
        Intend(2, "相等", "==");
        Intend(2, "大于", ">");
        Intend(2, "大于等于", ">=");
        Intend(2, "小于", "<");
        Intend(2, "小于等于", "<=");
        Intend(2, "自加1", "++");
        Intend(2, "自减1", "--");
        Intend(2, "分隔符", ";");
        
        Intend(3, "左方括号", "\\[");
        Intend(3, "右方括号", "\\]");
        Intend(4, "左大括号", "\\{");
        Intend(4, "右大括号", "\\}");
        Intend(5, "左括号", "\\(");
        Intend(5, "右括号", "\\)");
        
        int i = intends.Create();
        JIntend& it = intends.Get(i);
        it.priority = 6;
        it.lable = "字符串";
        it.regulation.Assign("\"(\0*)\"", 6);
        
        for (int i = 0; i < netlength; i++) {
            Merger(i, networks[i]);
        }
        
        ReadSection(input);
        LOG_PRINT("input: ", input);
        LOG_PRINT("tokens: ", tokens);
    }
    
    
private:
    
    JList<JIntend> intends;
    
    static const int netlength = 7;
    JNetwork<int, char> networks[netlength];
    int neti = 0, netj = 0;
    char peek = '\0';
    int pos = 0;
    
    JString section;
    JString value;
    JList<JToken> tokens;
    
    void ReadSection(const char *section) {
        this->section.Assign(section);
        ReadPeek();
    }
    
    void ReadPeek() {
        int l = section.Length();
        
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        
        for (int i = 0; i < l; i++) {
            peek = section.Get(i);
            pos = i;
            Follow();
        }
        TryExport();
    }
    
    void Follow() {
        
        for (int i = netlength - 1; i > neti; i--) {
            if (AcceptBetterPeek(i)) {
                TryExport();
                FollowBetterPeek(i);
                return;
            } else if (AcceptBetterEmpty(i)) {
                TryExport();
                FollowBetterEmpty(i);
                return;
            }
        }
        
        if (AcceptPeek()) {
            FollowPeek();
            return;
        } else if (AcceptEmpty()) {
            // \0表示匹配所有
            FollowEmpty();
            return;
        }
        
        for (int i = neti - 1; i >= 0; i--) {
            if (AcceptBetterPeek(i)) {
                TryExport();
                FollowBetterPeek(i);
                return;
            } else if (AcceptBetterEmpty(i)) {
                TryExport();
                FollowBetterEmpty(i);
                return;
            }
        }
        
        TryExport();
        
        // 重置
        neti = 0;
        netj = 0;
    }
    
    bool AcceptBetterPeek(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[i].NextVertex(0, peek) != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowBetterPeek(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        value.Merge(peek);
        neti = i;
        netj = networks[neti].NextVertex(0, peek);
    }
    
    bool AcceptBetterEmpty(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        return networks[i].NextVertex(0, '\0') != JLIST_FALG_NOT_EXIST;
    }
    
    void FollowBetterEmpty(int i) {
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        value.Merge(peek);
        neti = i;
        netj = networks[neti].NextVertex(0, '\0');
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
        value.Merge(peek);
        netj = networks[neti].NextVertex(netj, '\0');
    }
    
    void TryExport() {
        if (networks[neti].Get(netj).value < 0) {
            if (pos == 0) {
                return;
            }
            
            LOG_WARN("ERR!!! position: ", pos);
            return;
        }
        
        LOG_INFO("neti: ", neti, "; netj: ", netj, "; peek: ", peek);
        int v = networks[neti].Get(netj).value;
        LOG_INFO("OK!!! value: ", v);
        
        JIntend& it = intends.Get(v);
        LOG_INFO("OK!!! intends: ", it);
        
        // 优先级0的 空白符
        if (it.priority == 0) {
            return;
        }
        
        int i = tokens.Create();
        JToken& tk = tokens.Get(i);
        tk.lable.Assign(it.lable);
        tk.value.Assign(value);
        value.Clean();
        LOG_INFO("OK!!! tokens: ", tk);
        
//        // 重置
//        neti = 0;
//        netj = 0;
    }
    
    void Intend(int priority, const char *label, const char *regulation) {
        int i = intends.Create();
        JIntend& it = intends.Get(i);
        it.priority = priority;
        it.lable = label;
        it.regulation = regulation;
    }
    
    void Merger(int priority, JNetwork<int, char>& adopter) {
        JGraph<char> NFA;
        JSet<int> firstStatus;
        JMap<int, int> empty2lable;
        
        int l = intends.Length();
        for (int i = 0; i < l; i++) {
            JIntend& it = intends.Get(i);
            
            if (it.priority == priority) {
                LOG_INFO("intends: ", it);
                
                JDFA::TransformRegulation2NFA(it.regulation, NFA, firstStatus);
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
