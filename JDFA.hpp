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
#include "JNetwork.hpp"
#include "JRegNode.hpp"

class JDFA {
private:
    JString reg;
    JNetwork<int, char> dfa;
    
public:
    
    JDFA& Reg(const char *reg) {
        LOG_FUNCTION_ENTRY;
        if (!this->reg.Assign(reg)) {
            LOG_WARN("not Assign");
        }
        
        return *this;
    }
    
    JNetwork<int, char>& ObtainDFA() {
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
        JGraph<char>& nfa = tran.ObtainNFA();
        JSet<int>& first = tran.ObtainFirstStatus(synt.Tree());
        
        // 由NFA转换为DFA
        NFA2DFA(nfa, first, dfa);
        
        return dfa;
    }
    
    
    /*
        将正则表达式转为语法树
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<JRegNode> * CreateNodeCharacter(JString& reg, int index);
    
    JBinaryTree<JRegNode> * CreateNodeOperator(char op, JStack<JBinaryTree<JRegNode> *> & nodes);
    
    JBinaryTree<JRegNode> * Reg2Syntax(JString& reg);
    
    JBinaryTree<JRegNode> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    /*
        从语法树计算nullable、firstPos、lastPos
     */
    bool NodeNullable(JBinaryTree<JRegNode> *tree);
    
    void ObtainNodeFirstPosition(JBinaryTree<JRegNode> *tree, bool left, bool right);
    
    void ObtainNodeLastPosition(JBinaryTree<JRegNode> *tree, bool left, bool right);
    
    void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<JRegNode> *tree);
    
    /*
        从语法树计算followPos
     */
    void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    void ObtainNodeFollowPosition(JBinaryTree<JRegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        从语法树计算nullable、firstPos、lastPos、followPos，生成NFA
     */
//    friend class Translator;
    class Translator : public JBinaryTree<JRegNode>::Interface {
    private:
        JDFA *self;
        // 1.followPos顶点中存放着reg字符位置，可映射到reg字符
        // 2.pos2ver由reg字符位置映射到followPos顶点
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        JGraph<char> nfa;
        JSet<int> firstStat;
        
    public:
        
        Translator(JDFA *self) : self(self) {}
        
        virtual void Visit(JBinaryTree<JRegNode> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
            self->ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
        
        JGraph<char>& ObtainNFA() {
            if (!nfa.Empty()) {
                return nfa;
            }
            
            LOG_INFO("followPos: ", followPos);
            for (JGraph<int>::Iterator it = followPos.ObtainIterator(); it.HasNext();) {
                JGraphVertex<int>& ver = it.Next();
                nfa.AddVerter(self->reg.Get(ver.val));
                nfa.GetTail().arcs.Add(ver.arcs);
            }
            
            // 在NFA尾部节点加入一个空节点和指向空节点的弧
            int v = nfa.AddVerter('\0');
            nfa.Get(v - 1).arcs.Add(v);
            
            LOG_INFO("NFA: ", nfa);
            return nfa;
        }
        
        JSet<int>& ObtainFirstStatus(JBinaryTree<JRegNode> *tree) {
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
    };
    
    int CreateDFAVerter(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status) {
        int k = Dstatus.Add(status);
        int v = DFA.AddVerter(k);
        stat2ver.Add(k, v);
        return k;
    }
    
    void CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch) {
        int s = stat2ver.Get(start);
        int e = stat2ver.Get(end);
        LOG_INFO("arc: ", s, ", ", e, ", ", ch);
        DFA.AddArc(s, e, ch);
    }
    
    void TransformDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify) {
        LOG_INFO("transform: ", status);
        classify.Clean();
        
        for (JSet<int>::Iterator it1 = status.ObtainIterator(); it1.HasNext();) {
            int i = it1.Next();
            JGraphVertex<char> ver = NFA.Get(i);
            LOG_INFO("i: ", i , ", ver: ", ver);
            
            // 先取出关键字，避免NFA无边的节点未加入分类器
            JSet<int>& cf = classify.Pray(ver.val);
            for (JSet<int>::Iterator it2 = ver.arcs.ObtainIterator(); it2.HasNext();){
                cf.Add(it2.Next());
            }
        }
        LOG_INFO("classify: ", classify);
    }
    
    void NFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, JNetwork<int, char>& DFA) {
        JSet<JSet<int>> Dstatus;
        JMap<int, int> stat2ver;
        JStack<int> Ustat(-1);
        
        // 加入初始状态
        int k = CreateDFAVerter(DFA, Dstatus, stat2ver, firstStatus);
        Ustat.Push(k);
        
        // 处理未标记的状态
        JMap<char, JSet<int>> classify;
        while (Ustat.GetTop() != -1) {
            int statPos = Ustat.Pop();
            
            // 对未标记状态，以字符进行归类
            TransformDFAStatus(NFA, Dstatus.Get(statPos), classify);
            
            // 检测状态是否标记
            for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
                JMapPair<char, JSet<int>>& mp = it.Next();
                LOG_INFO("mp: ", mp);
                
                // 无转化量时，为终止状态
                if (mp.value.Empty()) {
                    int k = DFA.AddVerter(-1);
                    CreateDFAFollow(DFA, stat2ver, statPos, k, '\0');
                    continue;
                }

                // 一般情况，为基础状态，先判断状态是否已存在
                int k = Dstatus.Exist(mp.value);
                LOG_INFO("k: ", k);
                if (k == JSet<int>::FALG_NOT_EXIST) {
                    k = CreateDFAVerter(DFA, Dstatus, stat2ver, mp.value);
                    Ustat.Push(k);
                }
                CreateDFAFollow(DFA, stat2ver, statPos, k, mp.key);
            }
        }
        
        LOG_INFO("DFA: ", DFA);
        LOG_INFO("Dstatus: ", Dstatus);        
    }
    
};

#endif /* DFA_hpp */
