//
//  main.cpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include <iostream>
//#include "Hello.hpp"
#include "JBinaryTree.hpp"
#include "DFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"
#include "JGraph.hpp"
#include "JSet.hpp"

#include "JList.hpp"

using namespace std;

//#ifdef DEBUG
//    #undef DEBUG
//    #define LOG_ACROSS_LEVEL        LOG_LEVEL_DEBUG
//#endif

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
    JList<char> lis;
    lis.Add('d');
    lis.Add('d');
    lis.Add('e');
    lis.Add('d');
    lis.Add('e');
    for (int i = 0; i < 40; i++) {
        lis.Add('q');
    }
    lis.Echo();
    
    for (int i = 30; i > 0; i--) {
        lis.Delete(i);
    }
    lis.Echo();
    lis.Delete(0);
    lis.Echo();
    LOG_INFO(lis);
    
    JSet<char> set;
    set.Add('1');
    set.Add('2');
    set.Add('3');
    set.Add('4');
    set.Add('5');
    set.Add('4');
    set.Add('5');
    set.Echo();
    for (int i = 0; i < 60; i++) {
        set.Add('q');
    }
    for (int i = 0; i < 10; i++) {
        set.Add('0' + i);
    }
    for (int i = 0; i < 60; i++) {
        set.Add('a' + i);
    }
    set.Echo();
    LOG_INFO(set);
    
    JGraph<char> graph;
    graph.AddVerter('a');
    graph.AddVerter('c');
    graph.AddVerter('d');
    graph.AddVerter('a');
    for (int i = 0; i < 50; i++) {
        graph.AddVerter('q');
    }
    graph.AddArc(1, 2);
    graph.AddArc(1, 0);
    graph.AddArc(1, 3);
    graph.AddArc(3, 0);
    graph.AddArc(1, 2);
    graph.Echo();

    DFA dfa;
    dfa.Reg("(qa|b)*a\\*bb");
    dfa.Reg2NFA();
    
    return 0;
}

