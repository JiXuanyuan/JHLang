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
#include "JDFARegNode.hpp"

class JDFA {
public:
    
    JDFA() {
        LOG_FUNCTION_ENTRY;
    }
    
    JDFA(const char *reg) {
        LOG_FUNCTION_ENTRY;
        this->regulation.Assign(reg);
    }
    
    JDFA& Regulation(const char *reg);
    
//    JDFA& Separator(const char *sep) {
//        LOG_FUNCTION_ENTRY;
////        if (!this->separator.Assign(sep)) {
////            LOG_WARN("not Assign");
////        }
//        return *this;
//    }
    
    JNetwork<int, char>& ObtainDFA();
    
    static void ObtainNFA(JGraph<char>& NFA, JSet<int>& firstStatus, JString& regulation) {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("regulation: ", regulation);
        
        // 获得语法树
        JBinaryTree<JDFARegNode>::Root synt = Reg2Syntax(regulation);
        
        // 由语法树遍历，获得NFA
        Translator tran;
        synt.TraversePostorder(&tran);
        tran.ObtainNFAAndFirstStatus(synt.Tree(), regulation, NFA, firstStatus);
    }
    
    static void TransformNFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, const JMap<int, int>& empty2lable, JNetwork<int, char>& DFA) {
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
            TransformDFAStatus(NFA, Dstatus.Get(statPos), classify);
            
            // 检测状态是否标记
            for (JMap<char, JSet<int>>::Iterator it = classify.ObtainIterator(); it.HasNext();) {
                JMapPair<char, JSet<int>>& map = it.Next();
                LOG_INFO("map: ", map);
                
                //            // 无转化量时，为终止状态
                //            if (map.value.Empty()) {
                //                CreateDFAFollowAccept(DFA, stat2ver, statPos);
                //                continue;
                //            }
                
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
        
        DFA.Echo();
        LOG_INFO("DFA: ", DFA);
        
//        LOG_INFO("Dstatus: ", Dstatus);
    }
    
    
private:
    
    JString regulation;
//    JString separator;
    JNetwork<int, char> dfa;
    
    /*
        将正则表达式转为语法树
     */
    static bool OperatorPrecede(char op1, char op2);
    
    static JBinaryTree<JDFARegNode> * CreateNodeCharacter(const JString& reg, int index);
    
    static JBinaryTree<JDFARegNode> * CreateNodeOperator(char op, JStack<JBinaryTree<JDFARegNode> *> & nodes);
    
    static JBinaryTree<JDFARegNode> * Reg2Syntax(const JString& reg);
    
    static JBinaryTree<JDFARegNode> * Reg2Syntax(const JString& reg, int& i, char endChar);
    
    /*
        从语法树计算nullable、firstPos、lastPos
     */
    static bool NodeNullable(JBinaryTree<JDFARegNode> *tree);
    
    static void ObtainNodeFirstPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeLastPosition(JBinaryTree<JDFARegNode> *tree, bool left, bool right);
    
    static void ObtainNodeNullableAndFirstLastPosition(JBinaryTree<JDFARegNode> *tree);
    
    /*
        从语法树计算followPos
     */
    static void ObtainNodeFollowGraphArc(JGraph<int>& followPos, JMap<int, int>& pos2ver, JSet<int>& startPos, JSet<int>& endPos);
    
    static void ObtainNodeFollowPosition(JBinaryTree<JDFARegNode> *tree, JGraph<int>& followPos, JMap<int, int>& pos2ver);
    
    /*
        从语法树计算nullable、firstPos、lastPos、followPos，生成NFA
     */
//    friend class Translator;
    class Translator : public JBinaryTree<JDFARegNode>::Interface {
    private:
        
//        JDFA *self;
        // 1.followPos顶点中存放着reg字符位置，可映射到reg字符
        // 2.pos2ver由reg字符位置映射到followPos顶点
        JGraph<int> followPos;
        JMap<int, int> pos2ver;
        JGraph<char> nfa;
        JSet<int> firstStat;
        
    public:
        
//        Translator(JDFA *self) : self(self) {}
        
        virtual void Visit(JBinaryTree<JDFARegNode> *tree) {
            JDFA::ObtainNodeNullableAndFirstLastPosition(tree);
            JDFA::ObtainNodeFollowPosition(tree, followPos, pos2ver);
            LOG_INFO("pos2ver: ", pos2ver);
            LOG_INFO("followPos: ", followPos);
        }
        
        JGraph<char>& ObtainNFA(JBinaryTree<JDFARegNode> *tree, const JString& regulation);
        
        JSet<int>& ObtainFirstStatus(JBinaryTree<JDFARegNode> *tree);
        
        
        
        
        /*
         
         */
        void ObtainNFAAndFirstStatus(JBinaryTree<JDFARegNode> *tree, JString& regulation, JGraph<char>& NFA, JSet<int>& firstStatus) {
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

        
        
        
    };
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    /*
        由NFA，从firstStatus开始，转化成DFA
     */
    static int CreateDFAVertex(JNetwork<int, char>& DFA, JSet<JSet<int>>& Dstatus, JMap<int, int>& stat2ver, const JSet<int>& status);
    
    static void CreateDFAFollow(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int end, char ch);
    
    static void CreateDFAFollowAccept(JNetwork<int, char>& DFA, JMap<int, int>& stat2ver, int start, int flag);
    
    static void TransformDFAStatus(const JGraph<char>& NFA, JSet<int>& status, JMap<char, JSet<int>>& classify);
    
    static void NFA2DFA(const JGraph<char>& NFA, const JSet<int>& firstStatus, JNetwork<int, char>& DFA);
    
    
    
    

    
    
};

#endif /* DFA_hpp */
