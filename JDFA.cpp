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
#include "JRegNode.hpp"

/*
 
 */
JDFA& JDFA::Reg(const char *reg) {
    LOG_FUNCTION_ENTRY;
    if (!this->reg.Assign(reg)) {
        LOG_WARN("not Assign");
    }
    
    return *this;
}

JNetwork<int, char>& JDFA::ObtainDFA() {
    LOG_FUNCTION_ENTRY;
    LOG_INFO("reg = ", reg);
    
    if (!dfa.Empty()) {
        return dfa;
    }
    
    // 获得语法树
    JBinaryTree<JRegNode>::Root synt = Reg2Syntax(reg);
    
    // 由语法树遍历，获得NFA
    Translator tran(this);
    synt.TraversePostorder(&tran);
    JGraph<char>& nfa = tran.ObtainNFA(synt.Tree());
    JSet<int>& first = tran.ObtainFirstStatus(synt.Tree());
    
    // 由NFA转换为DFA
    NFA2DFA(nfa, first, dfa);
    
    return dfa;
}

/*
 
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

inline JBinaryTree<JRegNode> * JDFA::CreateNodeCharacter(const JString& reg, int index) {
    JBinaryTree<JRegNode> * n = new JBinaryTree<JRegNode>;
    n->Node().Assign(reg.Get(index), index);
    return n;
}

inline JBinaryTree<JRegNode> * JDFA::CreateNodeOperator(char op, JStack<JBinaryTree<JRegNode> *>& nodes) {
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
JBinaryTree<JRegNode> * JDFA::Reg2Syntax(const JString& reg) {
    int i = 0;
    return Reg2Syntax(reg, i, '\0');
}

JBinaryTree<JRegNode> * JDFA::Reg2Syntax(const JString& reg, int& i, char endChar) {
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
//    } while (ops.GetTop() != '\0');
    } while (ops.Empty());
    
    // 返回值
    LOG_INFO("end, i = ", i);
    opn = nodes.Pop();
    return opn;
}

/*
 
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


/*
 
 */
JGraph<char>& JDFA::Translator::ObtainNFA(JBinaryTree<JRegNode> *tree) {
    if (!nfa.Empty() || tree == NULL) {
        return nfa;
    }
    
    LOG_INFO("followPos: ", followPos);
    for (JGraph<int>::Iterator it = followPos.ObtainIterator(); it.HasNext();) {
        JGraphVertex<int>& ver = it.Next();
        nfa.AddVerter(self->reg.Get(ver.value));
        nfa.GetTail().arcs.Add(ver.arcs);
    }
    
    // 在NFA尾部节点加入一个空节点和指向空节点的弧
    int v = nfa.AddVerter('\0');
    LOG_INFO("lastPos:", tree->Node().lastPos);
    for (JSet<int>::Iterator it = tree->Node().lastPos.ObtainIterator(); it.HasNext();) {
        int l = pos2ver.Get(it.Next());
        LOG_INFO("arc: ", l, ", ", v);
        nfa.Get(l).arcs.Add(v);
    }
    
    LOG_INFO("NFA: ", nfa);
    return nfa;
}

/*
 
 */
JSet<int>& JDFA::Translator::ObtainFirstStatus(JBinaryTree<JRegNode> *tree) {
    if (!firstStat.Empty() || tree == NULL) {
        return firstStat;
    }
    
    LOG_INFO("firstPos:", tree->Node().firstPos);
    for (JSet<int>::Iterator it = tree->Node().firstPos.ObtainIterator(); it.HasNext();) {
        firstStat.Add(pos2ver.Get(it.Next()));
    }
    
    LOG_INFO("firstStat:", firstStat);
    return firstStat;
}

inline int JDFA::CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status) {
    int k = Dstatus.Add(status);
    int v = DFA.AddVertex(k);
    stat2ver.Add(k, v);
    return k;
}

inline void JDFA::CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch) {
    int s = stat2ver.Get(start);
    int e = stat2ver.Get(end);
    LOG_INFO("arc: ", s, ", ", e, ", ", ch);
    DFA.AddArc(s, e, ch);
}

inline void JDFA::CreateDFAFollowAccept(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start) {
    int s = stat2ver.Get(start);
    int e = DFA.AddVertex(-1);
    LOG_INFO("arc: ", s, ", ", e);
    DFA.AddArc(s, e, '\0');
}

/*
 
 */
inline void JDFA::TransformDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify) {
    LOG_INFO("transform: ", status);
    classify.Clean();
    
    for (JSet<int>::Iterator it1 = status.ObtainIterator(); it1.HasNext();) {
        int i = it1.Next();
        JGraphVertex<char> ver = NFA.Get(i);
        LOG_INFO("i: ", i , ", ver: ", ver);
        
        // 先取出关键字，避免NFA无边的节点未加入分类器
        JSet<int>& cf = classify.Pray(ver.value);
        for (JSet<int>::Iterator it2 = ver.arcs.ObtainIterator(); it2.HasNext();){
            cf.Add(it2.Next());
        }
    }
    LOG_INFO("classify: ", classify);
}


void JDFA::NFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, JNetwork<int, char>& DFA) {
    JSet<JSet<int>> Dstatus;
    JMap<int, int> stat2ver;
    JStack<int> Ustat(-1);
    
    // 加入初始状态
    int k = CreateDFAVertex(DFA, Dstatus, stat2ver, firstStatus);
    Ustat.Push(k);
    
    // 处理未标记的状态
    JMap<char, JSet<int>> classify;
    while (!Ustat.Empty()) {
//    while (Ustat.GetTop() != -1) {
        int statPos = Ustat.Pop();
        
        // 对未标记状态，以字符进行归类
        TransformDFAStatus(NFA, Dstatus.Get(statPos), classify);
        
        // 检测状态是否标记
        for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
            JMapPair<char, JSet<int>>& map = it.Next();
            LOG_INFO("map: ", map);
            
            // 无转化量时，为终止状态
            if (map.value.Empty()) {
                CreateDFAFollowAccept(DFA, stat2ver, statPos);
                continue;
            }
            
            // 一般情况，为基础状态，先判断状态是否已存在
            int k = Dstatus.ExistPosition(map.value);
            LOG_INFO("k: ", k);
            if (k == JSet<JSet<int>>::FALG_NOT_EXIST) {
                k = CreateDFAVertex(DFA, Dstatus, stat2ver, map.value);
                Ustat.Push(k);
            }
            CreateDFAFollow(DFA, stat2ver, statPos, k, map.key);
            LOG_INFO("DFA: ", DFA);
            LOG_INFO("Dstatus: ", Dstatus);
        }
    }
    
    LOG_INFO("DFA: ", DFA);
    LOG_INFO("Dstatus: ", Dstatus);
}


