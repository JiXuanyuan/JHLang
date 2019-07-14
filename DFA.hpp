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
        int regIndex;
        char var;
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
    
public:
    class Pchar : public JBinaryTree<char>::Interface {
        virtual void func(JBinaryTree<char> *tree) {
            LOG_INFO(tree->Node());
        }
    };
    
    class PNode : public JBinaryTree<Node>::Interface {
        virtual void func(JBinaryTree<Node> *tree) {
            LOG_INFO(tree->Node());
        }
    };
    
    DFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    ~DFA() {
        LOG_FUNCTION_ENTRY;
    }
    
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
//        JBinaryTree<char> *opn = NULL;
//        opn = Reg2Syntax(reg, 0, NULL);
//        Pchar pc;
//        JBinaryTree<char>::Traverse(opn, &pc);
//        JBinaryTree<char>::Destory(opn);
        
        JBinaryTree<Node> *nodes = NULL;
        nodes = Reg2NFA(reg);
        PNode pn;
        JBinaryTree<Node>::Traverse(nodes, &pn);
        JBinaryTree<Node>::Destory(nodes);
        
        return true;
    }
    
    JBinaryTree<char> * Reg2Syntax(JString& reg, int offset, int* end);
    
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<Node> * CreateNode(JString& reg, int index);
    
    JBinaryTree<Node> * CreateOperatorNode(char op, JStack<JBinaryTree<Node> *> & nodes);
    
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    JBinaryTree<Node> * Reg2NFA(JString& reg) {
        int i = 0;
        return Reg2NFA(reg, i, '\0');
    }
    
    JBinaryTree<Node> * Reg2NFA(JString& reg, int& i, char endChar);
};


#endif /* DFA_hpp */
