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
    
//    bool OperatorPrecede(char op1, char op2) {
//        // 比较 '&', '|', '*' 运算的优先级
//        if (op1 == '*') {
//            return true;
//        } else if (op2 == '*') {
//            return false;
//        } else if (op1 == '&' || op1 == '|' ) {
//            return true;
//        }
//        return false;
//    }
//
//    bool Reg2NFA() {
//        LOG_FUNCTION_ENTRY;
//        LOG_INFO("reg = ", reg);
//        JStack<char> ops('\0');  // 优先级比'&', '|', '*'低的符号
//        JStack<JBinaryTree<char> *> nodes(NULL);
//        char op;
//        char ch;
//        // 顶点、左节点、右节点
//        JBinaryTree<char> *fn = NULL;
//        JBinaryTree<char> *ln = NULL;
//        JBinaryTree<char> *rn = NULL;
//        // 操作符节点、符号节点
//        JBinaryTree<char> *opn = NULL;
//        JBinaryTree<char> *chn = NULL;
//        int l = reg.Length();
//        for (int i = 0; i < l; i++) {
//            ch = reg.Get(i);
//            if (ch == '\\') {   // 转义符
//                op = '&';
//                chn = new JBinaryTree<char>(reg.Get(++i));
//            } else if (ch == '*') {
//                op = '*';
//                chn = NULL;
//            } else if (ch == '|') {
//                op = '|';
//                chn = new JBinaryTree<char>(reg.Get(++i));
//            }  else {
//                op = '&';
//                chn = new JBinaryTree<char>(ch);
//            }
//            LOG_INFO("ch = ", ch, ", op = ", op);
//
//            while (OperatorPrecede(ops.GetTop(), op)) {
//                fn = new JBinaryTree<char>(ops.Pop());
//                rn = nodes.Pop();
//                ln = nodes.Pop();
//                fn = fn->Merge(ln, rn);
//                nodes.Push(fn);
//            }
//
//            ops.Push(op);
//            nodes.Push(chn);
//        }
//
//        do {
//            op = ops.Pop();
//            fn = new JBinaryTree<char>(op);
//            rn = nodes.Pop();
//            ln = nodes.Pop();
//            fn = fn->Merge(ln, rn);
//            nodes.Push(fn);
//        } while (OperatorPrecede(ops.GetTop(), op));
//        opn = nodes.Pop();
//
//        JBinaryTree<char>::Traverse(opn, this);
//        JBinaryTree<char>::Destory(opn);
//
//        return true;
//    }
    
    bool Reg2NFA() {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("reg = ", reg);
        JBinaryTree<char> *opn = NULL;
        opn = Reg2NFA(reg, 0, NULL);
        JBinaryTree<char>::Traverse(opn, this);
        JBinaryTree<char>::Destory(opn);
        return true;
    }
    
    bool OperatorPrecede(char op1, char op2) {
        // 比较 '&', '|', '*' 运算的优先级
        if (op1 == '*') {
            return true;
        } else if (op2 == '*') {
            return false;
        } else if (op1 == '&' || op1 == '|' ) {
            return true;
        }
        return false;
    }
    /*
        实现将正则表达式转为NFA，使用二叉树表示
     */
    JBinaryTree<char> * Reg2NFA(JString& reg, int offset, int* end) {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("reg = ", reg, ", offset = ", offset);
        JStack<char> ops('\0');  // 优先级比'&', '|', '*'低的符号
        JStack<JBinaryTree<char> *> nodes(NULL);
        char op;
        char ch;
        // 操作符节点、符号节点
        JBinaryTree<char> *opn = NULL;
        JBinaryTree<char> *chn = NULL;
        // 顶点、左节点、右节点
        JBinaryTree<char> *fn = NULL;
        JBinaryTree<char> *ln = NULL;
        JBinaryTree<char> *rn = NULL;
        //
        int l = reg.Length();
        int i = offset;
        for (; i < l; i++) {
            ch = reg.Get(i);
            if (ch == '\\') {   // 转义符
                op = '&';
                chn = new JBinaryTree<char>(reg.Get(++i));
            } else if (ch == '(') {
                op = '&';
                chn = Reg2NFA(reg, ++i, &i);
            } else if (ch == ')') {
                LOG_INFO("ch = ", ch, ", break");
                break;
            } else if (ch == '*') {
                op = '*';
                chn = NULL;
            } else if (ch == '|') {
                op = '|';
                chn = new JBinaryTree<char>(reg.Get(++i));
            } else {
                op = '&';
                chn = new JBinaryTree<char>(ch);
            }
            LOG_INFO("ch = ", ch, ", op = ", op);
            
            while (OperatorPrecede(ops.GetTop(), op)) {
                fn = new JBinaryTree<char>(ops.Pop());
                rn = nodes.Pop();
                ln = nodes.Pop();
                fn = fn->Merge(ln, rn);
                nodes.Push(fn);
            }
            
            ops.Push(op);
            nodes.Push(chn);
        }
        
        do {
            op = ops.Pop();
            fn = new JBinaryTree<char>(op);
            rn = nodes.Pop();
            ln = nodes.Pop();
            fn = fn->Merge(ln, rn);
            nodes.Push(fn);
        } while (ops.GetTop() != '\0');
        
        // 返回值
        opn = nodes.Pop();
        if (end != NULL) {
            *end = i;
            LOG_INFO("end = ", *end);
        }
        return opn;
    }
};


#endif /* DFA_hpp */
