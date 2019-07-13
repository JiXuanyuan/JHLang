//
//  DFA.cpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include "DFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"
#include "JBinaryTree.hpp"
#include "JStack.hpp"

bool DFA::OperatorPrecede(char op1, char op2) {
    // 比较 '&', '|', '*' 运算的优先级
    int pr1 = 0;
    int pr2 = 0;
    if (op1 == '*') {
        pr1 = 3;
    } else if (op1 == '|') {
        pr1 = 2;
    } else if (op1 == '&') {
        pr1 = 1;
    }
    
    if (op2 == '*') {
        pr2 = 3;
    } else if (op2 == '|') {
        pr2 = 2;
    } else if (op2 == '&') {
        pr2 = 1;
    }
    
    return pr1 >= pr2;
}
/*
    实现将正则表达式转为NFA，使用二叉树表示
 */
JBinaryTree<char> * DFA::Reg2Syntax(JString& reg, int offset, int* end) {
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
            chn = Reg2Syntax(reg, ++i, &i);
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
