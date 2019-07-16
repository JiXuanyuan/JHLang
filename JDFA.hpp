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

class JDFA {
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
        
        bool IsOperator() { return type == TYPE_OPER; }
        
        bool IsCharacter() { return type == TYPE_CHER; }
        
        char Value() { return val; }
        
        int RegIndex() { return regIndex; }
        
        friend std::ostream& operator << (std::ostream& os, const Node& n) {
            if (n.type == 1) {
                os << "{ op = " << n.val;
            } else if (n.type == 2) {
                os << "{ ch = " << n.val << ", pos = " << n.regIndex;
            }
            os << ", nullable = " << n.nullable;
            os << ", firstPos = " << n.firstPos;
            os << ", lastPos = " << n.lastPos << " }";
            return os;
        }
    };
    
    JString reg;
    JNetwork<int, char> dfa;
    
public:
    
    JNetwork<int, char>& DeterministicFiniteAutomata() {
        return dfa;
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
        
        JBinaryTree<Node>::Root root;
        root = Reg2Syntax(reg);
        
        Syntax2NFA syn2nfa(this);
        root.TraversePostorder(&syn2nfa);
        ObtainNFA(&syn2nfa);
        
        
        NFA2DFA(dfa, root.Tree(), &syn2nfa);
        
        
        return true;
    }
    
    
    /*
        将正则表达式转为语法树
     */
    bool OperatorPrecede(char op1, char op2);
    
    JBinaryTree<Node> * CreateNodeCharacter(JString& reg, int index);
    
    JBinaryTree<Node> * CreateNodeOperator(char op, JStack<JBinaryTree<Node> *> & nodes);
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg);
    
    JBinaryTree<Node> * Reg2Syntax(JString& reg, int& i, char endChar);
    
    /*
        从语法树计算nullable、firstPos、lastPos
     */
    bool NodeNullable(JBinaryTree<Node> *tree);
    
    void ObtainNodeFirstPosition(JBinaryTree<Node> *tree, bool left, bool right);
    
    void ObtainNodeLastPosition(JBinaryTree<Node> *tree, bool left, bool right);
    
    void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<Node> *tree);
    
    /*
        从语法树计算followPos
     */
    void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    void ObtainNodeFollowPosition(JBinaryTree<Node> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        从语法树计算nullable、firstPos、lastPos、followPos，生成NFA
     */
    friend class Syntax2NFA;
    class Syntax2NFA : public JBinaryTree<Node>::Interface {
    public:
        JDFA *self;
        // 1.followPos顶点中存放着reg字符位置，可映射到reg字符
        // 2.pos2ver由reg字符位置映射到followPos顶点
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        
        JGraph<char> NFA;
        
        Syntax2NFA(JDFA *self) : self(self) {}
        
        virtual void Visit(JBinaryTree<Node> *tree) {
            self->ObtainNodeNullableAndFirstLastPosition(tree);
            self->ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
    };
    
    void ObtainNFA(Syntax2NFA* syn2nfa) {
        for (JGraph<int>::Iterator it = syn2nfa->followPos.ObtainIterator(); it.HasNext();) {
            JGraphVertex<int>& ver = it.Next();
            syn2nfa->NFA.AddVerter(reg.Get(ver.val));
            syn2nfa->NFA.GetTail().arcs.Add(ver.arcs);
        }
        
        // 在NFA尾部节点加入一个空节点和指向空节点的弧
        int v = syn2nfa->NFA.AddVerter('\0');
        syn2nfa->NFA.Get(v - 1).arcs.Add(v);
        
        LOG_INFO("NFA: ", syn2nfa->NFA);
    }
    
    
    void NFA2DFA(JNetwork<int, char>& DFA, JBinaryTree<Node> *tree, Syntax2NFA* syn2nfa) {
        static const int FLAG_STACK_EMPTY = -1;
        
        JSet<JSet<int>> Dstatus;
        JMap<int, int> stat2ver;
        
        JStack<int> Ustat(FLAG_STACK_EMPTY);
        JMap<char, JSet<int>> classify;
        
        
        // 获取初始NFA开始节点
        JSet<int> firstStatus;
        LOG_INFO("firstPos:", tree->Node().firstPos);
        for (JSet<int>::Iterator it = tree->Node().firstPos.ObtainIterator(); it.HasNext();) {
            firstStatus.Add(syn2nfa->pos2ver.Get(it.Next()));
        }
        LOG_INFO("firstStatus:", firstStatus);
        
        
        // 加入初始状态
        int k = Dstatus.Add(firstStatus);
        int v = DFA.AddVerter(k);
        stat2ver.Add(k, v);
        Ustat.Push(k);
        
        
        // 处理未标记的状态
        while (Ustat.GetTop() != FLAG_STACK_EMPTY) {
            int statPos = Ustat.Pop();
            
            // 对未标记状态，以字符进行归类
            JSet<int>& s = Dstatus.Get(statPos);
            LOG_INFO("transform: ", s);
            for (JSet<int>::Iterator it1 = s.ObtainIterator(); it1.HasNext();) {
                int i = it1.Next();
                JGraphVertex<char> ver = syn2nfa->NFA.Get(i);
                LOG_INFO("i: ", i , ", ver: ", ver);
                
                // 先取出关键字，避免NFA无边的节点未加入分类器
                JSet<int>& cf = classify.Pray(ver.val);
                for (JSet<int>::Iterator it2 = ver.arcs.ObtainIterator(); it2.HasNext();){
                    cf.Add(it2.Next());
                }
            }
            LOG_INFO("classify: ", classify);
            
            // 检测状态是否标记
            for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
                JMapPair<char, JSet<int>>& mp = it.Next();
                LOG_INFO("mp: ", mp);

                if (mp.value.Empty()) {

                    int e = DFA.AddVerter(-1);
//                    stat2ver.Add(k, v);
                    
                    
                    int s = stat2ver.Get(statPos);
//                    int e = stat2ver.Get(v);
                    LOG_INFO("arc: ", s, ", ", e, ", ", '\0');
                    DFA.AddArc(s, e, '\0');
                    
                    mp.value.Clean();
                    continue;
                }
                
                
                int k = Dstatus.Exist(mp.value);
                LOG_INFO("k: ", k);
                if (k == JSet<int>::FALG_NOT_EXIST) {
                    k = Dstatus.Add(mp.value);
                    int v = DFA.AddVerter(k);
                    stat2ver.Add(k, v);
                    Ustat.Push(k);
                }
                
                int s = stat2ver.Get(statPos);
                int e = stat2ver.Get(k);
                LOG_INFO("arc: ", s, ", ", e, ", ", mp.key);
                DFA.AddArc(s, e, mp.key);
                
                
//                mp.value.Clean();
            }
            classify.Clean();
            LOG_INFO("classify: ", classify);
            LOG_INFO("Dstatus: ", Dstatus);
            LOG_INFO("Ustat: ", Ustat);
        }
        
        LOG_INFO("DFA: ", DFA);
        LOG_INFO("Dstatus: ", Dstatus);
    
        // 转换
        
    }
    
};

#endif /* DFA_hpp */
