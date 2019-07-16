//
//  DFA.cpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include "JDFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"
#include "JBinaryTree.hpp"
#include "JStack.hpp"
#include "JSet.hpp"
#include "JGraph.hpp"
#include "JMap.hpp"
#include "JNetwork.hpp"

/*
    实现将正则表达式转为语法树，旧函数
 */
/*
    实现将正则表达式转为语法树，旧函数
 */
inline bool JDFA::OperatorPrecede(char op1, char op2) {
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

inline JBinaryTree<JDFA::JRegNode> * JDFA::CreateNodeCharacter(JString& reg, int index) {
    JBinaryTree<JRegNode> * n = new JBinaryTree<JRegNode>;
    n->Node().Assign(reg.Get(index), index);
    return n;
}

inline JBinaryTree<JDFA::JRegNode> * JDFA::CreateNodeOperator(char op, JStack<JBinaryTree<JRegNode> *>& nodes) {
    // 顶点、左节点、右节点
    JBinaryTree<JRegNode> *fn = NULL;
    JBinaryTree<JRegNode> *ln = NULL;
    JBinaryTree<JRegNode> *rn = NULL;
    
    fn = new JBinaryTree<JRegNode>;
    fn->Node().Assign(op);
    
    rn = nodes.Pop();
    ln = nodes.Pop();
    fn = fn->Merge(ln, rn);
    return fn;
}

/*
    实现将正则表达式转为语法树
 */
JBinaryTree<JDFA::JRegNode> * JDFA::Reg2Syntax(JString& reg) {
    int i = 0;
    return Reg2Syntax(reg, i, '\0');
}

JBinaryTree<JDFA::JRegNode> * JDFA::Reg2Syntax(JString& reg, int& i, char endChar) {
    LOG_FUNCTION_ENTRY;
    LOG_INFO("start, reg = ", reg, ", i = ", i);
    JStack<char> ops('\0');  // 优先级比'&', '|', '*'低的符号
    JStack<JBinaryTree<JRegNode> *> nodes(NULL);
    char op;
    char ch;
    // 操作符节点、符号节点
    JBinaryTree<JRegNode> *opn = NULL;
    JBinaryTree<JRegNode> *chn = NULL;
    // 顶点
    JBinaryTree<JRegNode> *fn = NULL;
    
    int l = reg.Length();
    for (; i < l; i++) {
        ch = reg.Get(i);
        
        if (ch == '\\') {   // 转义符
            op = '&';
            chn = CreateNodeCharacter(reg, ++i);
        } else if (ch == '(') {
            op = '&';
            chn = Reg2Syntax(reg, ++i, ')');
        } else if (ch == endChar) {
            LOG_INFO("ch = ", ch, ", break");
            break;
        } else if (ch == '*') {
            op = '*';
            chn = NULL;
        } else if (ch == '|') {
            op = '|';
            chn = CreateNodeCharacter(reg, ++i);
        } else {
            op = '&';
            chn = CreateNodeCharacter(reg, i);
        }
        LOG_INFO("ch = ", ch, ", op = ", op);
        
        while (OperatorPrecede(ops.GetTop(), op)) {
            fn = CreateNodeOperator(ops.Pop(), nodes);
            nodes.Push(fn);
        }
        
        ops.Push(op);
        nodes.Push(chn);
    }
    
    do {
        fn = CreateNodeOperator(ops.Pop(), nodes);
        nodes.Push(fn);
    } while (ops.GetTop() != '\0');
    
    // 返回值
    LOG_INFO("end, i = ", i);
    opn = nodes.Pop();
    return opn;
}

/*
    实现将正则表达式转为语法树，旧函数
 */
inline bool JDFA::NodeNullable(JBinaryTree<JRegNode> *tree) {
    if (tree == NULL) {
        return true;
    }
    return tree->Node().nullable;
}

inline void JDFA::ObtainNodeFirstPosition(JBinaryTree<JRegNode> *tree, bool left, bool right) {
    if (left && tree->LeftChild() != NULL) {
        tree->Node().firstPos.Add(tree->LeftChild()->Node().firstPos);
    }
    
    if (right && tree->RightChild() != NULL) {
        tree->Node().firstPos.Add(tree->RightChild()->Node().firstPos);
    }
}

inline void JDFA::ObtainNodeLastPosition(JBinaryTree<JRegNode> *tree, bool left, bool right) {
    if (left && tree->LeftChild() != NULL) {
        tree->Node().lastPos.Add(tree->LeftChild()->Node().lastPos);
    }
    
    if (right && tree->RightChild() != NULL) {
        tree->Node().lastPos.Add(tree->RightChild()->Node().lastPos);
    }
}

/*
 实现将正则表达式转为语法树，旧函数
 */
void JDFA::ObtainNodeNullableAndFirstLastPosition(JBinaryTree<JRegNode> *tree) {
    // 后序遍历处理，遍历过程tree不为NULL
    JRegNode& n = tree->Node();
    
    if (n.IsCharacter()) {
        n.nullable = false;
        n.firstPos.Add(n.RegIndex());
        n.lastPos.Add(n.RegIndex());
        LOG_INFO("node: ", tree->Node());
        return;
    }
    
    if (n.IsOperator()) {
        char ch = n.Value();
        if (ch == '*') {
            n.nullable = true;
            // *操作，右节点为NULL，(true, false)同(true, true)
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
        LOG_INFO("node: ", tree->Node());
        return;
    }
    
    LOG_WARN("node, type = 0");
}

/*
 实现将正则表达式转为语法树，旧函数
 */
inline void JDFA::ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos) {
    int ls = startPos.Length();
    int le = endPos.Length();
    int s = 0;
    int e = 0;
    
    for (int i = 0; i < ls; i++) {
        s = pos2ver.Get(startPos.Get(i));
        for (int j = 0; j < le; j++) {
            e = pos2ver.Get(endPos.Get(j));
            followPos.AddArc(s, e);
            LOG_INFO("add arc: ", s, ", ", e);
        }
    }
}

/*
 实现将正则表达式转为语法树，旧函数
 */
void JDFA::ObtainNodeFollowPosition(JBinaryTree<JRegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver) {
    JRegNode& n = tree->Node();
    
    if (n.IsCharacter()) {
        int k = n.RegIndex();
        int v = followPos.AddVerter(k);
        pos2ver.Add(k, v);
        return;
    }
    
    if (n.IsOperator() && n.Value() == '*') {
        ObtainNodeFollowGraphArc(followPos, pos2ver,
                                 tree->Node().lastPos,
                                 tree->Node().firstPos);
        return;
    }
    
    if (n.IsOperator() && n.Value() == '&') {
        if (tree->LeftChild() == NULL || tree->RightChild() == NULL) {
            return;
        }
        ObtainNodeFollowGraphArc(followPos, pos2ver,
                                 tree->LeftChild()->Node().lastPos,
                                 tree->RightChild()->Node().firstPos);
        return;
    }
    
    LOG_WARN("node, type = 0");
}
