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
#include "JDFARegNode.hpp"

/*
    public方法
 */
JDFA& JDFA::Regulation(const char *reg) {
    LOG_FUNCTION_ENTRY;
    
    this->regulation.Assign(reg);
    
    return *this;
}

JNetwork<int, char>& JDFA::ObtainDFA() {
    LOG_FUNCTION_ENTRY;
    LOG_INFO("regulation = ", regulation);
    if (!mDFA.Empty()) {
        return mDFA;
    }
    
    JGraph<char> NFA;
    JSet<int> firstStatus;
    TransformRegulation2NFA(regulation, NFA, firstStatus);
//    // 获得语法树
//    JBinaryTree<JDFARegNode>::Root synt = HandleReg2Syntax(regulation);
//    // 由语法树遍历，获得NFA
//    Translator tran;
//    synt.TraversePostorder(&tran);
//    tran.ObtainNFAAndFirstStatus(synt.Tree(), regulation, NFA, firstStatus);
    
    JMap<int, int> empty2lable;
    empty2lable.Add(NFA.Length() - 1, -1);
    // 由NFA转换为DFA
    TransformNFA2DFA(NFA, firstStatus, empty2lable, mDFA);
    LOG_INFO("DFA: ", mDFA);
    
    return mDFA;
}


/*
    public方法，提供在外部创建DFA
 */
void JDFA::TransformRegulation2NFA(const JString& regulation, JGraph<char>& NFA, JSet<int>& firstStatus) {
    LOG_FUNCTION_ENTRY;
    LOG_INFO("regulation: ", regulation);
    
    // 获得语法树
    JBinaryTree<JDFARegNode>::Root synt = HandleReg2Syntax(regulation);
    
    // 由语法树遍历，获得NFA
    Translator tran;
    synt.TraversePostorder(&tran);
    tran.ObtainNFAAndFirstStatus(synt.Tree(), regulation, NFA, firstStatus);
}

void JDFA::TransformNFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, const JMap<int, int>& empty2lable, JNetwork<int, char>& DFA) {
    LOG_FUNCTION_ENTRY;
    HandleNFA2DFA(NFA, firstStatus, empty2lable, DFA);
}


/*
    1 将正则表达式转为语法树
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

inline JBinaryTree<JDFARegNode> * JDFA::CreateNodeCharacter(const JString& reg, int index) {
    JBinaryTree<JDFARegNode> * n = new JBinaryTree<JDFARegNode>;
    n->Node().Assign(reg.Get(index), index);
    return n;
}

inline JBinaryTree<JDFARegNode> * JDFA::CreateNodeOperator(char op, JStack<JBinaryTree<JDFARegNode> *>& nodes) {
    // 顶点、左节点、右节点
    JBinaryTree<JDFARegNode> *fn = NULL;
    JBinaryTree<JDFARegNode> *ln = NULL;
    JBinaryTree<JDFARegNode> *rn = NULL;
    
    fn = new JBinaryTree<JDFARegNode>;
    fn->Node().Assign(op);
    
    rn = nodes.Pop();
    ln = nodes.Pop();
    fn = fn->Merge(ln, rn);
    return fn;
}

JBinaryTree<JDFARegNode> * JDFA::HandleReg2Syntax(const JString& reg) {
    int i = 0;
    return Reg2Syntax(reg, i, '\0');
}

JBinaryTree<JDFARegNode> * JDFA::Reg2Syntax(const JString& reg, int& i, char endChar) {
    LOG_FUNCTION_ENTRY;
    LOG_INFO("start, reg = ", reg, ", i = ", i);
    JStack<char> ops('\0');  // 优先级比'&', '|', '*'低的符号
    JStack<JBinaryTree<JDFARegNode> *> nodes(NULL);
    char op;
    char ch;
    // 操作符节点、符号节点
    JBinaryTree<JDFARegNode> *opn = NULL;
    JBinaryTree<JDFARegNode> *chn = NULL;
    // 顶点
    JBinaryTree<JDFARegNode> *fn = NULL;
    
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
//    } while (ops.Empty());
    
    // 返回值
    LOG_INFO("end, i = ", i);
    opn = nodes.Pop();
    return opn;
}


/*
    2.1 从语法树计算nullable、firstPos、lastPos
 */
