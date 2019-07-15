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

class DFA {
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
        
        bool IsOperator() {
            return type == TYPE_OPER;
        }
        
        bool IsCharacter() {
            return type == TYPE_CHER;
        }
        
        char Value() {
            return val;
        }
        
        int RegIndex() {
            return regIndex;
        }
        
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
//    JGraph<char> dfa;
    
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
        
        Syntax2NFA syn2nfa(this);
        root.TraversePostorder(&syn2nfa);
        ObtainNFA(&syn2nfa);
        
        
        NFA2DFA(root.Tree(), &syn2nfa);
        
        
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
        DFA *self;
        // 1.followPos顶点中存放着reg字符位置，可映射到reg字符
        // 2.pos2ver由reg字符位置映射到followPos顶点
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        
        JGraph<char> NFA;
        
        Syntax2NFA(DFA *self) : self(self) {}
        
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
        LOG_INFO("NFA: ", syn2nfa->NFA);
    }
    
    
    void NFA2DFA(JBinaryTree<Node> *tree, Syntax2NFA* syn2nfa) {
        JNetwork<int, char> DFA;
        JSet<JSet<int>> Dstatus;
        JMap<int, int> stat2ver;
        
        JStack<int> Ustat(-1);
        JMap<char, JSet<int>> classify;
        
        
        // 获取初始NFA开始节点
        JSet<int> firstStatus;
        LOG_INFO("firstPos:", tree->Node().firstPos);
        for (JSet<int>::Iterator it = tree->Node().firstPos.ObtainIterator(); it.HasNext();) {
            firstStatus.Add(syn2nfa->pos2ver.Get(it.Next()));
        }
        LOG_INFO("firstStatus:", firstStatus);
        
        
        // 检测状态是否标记
        int ext = Dstatus.Add(firstStatus);
        if (ext != JSet<int>::FALG_EXIST) {
            Ustat.Push(ext);
            int val = DFA.AddVerter(ext);
            stat2ver.Add(ext, val);
        }
        
        
        while (Ustat.GetTop() != -1) {
            int statPos = Ustat.Pop();
            
            JSet<int>& stat = Dstatus.Get(statPos);
            LOG_INFO("transform status:", stat);
            for (JSet<int>::Iterator it1 = stat.ObtainIterator(); it1.HasNext();) {
                int verPos = it1.Next();
                JGraphVertex<char> ver = syn2nfa->NFA.Get(verPos);
                char key = ver.val;
                LOG_INFO("ver: ", verPos, ", key:", key);
                for (JSet<int>::Iterator it2 = ver.arcs.ObtainIterator(); it2.HasNext();){
                    classify.Pray(key).Add(it2.Next());
                }
            }
            LOG_INFO("classify: ", classify);
            
            // 检测状态是否标记
            for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
                int ext = Dstatus.Add(it.Next().value);
                if (ext != JSet<int>::FALG_EXIST) {
                    Ustat.Push(ext);
                    int val = DFA.AddVerter(ext);
                    stat2ver.Add(ext, val);
                }
            }
            
            classify.Clean();
            LOG_INFO("classify: ", classify);
            
        }
        LOG_INFO("DFA: ", DFA);
        LOG_INFO("Dstatus: ", Dstatus);
    
        // 转换
        
    }
    
};

#endif /* DFA_hpp */
