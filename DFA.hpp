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
                os << "op = " << n.val;
            } else if (n.type == 2) {
                os << "ch = " << n.val << ", pos = " << n.regIndex;
            }
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
        比较 '&', '|', '*' 运算的优先级
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<Node> * CreateNodeCharacter(JString& reg, int index);
    
    JBinaryTree<Node> * CreateNodeOperator(char op, JStack<JBinaryTree<Node> *> & nodes);
    
    /*
        实现将正则表达式转为语法树
     */
    JBinaryTree<Node> * Reg2Syntax(JString& reg) {
        int i = 0;
        return Reg2Syntax(reg, i, '\0');
    }
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    bool NodeNullable(JBinaryTree<Node> *tree) {
        if (tree == NULL) {
            return true;
        }
        return tree->Node().nullable;
    }
    
    void ObtainNodeFirstPosition(JBinaryTree<Node> *tree, bool left, bool right) {
        if (left && tree->LeftChild() != NULL) {
            tree->Node().firstPos.Add(tree->LeftChild()->Node().firstPos);
        }
        
        if (right && tree->RightChild() != NULL) {
            tree->Node().firstPos.Add(tree->RightChild()->Node().firstPos);
        }
    }
    
    void ObtainNodeLastPosition(JBinaryTree<Node> *tree, bool left, bool right) {
        if (left && tree->LeftChild() != NULL) {
            tree->Node().lastPos.Add(tree->LeftChild()->Node().lastPos);
        }
        
        if (right && tree->RightChild() != NULL) {
            tree->Node().lastPos.Add(tree->RightChild()->Node().lastPos);
        }
    }
    
    void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<Node> *tree) {
        // 后序遍历处理，遍历过程tree不为NULL
        Node& n = tree->Node();
        
        if (n.IsCharacter()) {
            n.nullable = false;
            n.firstPos.Add(n.RegIndex());
            n.lastPos.Add(n.RegIndex());
            return;
        }
        
        if (n.IsOperator()) {
            char ch = n.Value();
            if (ch == '*') {
                n.nullable = true;
                // *操作，右节点为NULL，同(true, true)
                ObtainNodeFirstPosition(tree, true, false);
                ObtainNodeLastPosition(tree, true, false);
            } else if (ch == '|') {
                n.nullable = NodeNullable(tree->LeftChild()) || NodeNullable(tree->RightChild());
                ObtainNodeFirstPosition(tree, true, true);
                ObtainNodeLastPosition(tree, true, true);
            } else if (ch == '&') {
                n.nullable = NodeNullable(tree->LeftChild()) && NodeNullable(tree->RightChild());
                ObtainNodeFirstPosition(tree, true, NodeNullable(tree->LeftChild()));
                ObtainNodeLastPosition(tree, NodeNullable(tree->RightChild()), true);
            }
            return;
        }
        
        LOG_WARN("Node, type = 0");
    }
    
    void ObtainNodeFollowPosition(JBinaryTree<Node> *tree) {
        char ch = tree->Node().Value();
        if (ch == '*') {
            
        } else if (ch == '|') {
            
        }
    }
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    friend class Syntax2NFA;
    class Syntax2NFA : public JBinaryTree<Node>::Interface {
    public:
        DFA *self;
        JGraph<char> *nfa;
        Syntax2NFA(DFA *self, JGraph<char> *nfa) : self(self), nfa(nfa) {}
        virtual void Visit(JBinaryTree<Node> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
//            self->ObtainNodeFirstPosition(tree);
//            self->ObtainNodeLastPosition(tree);
            
            LOG_INFO(tree->Node(), ", Nullable: ", tree->Node().nullable);
            LOG_INFO("FirstPosition: ", tree->Node().firstPos);
            LOG_INFO("LastPosition: ", tree->Node().lastPos);
        }
    };
    
    
};

#endif /* DFA_hpp */
