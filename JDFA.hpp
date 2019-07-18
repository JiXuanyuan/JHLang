//
//  DFA.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef DFA_hpp
#define DFA_hpp

#include "JLog.hpp"
#include "JString.hpp"
#include "JBinaryTree.hpp"
#include "JStack.hpp"
#include "JSet.hpp"
#include "JGraph.hpp"
#include "JMap.hpp"
#include "JNetwork.hpp"
#include "JDFARegNode.hpp"

class JDFA {
public:
    
    JDFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    JDFA(const char *reg) {
        LOG_FUNCTION_ENTRY;
        this->regulation.Assign(reg);
    }
    
    JDFA& Regulation(const char *reg);
    
//    JDFA& Separator(const char *sep) {
//        LOG_FUNCTION_ENTRY;
////        if (!this->separator.Assign(sep)) {
////            LOG_WARN("not Assign");
////        }
//        return *this;
//    }
    
    JNetwork<int, char>& ObtainDFA();
    
    static void ObtainNFA(JGraph<char>& NFA, JSet<int>& firstStatus, JString& regulation, int flag) {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("regulation = ", regulation);
        
        // 获得语法树
        JBinaryTree<JDFARegNode>::Root synt = Reg2Syntax(regulation);
        
        // 由语法树遍历，获得NFA
//        Translator tran;
//        synt.TraversePostorder(&tran);
//        JGraph<char>& nfa = tran.ObtainNFA(synt.Tree());
//        JSet<int>& first = tran.ObtainFirstStatus(synt.Tree());
        
        // 由NFA转换为DFA
//        NFA2DFA(nfa, first, dfa);
        
        
        
    }
    
    
private:
    
    JString regulation;
//    JString separator;
    JNetwork<int, char> dfa;
    
    /*
        将正则表达式转为语法树
     */
    static bool OperatorPrecede(char op1, char op2);
    
    static JBinaryTree<JDFARegNode> * CreateNodeCharacter(const JString& reg, int index);
    
    static JBinaryTree<JDFARegNode> * CreateNodeOperator(char op, JStack<JBinaryTree<JDFARegNode> *> & nodes);
    
    static JBinaryTree<JDFARegNode> * Reg2Syntax(const JString& reg);
    
    static JBinaryTree<JDFARegNode> * Reg2Syntax(const JString& reg, int& i, char endChar);
    
    /*
        从语法树计算nullable、firstPos、lastPos
     */
    static bool NodeNullable(JBinaryTree<JDFARegNode> *tree);
    
    static void ObtainNodeFirstPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeLastPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<JDFARegNode> *tree);
    
    /*
        从语法树计算followPos
     */
    static void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    static void ObtainNodeFollowPosition(JBinaryTree<JDFARegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        从语法树计算nullable、firstPos、lastPos、followPos，生成NFA
     */
//    friend class Translator;
    class Translator : public JBinaryTree<JDFARegNode>::Interface {
    private:
        
        JDFA *self;
        // 1.followPos顶点中存放着reg字符位置，可映射到reg字符
        // 2.pos2ver由reg字符位置映射到followPos顶点
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        JGraph<char> nfa;
        JSet<int> firstStat;
        
    public:
        
        Translator(JDFA *self) : self(self) {}
        
        virtual void Visit(JBinaryTree<JDFARegNode> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
            self->ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
        
        JGraph<char>& ObtainNFA(JBinaryTree<JDFARegNode> *tree);
        
        JSet<int>& ObtainFirstStatus(JBinaryTree<JDFARegNode> *tree);
        
    };
    
    /*
        由NFA，从firstStatus开始，转化成DFA
     */
    int CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status);
    
    void CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch);
    
    void CreateDFAFollowAccept(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start);
    
    void TransformDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify);
    
    void NFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, JNetwork<int, char>& DFA);
    
};

#endif /* DFA_hpp */
