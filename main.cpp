//
//  main.cpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include <iostream>
#include "JLog.hpp"
#include "Hello.hpp"
#include "JDFA.hpp"
#include "JLex.hpp"

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
    
//    JDFA dfa("(a|b)*abb");
////    dfa.Reg("(q|k|j h|j h 88*a|bn|h)*a\\*bbgv*|v|f*|fjf");
////    dfa.Reg("(a|b)*abb");
//    LOG_INFO(dfa.Reg("(a|b)*abb").ObtainDFA());
//    LOG_INFO(dfa.ObtainDFA());
//    LOG_INFO(dfa.ObtainDFA());
    
    JLex lex;
    lex.Test();
    
    return 0;
}