inline bool JDFA::NodeNullable(JBinaryTree<JDFARegNode> *tree) {
    if (tree == NULL) {
        return true;
    }
    return tree->Node().nullable;
}

inline void JDFA::ObtainNodeFirstPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right) {
    if (left && tree->LeftChild() != NULL) {
        tree->Node().firstPos.Add(tree->LeftChild()->Node().firstPos);
    }
    
    if (right && tree->RightChild() != NULL) {
        tree->Node().firstPos.Add(tree->RightChild()->Node().firstPos);
    }
}

inline void JDFA::ObtainNodeLastPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right) {
    if (left && tree->LeftChild() != NULL) {
        tree->Node().lastPos.Add(tree->LeftChild()->Node().lastPos);
    }
    
    if (right && tree->RightChild() != NULL) {
        tree->Node().lastPos.Add(tree->RightChild()->Node().lastPos);
    }
}

void JDFA::ObtainNodeNullableAndMergeFLPosition(JBinaryTree<JDFARegNode> *tree) {
    // 后序遍历处理，遍历过程tree不为NULL
    JDFARegNode& n = tree->Node();
    
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
    2.2 从语法树计算followPos
 */
inline void JDFA::ObtainNodeFollowPosition(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos) {
    int ls = startPos.Length();
    int le = endPos.Length();
    int s = 0;
    int e = 0;
    
    for (int i = 0; i < ls; i++) {
        s = pos2ver.GetByKey(startPos.Get(i));
        for (int j = 0; j < le; j++) {
            e = pos2ver.GetByKey(endPos.Get(j));
            followPos.AddArc(s, e);
            LOG_INFO("add arc: ", s, ", ", e);
        }
    }
}

void JDFA::MergeNodeFollowPosition(JBinaryTree<JDFARegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver) {
    JDFARegNode& n = tree->Node();
    
    if (n.IsCharacter()) {
        int k = n.RegIndex();
        int v = followPos.AddVerter(k);
        pos2ver.Add(k, v);
        return;
    }
    
    if (n.IsOperator() && n.Value() == '*') {
        ObtainNodeFollowPosition(followPos, pos2ver,
                                 tree->Node().lastPos,
                                 tree->Node().firstPos);
        return;
    }
    
    if (n.IsOperator() && n.Value() == '&') {
        if (tree->LeftChild() == NULL || tree->RightChild() == NULL) {
            return;
        }
        ObtainNodeFollowPosition(followPos, pos2ver,
                                 tree->LeftChild()->Node().lastPos,
                                 tree->RightChild()->Node().firstPos);
        return;
    }
    
    LOG_WARN("node, type = 0");
}

/*
    2.4 遍历后，计算NFA、firstStatus
 */
void JDFA::Translator::ObtainNFAAndFirstStatus(JBinaryTree<JDFARegNode> *tree, const JString& regulation, JGraph<char>& NFA, JSet<int>& firstStatus) {
    if (tree == NULL) {
        return;
    }
    
    // 1.输入的NFA可能已包含原NFA结构
    //  两个NFA图之间为|关系运算，followPos关系保持不变，firstStatus为两者的并集
    int offset = NFA.Length();
    LOG_INFO("offset: ", offset);
    
    // 2.由followPos构建NFA，并将位置量转换成对应字符
    LOG_INFO("followPos: ", followPos);
    for (JGraph<int>::Iterator it1 = followPos.ObtainIterator(); it1.HasNext();) {
        JGraphVertex<int>& ver = it1.Next();
        
        int i = NFA.AddVerter(regulation.Get(ver.value));
        JSet<int>& arcs = NFA.Get(i).arcs;
        for (JSet<int>::Iterator it2 = ver.arcs.ObtainIterator(); it2.HasNext();) {
            arcs.Add(it2.Next() + offset);
        }
        LOG_INFO("copy, i: ", i, ", arcs: ", arcs);
    }
    
    // 3.在NFA尾部节点加入一个空节点，和指向空节点的弧，
    int v = NFA.AddVerter('\0');
    // 特殊处理便于获取接受节点的位置，实际中接受节点不指向任何节点（2019/07/18）
    NFA.Get(v).arcs.Add(v);
    
    // 原followPos树与'\0'为&关系运算，左节点为followPos树、右节点为'\0'
    // 4.&运算，计算新结构的followPos，左节点lastPos中的每个i，都有followPos(i)为右节点firstPos集合
    LOG_INFO("lastPos:", tree->Node().lastPos);
    for (JSet<int>::Iterator it = tree->Node().lastPos.ObtainIterator(); it.HasNext();) {
        int l = pos2ver.GetByKey(it.Next()) + offset;
        
        LOG_INFO("end, arc: ", l, ", ", v);
        NFA.Get(l).arcs.Add(v);
    }
    
    // 计算firstStatus，为两个DFA的firstPos并集
    // 5.&运算，计算新结构的firstPos，等于左节点firstPos、与右节点nullable时firstPos的并集
    LOG_INFO("firstPos:", tree->Node().firstPos);
    for (JSet<int>::Iterator it = tree->Node().firstPos.ObtainIterator(); it.HasNext();) {
        int l = pos2ver.GetByKey(it.Next()) + offset;
        
        LOG_INFO("start, i: ", l);
        firstStatus.Add(l);
    }
    LOG_INFO("nullable:", tree->Node().nullable);
    if (tree->Node().nullable) {
        firstStatus.Add(v);
    }
    
    LOG_INFO("NFA: ", NFA);
    LOG_INFO("firstStat:", firstStatus);
}


/*
    3 由NFA，从firstStatus开始，转化成DFA
 */
inline int JDFA::CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status) {
    int k = Dstatus.Add(status);
    int v = DFA.AddVertex(0);
    stat2ver.Add(k, v);
    return k;
}

