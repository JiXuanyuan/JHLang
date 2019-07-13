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

template<class T>
class JStack {
private:
    static const int SIZE_INITIAL = 50;
    static const int SIZE_GROWTH = 20;
    
    T *data = NULL;
    int length = 0;
    int size = SIZE_INITIAL;
    
    bool Initialize() {
        LOG_FUNCTION_ENTRY;
        data = new T[SIZE_INITIAL];
        return true;
    }
    
    bool Expand() {
        LOG_FUNCTION_ENTRY;
        int nsize = size + SIZE_GROWTH;
        T *p = new T[nsize];
        for (int i = 0; i < length; i++) {
            p[i] = data[i];
        }
        delete [] data;
        data = p;
        return true;
    }
    
public:
    JStack() {
        LOG_FUNCTION_ENTRY;
        Initialize();
    }
    
    JStack(const T& t) {
        LOG_FUNCTION_ENTRY;
        Initialize();
        Push(t);
    }
    
    ~JStack() {
        LOG_FUNCTION_ENTRY;
        delete [] data;
    }
    
    T GetTop() {
        LOG_FUNCTION_ENTRY;
        int i = length - 1;
        LOG_DEBUG("data = ", data[i], ", length = ", length);
        return data[length - 1];
    }
    
    bool Push(const T& t) {
        LOG_FUNCTION_ENTRY;
        if (length >= size) {
            Expand();
        }
        data[length] = t;
        length++;
        LOG_DEBUG("data = ", t, ", length = ", length);
        return true;
    }
    
    T Pop() {
        LOG_FUNCTION_ENTRY;
        length--;
        LOG_DEBUG("data = ", data[length], ", length = ", length);
        return data[length];
    }
    
};

#endif /* JStack_hpp */
