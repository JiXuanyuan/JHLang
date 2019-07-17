//
//  JStack.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JStack_hpp
#define JStack_hpp

#include "JLog.hpp"
#include "JList.hpp"

template<class T>
class JStack : protected JList<T>  {
public:
//    JStack() : JList<T>()  {
//        LOG_FUNCTION_ENTRY;
//    }
    
    JStack(const T& t) : JList<T>()  {
        LOG_FUNCTION_ENTRY;
        Push(t);
    }
    
    T GetTop() {
        LOG_FUNCTION_ENTRY;
        return JList<T>::GetTail();
    }
    
    bool Empty() {
        // 初始化增加了一个字符
        return JList<T>::Length() < 1;
    }
    
    int Push(const T& t) {
        LOG_FUNCTION_ENTRY;
        return JList<T>::Add(t);
    }
    
    T Pop() {
        LOG_FUNCTION_ENTRY;
        T t = JList<T>::GetTail();
        JList<T>::DeleteTail();
        return t;
    }
    
    friend std::ostream& operator << (std::ostream& os, const JStack& js) {
        os << "[ ";
        int l = js.Length();
        for (int i = 0; i < l; i++) {
            os << js.Get(i) << ", ";
        }
        os << "]";
        
        return os;
    }
    
};

#endif /* JStack_hpp */