inline void JDFA::CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch) {
    int s = stat2ver.GetByKey(start);
    int e = stat2ver.GetByKey(end);
    LOG_INFO("arc: ", s, ", ", e, ", ", ch);
    DFA.AddArc(s, e, ch);
}

inline void JDFA::CreateDFAFollowAccept(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int flag) {
    int s = stat2ver.GetByKey(start);
    int e = DFA.AddVertex(flag);
    LOG_INFO("arc: ", s, ", ", e);
    DFA.AddArc(s, e, '\0');
}

inline void JDFA::ClassifyDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify) {
    LOG_INFO("transform: ", status);
    
    // 清空分类器
    for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
        it.Next().value.Clean();
    }
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

inline void JDFA::HandleNFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, const JMap<int, int>& empty2lable, JNetwork<int, char>& DFA) {
    LOG_FUNCTION_ENTRY;
    JSet<JSet<int>> Dstatus;
    JMap<int, int> stat2ver;
    JStack<int> Ustat(-1);
    
    // 加入初始状态
    int k = CreateDFAVertex(DFA, Dstatus, stat2ver, firstStatus);
    Ustat.Push(k);
    
    // 处理未标记的状态
    JMap<char, JSet<int>> classify;
    //        while (!Ustat.Empty()) {
    while (Ustat.GetTop() != -1) {
        
        int statPos = Ustat.Pop();
        LOG_INFO("statPos: ", statPos);
        // 对未标记状态，以字符进行归类
        ClassifyDFAStatus(NFA, Dstatus.Get(statPos), classify);
        
        // 检测状态是否标记
        for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
            JMapPair<char, JSet<int>>& map = it.Next();
            LOG_INFO("map: ", map);
            
//                // 无转化量时，为终止状态
//                if (map.value.Empty()) {
//                    CreateDFAFollowAccept(DFA, stat2ver, statPos);
//                    continue;
//                }
            
            // 无转化量时，为终止状态
            if (map.key == '\0') {
                int fl = empty2lable.GetByKey(map.value.Get(0));
                LOG_INFO("flag: ", fl);
                CreateDFAFollowAccept(DFA, stat2ver, statPos, fl);
                continue;
            }
            
            // 一般情况，为基础状态，先判断状态是否已存在
            int k = Dstatus.ExistPosition(map.value);
            LOG_INFO("k: ", k);
            if (k == JLIST_FALG_NOT_EXIST) {
                k = CreateDFAVertex(DFA, Dstatus, stat2ver, map.value);
                Ustat.Push(k);
            }
            CreateDFAFollow(DFA, stat2ver, statPos, k, map.key);
            LOG_INFO("DFA: ", DFA);
            LOG_INFO("Dstatus: ", Dstatus);
        }
    }
    
    LOG_INFO("DFA: ", DFA);
}
