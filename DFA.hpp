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

class DFA {
private:
    class Node {
    private:
        static const int TYPE_OPER = 1;
        static const int TYPE_CHER = 2;
        // type: 1.操作符节点，2.符号位置节点
        int type = 0;
        char val;
        int regIndex;
    public:
        
        bool nullable;
        JSet<int> firstPos;
        JSet<int> lastPos;
        
        void Assign(char op) {
            type = TYPE_OPER;
            val = op;
        }
        
        void Assign(char ch, int pos) {
            type = TYPE_CHER;
            val = ch;
            regIndex = pos;
        }
        
        bool IsOperator() {
            return type == TYPE_OPER;
        }
        
        bool IsCharacter() {
            return type == TYPE_CHER;
        }
        
        char Value() {
            return val;
        }
        
        int RegIndex() {
            return regIndex;
        }
        
        friend std::ostream& operator << (std::ostream& os, const Node& n) {
            if (n.type == 1) {
                os << "{ op = " << n.val;
            } else if (n.type == 2) {
                os << "{ ch = " << n.val << ", pos = " << n.regIndex;
            }
            os << ", nullable = " << n.nullable;
            os << ", firstPos = " << n.firstPos;
            os << ", lastPos = " << n.lastPos << " }";
            return os;
        }
    };
    
    JString reg;
    JGraph<char> dfa;
    
    /*
     实现将正则表达式转为语法树，旧函数
     */
//    JBinaryTree<char> * Reg2Syntax_old(JString& reg, int offset, int* end);
    
//    void hello() {
//        LOG_INFO("hello, reg = ", this->reg);
//    }
    
public:
    
    DFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    ~DFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    friend class PNode;
    class PNode : public JBinaryTree<Node>::Interface {
    public:
        virtual void Visit(JBinaryTree<Node> *tree) {
            LOG_INFO(tree->Node());
        }
    };
    
    
    
    
    
    bool Reg(const char *reg) {
        LOG_FUNCTION_ENTRY;
        if (!this->reg.Assign(reg)) {
            LOG_WARN("not Assign");
            return false;
        }
        
        LOG_INFO("reg = ", this->reg);
        return true;
    }
    
    bool Reg2NFA() {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("reg = ", reg);
        
        JBinaryTree<Node>::Root root;
        root = Reg2Syntax(reg);
        
        PNode pn;
        root.TraversePostorder(&pn);
        
        JGraph<char> nfa;
        Syntax2NFA syn2nfa(this, &nfa);
        root.TraversePostorder(&syn2nfa);
        
        return true;
    }
    
    
    /*
     实现将正则表达式转为语法树
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<Node> * CreateNodeCharacter(JString& reg, int index);
    
    JBinaryTree<Node> * CreateNodeOperator(char op, JStack<JBinaryTree<Node> *> & nodes);
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg) {
        int i = 0;
        return Reg2Syntax(reg, i, '\0');
    }
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    /*
     实现将正则表达式转为语法树
     */
    bool NodeNullable(JBinaryTree<Node> *tree);
    
    void ObtainNodeFirstPosition(JBinaryTree<Node> *tree, bool left, bool right);
    
    void ObtainNodeLastPosition(JBinaryTree<Node> *tree, bool left, bool right);
    
    void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<Node> *tree);
    
    /*
     实现将正则表达式转为语法树
     */
    void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    void ObtainNodeFollowPosition(JBinaryTree<Node> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    friend class Syntax2NFA;
    class Syntax2NFA : public JBinaryTree<Node>::Interface {
    public:
        DFA *self;
        JGraph<char> *nfa;
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        
        Syntax2NFA(DFA *self, JGraph<char> *nfa) : self(self), nfa(nfa) {}
        
        virtual void Visit(JBinaryTree<Node> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
            self->ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
    };
    
    
};

#endif /* DFA_hpp */
