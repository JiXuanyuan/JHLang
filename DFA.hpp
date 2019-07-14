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

class DFA {
private:
    class Node {
    public:
        // type: 1.操作符节点，2.符号位置节点
        int type = 0;
        char var;
        int regIndex;
        void Assign(char op) {
            type = 1;
            var = op;
        }
        void Assign(char ch, int pos) {
            type = 2;
            var = ch;
            regIndex = pos;
        }
        friend std::ostream& operator << (std::ostream& os, const Node& n) {
            if (n.type == 1) {
                os << "op = " << n.var;
            } else if (n.type == 2) {
                os << "ch = " << n.var << ", pos = " << n.regIndex;
            }
            return os;
        }
    };
    
    JString reg;
    
    void hello() {
        LOG_INFO("hello, reg = ", this->reg);
    }
    
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
        root.Traverse(&pn);
        
        Syntax2NFA syn2nfa(this);
        root.Traverse(&syn2nfa);
        
        return true;
    }
    
    /*
        实现将正则表达式转为语法树，旧函数
     */
    JBinaryTree<char> * Reg2Syntax_old(JString& reg, int offset, int* end);
    
    /*
        比较 '&', '|', '*' 运算的优先级
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<Node> * CreateNode(JString& reg, int index);
    
    JBinaryTree<Node> * CreateOperatorNode(char op, JStack<JBinaryTree<Node> *> & nodes);
    
    /*
        实现将正则表达式转为语法树
     */
    JBinaryTree<Node> * Reg2Syntax(JString& reg) {
        int i = 0;
        return Reg2Syntax(reg, i, '\0');
    }
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    friend class Syntax2NFA;
    class Syntax2NFA : public JBinaryTree<Node>::Interface {
    public:
        DFA *self;
        Syntax2NFA(DFA *self) : self(self) {}
        virtual void Visit(JBinaryTree<Node> *tree) {
            LOG_INFO(tree->Node());
            self->hello();
        }
    };
};

#endif /* DFA_hpp */
