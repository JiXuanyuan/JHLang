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

class DFA : public JBinaryTree<char>::Interface {
private:
    JString reg;
    JBinaryTree<char> nfa;
    
public:
    
    virtual void func(JBinaryTree<char> *tree) {
        LOG_INFO(tree->Node());
    }
    
    DFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    ~DFA() {
        LOG_FUNCTION_ENTRY;
//        JBinaryTree<char>::Destory(nfa);
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
        JBinaryTree<char> *opn = NULL;
        opn = Reg2Syntax(reg, 0, NULL);
        JBinaryTree<char>::Traverse(opn, this);
        JBinaryTree<char>::Destory(opn);
        return true;
    }
    
    bool OperatorPrecede(char op1, char op2);
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    JBinaryTree<char> * Reg2Syntax(JString& reg, int offset, int* end);
};


#endif /* DFA_hpp */
