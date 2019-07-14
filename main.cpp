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

using namespace std;

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    
//    JSet<char> set;
//    set.Add('d');
//    set.Add('d');
//    set.Add('e');
//    set.Add('d');
//    set.Add('e');
//    set.Echo();
//    JList<char> ll;
//    for (int i = 0; i < 60; i++) {
//        ll.Add('q');
//    }
//    ll.Echo();
    JGraph<char> graph;
    graph.AddVerter('a');
    graph.AddVerter('c');
    graph.AddVerter('d');
    graph.AddVerter('a');
//    for (int i = 0; i < 50; i++) {
//        graph.AddVerter('q');
//    }
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

