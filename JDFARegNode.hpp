//
//  JDFARegNode.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/16.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JRegNode_hpp
#define JRegNode_hpp

#include "JLog.hpp"

class JDFARegNode {
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
    
    bool IsOperator() const { return type == TYPE_OPER; }
    
    bool IsCharacter() const { return type == TYPE_CHER; }
    
    char Value() const { return val; }
    
    int RegIndex() const { return regIndex; }
    
    friend std::ostream& operator << (std::ostream& os, const JDFARegNode& n) {
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

#endif /* JRegNode_hpp */
