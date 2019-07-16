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
#include "JRegNode.hpp"

class JDFA {
public:
    
    JDFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    JDFA(const char *reg) {
        LOG_FUNCTION_ENTRY;
        this->reg.Assign(reg);
    }
    
    JDFA& Reg(const char *reg);
    
    JNetwork<int, char>& ObtainDFA();
    
private:
    
    JString reg;
    JNetwork<int, char> dfa;
    
    /*
        将正则表达式转为语法树
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<JRegNode> * CreateNodeCharacter(JString& reg, int index);
    
    JBinaryTree<JRegNode> * CreateNodeOperator(char op, JStack<JBinaryTree<JRegNode> *> & nodes);
    
    JBinaryTree<JRegNode> * Reg2Syntax(JString& reg);
    
    JBinaryTree<JRegNode> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    /*
        从语法树计算nullable、firstPos、lastPos
     */
    bool NodeNullable(JBinaryTree<JRegNode> *tree);
    
    void ObtainNodeFirstPosition(JBinaryTree<JRegNode> *tree, bool left, bool right);
    
    void ObtainNodeLastPosition(JBinaryTree<JRegNode> *tree, bool left, bool right);
    
    void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<JRegNode> *tree);
    
    /*
        从语法树计算followPos
     */
    void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    void ObtainNodeFollowPosition(JBinaryTree<JRegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        从语法树计算nullable、firstPos、lastPos、followPos，生成NFA
     */
//    friend class Translator;
    class Translator : public JBinaryTree<JRegNode>::Interface {
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
        
        virtual void Visit(JBinaryTree<JRegNode> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
            self->ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
        
        JGraph<char>& ObtainNFA();
        
        JSet<int>& ObtainFirstStatus(JBinaryTree<JRegNode> *tree);
        
    };
    
    int CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status);
    
    void CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch);
    
    void TransformDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify);
    
    void NFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, JNetwork<int, char>& DFA);
    
};

#endif /* DFA_hpp */
