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
    
    JDFA& Regulation(const char *reg);
    
    JNetwork<int, char>& ObtainDFA();
    
    static void TransformRegulation2NFA(const JString& regulation, JGraph<char>& NFA, JSet<int>& firstStatus);
    
    static void TransformNFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, const JMap<int, int>& empty2lable, JNetwork<int, char>& DFA);
    
private:
    
    JString regulation;
    JNetwork<int, char> mDFA;
    
    /*
        1 将正则表达式转为语法树
     */
    static bool OperatorPrecede(char op1, char op2);
    
    static JBinaryTree<JDFARegNode> * CreateNodeCharacter(const JString& reg, int index);
    
    static JBinaryTree<JDFARegNode> * CreateNodeOperator(char op, JStack<JBinaryTree<JDFARegNode> *>& nodes);
    
    static JBinaryTree<JDFARegNode> * HandleReg2Syntax(const JString& reg);
    
    static JBinaryTree<JDFARegNode> * Reg2Syntax(const JString& reg, int& i, char endChar);
    
    /*
        2.1 从语法树计算nullable、firstPos、lastPos
     */
    static bool NodeNullable(JBinaryTree<JDFARegNode> *tree);
    
    static void ObtainNodeFirstPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeLastPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeNullableAndMergeFLPosition(JBinaryTree<JDFARegNode> *tree);
    
    /*
        2.2 从语法树计算followPos
     */
    static void ObtainNodeFollowPosition(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    static void MergeNodeFollowPosition(JBinaryTree<JDFARegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        2.3 后序遍历语法树，计算nullable、firstPos、lastPos、followPos
     */
    class Translator : public JBinaryTree<JDFARegNode>::Interface {
    private:
        
        JGraph<int> followPos;
        // pos2ver由reg字符位置映射到followPos顶点位置
        JMap<int, int> pos2ver;
        
    public:
        
        virtual void Visit(JBinaryTree<JDFARegNode> *tree) {
            JDFA::ObtainNodeNullableAndMergeFLPosition(tree);
            JDFA::MergeNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
        
        /*
            2.4 遍历后，计算NFA、firstStatus
         */
        void ObtainNFAAndFirstStatus(JBinaryTree<JDFARegNode> *tree, const JString& regulation, JGraph<char>& NFA, JSet<int>& firstStatus);
        
    };
    
    /*
        3 由NFA，从firstStatus开始，转化成DFA
     */
    static int CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status);
    
    static void CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch);
    
    static void CreateDFAFollowAccept(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int flag);
    
    static void ClassifyDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify);
    
    static void HandleNFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, const JMap<int, int>& empty2lable, JNetwork<int, char>& DFA);
    
};

#endif /* DFA_hpp */
