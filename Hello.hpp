//
//  Hello.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/6/23.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef Hello_hpp
#define Hello_hpp

#include <stdio.h>
#include <iostream>

#include "JLog.hpp"
#include "JBinaryTree.hpp"
#include "DFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"
#include "JGraph.hpp"
#include "JSet.hpp"
#include "JList.hpp"
#include "JMap.hpp"


class Hello {
private:
    int i;
public:
//    void print();
//    void set(int i);
    void print() {
        std::cout << "i = " << i << "\n";
    }
    
    void set(int i) {
        this->i = i;
        
    }
    
    void test() {
        LOG_INFO("==============Hello world!==============");
        
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
        
        JSet<char> set2;
        set2.Add(set);
        set2.Echo();
        for (int i = 0; i < 60; i++) {
            set2.Add('A' + i);
        }
        set2.Echo();
        LOG_INFO(set2);
        
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
        
        JMap<int, int> map;
        map.Add(20, 3);
        map.Add(10, 3);
        map.Add(20, 3);
        map.Add(40, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        map.Add(20, 3);
        for (int i = 0; i < 50; i++) {
            map.Add(i, i);
        }
        LOG_INFO(map.Get(20));
        LOG_INFO(map.Get(10));
        LOG_INFO(map.Get(1000));
        map.Echo();
        
        
        LOG_INFO("==============Hello world!==============");
    }
};



#endif /* Hello_hpp */
