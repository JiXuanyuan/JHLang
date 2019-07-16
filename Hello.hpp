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
#include "JDFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"
#include "JGraph.hpp"
#include "JSet.hpp"
#include "JList.hpp"
#include "JMap.hpp"
#include "JNetwork.hpp"


class Hello {
private:
    int i;
public:
    
    template<class T>
    void print(T t) {
        std::cout << "==============Hello world!==============\n";
        std::cout << t << "\n";
    }
    
    void test() {
        LOG_INFO("==============Hello world!==============");
        /*JList<char> lis;
        lis.Add('d');
        lis.Add('d');
        lis.Add('e');
        lis.Add('d');
        lis.Add('e');
        JLog::Rope("sad", 124, "dqwd", true, "89", false, lis, 45, 'q', 34, "csdwq");
        
        for (int i = 0; i < 40; i++) {
            lis.Add('A' + i);
        }
        lis.Echo();
        
        for (int i = 30; i > 0; i--) {
            lis.Delete(i);
        }
        lis.Echo();
        lis.Delete(0);
        lis.Echo();
        LOG_INFO(lis);
        
        lis.Add();
        lis.Add();
        lis.Echo();
        
        lis.Clean();
        lis.Echo();
        
        lis.Add();
        lis.Add();
        for (int i = 0; i < 40; i++) {
            lis.Add('A' + i);
        }
        lis.Echo();
        
        LOG_INFO("==============Hello world!==============");
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
        
        LOG_INFO("==============Hello world!==============");
        JSet<char> set2;
        set2.Add(set);
        set2.Echo();
        for (int i = 0; i < 60; i++) {
            set2.Add('A' + i);
        }
        set2.Echo();
        LOG_INFO(set2);
        
        LOG_INFO("==============Hello world!==============");
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
        
        LOG_INFO("==============Hello world!==============");
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
        JNetwork<int, char> net;
        net.AddVerter(12312);
        net.AddVerter(232);
        net.AddVerter(34312);
        net.AddVerter(12);
        net.AddVerter(5);
        net.AddVerter(122);
        for (int i = 0; i < 50; i++) {
            net.AddVerter(i);
        }
        net.AddArc(1, 2, 'f');
        net.AddArc(1, 0, 'q');
        net.AddArc(1, 3, 'c');
        net.AddArc(3, 0, '2');
        net.AddArc(1, 2, 'd');
        for (int i = 0; i < 50; i++) {
            net.AddArc(4, i, 'A'+ i);
        }
        net.Echo();
        LOG_INFO(net);
        
        LOG_INFO("==============Hello world!==============");
        JSet<JSet<int>> ver1;
        JSet<int> ver2;
        ver2.Add(12);
        ver2.Add(112);
        ver2.Add(2);
        ver2.Add(312);
        ver2.Add(12);
        ver2.Add(52);
        ver2.Add(12);
        
        ver1.Add(ver2);
        LOG_INFO(ver1);
        
        ver1.Add(ver2);
        LOG_INFO(ver1);
        
        ver2.Add(52);
        ver2.Add(1243);
        ver1.Add(ver2);
        LOG_INFO(ver1);
        
        ver1.Add(ver2);
        LOG_INFO(ver1);
        
        ver2.Add(1213);
        ver1.Add(ver2);
        LOG_INFO(ver1);
        
        
        LOG_INFO("==============Hello world!==============");
        JMap<char, JSet<int>> classify;
        
        classify.Pray('f').Add(1);
        classify.Pray('f').Add(2);
        classify.Pray('f').Add(3);
        classify.Pray('f').Add(1);
        classify.Pray('d').Add(13);
        classify.Pray('d').Add(100);
        classify.Pray('d').Add(12);
        classify.Pray('q').Add(134);
        classify.Pray('w').Add(1124);
        classify.Pray('w').Add(1124);
        LOG_INFO(classify);
        
        LOG_INFO("==============Hello world!==============");
        JList<char> lis2;
        for (int i = 0; i < 40; i++) {
            lis2.Add('A' + i);
        }
        lis2.Echo();
        JList<char>::Iterator it = lis2.ObtainIterator();
        for (;it.HasNext();) {
            LOG_INFO(it.Next());
        }
        
        LOG_INFO(lis2);
        JList<char>::Iterator it2 = lis2.ObtainIterator();
        while (it2.HasNext()) {
            LOG_INFO(it2.Next());
        }
        
        */
        LOG_INFO("==============Hello world!==============");
       
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
            set.Add('A' + i);
        }
        set.Echo();
        LOG_INFO(set);
        
        JSet<char> set2;
        set2.Add(set);
        set2.Echo();
        LOG_INFO(set2);
        
        set.Clean();
        set.Echo();
        LOG_INFO(set);
        set2.Echo();
        LOG_INFO(set2);
        
        LOG_INFO("==============Hello world!==============");
        JSet<JSet<int>> Dstatus;
        JMap<char, JSet<int>> classify;
        
        classify.Pray('c').Add(12);
        classify.Pray('c').Add(112);
        classify.Pray('c').Add(123);
        classify.Pray('c').Add(124);
        classify.Pray('c').Add(12);
        Dstatus.Add(classify.Get('c'));
        Dstatus.Add(classify.Get('c'));
        Dstatus.Add(classify.Get('c'));
        
        LOG_INFO("classify: ", classify);
        LOG_INFO("Dstatus: ", Dstatus);
        
        classify.Pray('c').Clean();
        
        LOG_INFO("classify: ", classify);
        LOG_INFO("Dstatus: ", Dstatus);
        
        LOG_INFO("==============Hello world!==============");
    }
};



#endif /* Hello_hpp */
