//
//  main.cpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include <iostream>
#include "Hello.hpp"
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
 
//    Hello h;
//    h.test();
    
    DFA dfa;
//    dfa.Reg("(qa|b)*a\\*bb");
    dfa.Reg("(a|b)*abb");
    dfa.Reg2NFA();
    
    return 0;
}

