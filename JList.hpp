//
//  JList.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JList_hpp
#define JList_hpp

#include "JLog.hpp"

template<class T>
class JList {
protected:
    static const int SIZE_INITIAL = 50;
    static const int SIZE_GROWTH = 30;
    
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
//        int nsize = size + SIZE_GROWTH;
//        T *p = new T[nsize];
//        for (int i = 0; i < length; i++) {
//            p[i] = data[i];
//        }
//        delete [] data;
//        data = p;
        return true;
    }
    
public:
    JList() {
        LOG_FUNCTION_ENTRY;
        Initialize();
    }
    
    ~JList() {
        LOG_FUNCTION_ENTRY;
        delete [] data;
    }
    
    JList(JList& s) {
        LOG_FUNCTION_ENTRY;
        Initialize();
        for (int i = 0; i < s.length; i++) {
            this->data[i] = s.data[i];
            length++;
            
            if (length >= size) {
                Expand();
            }
        }
    }
    
    int Length() {
        return length;
    }
    
    T& Get(int index) {
        return data[index];
    }
    
    T& GetTail() {
        return data[length - 1];
    }
    
    void Set(int index, const T& t) {
        data[index] = t;
    }
    
    int Add() {
        LOG_FUNCTION_ENTRY;
        length++;
        if (length >= size) {
            Expand();
        }
        LOG_DEBUG("length = ", length);
        return length - 1;
    }
    
    int Add(const T& t) {
        LOG_FUNCTION_ENTRY;
        data[length] = t;
        length++;
        if (length >= size) {
            Expand();
        }
        LOG_DEBUG("data = ", t, ", length = ", length);
        return length - 1;
    }
    
    void Delete(int index) {
        LOG_FUNCTION_ENTRY;
        for (int i = index; i < length; i++) {
            data[i] = data[i+1];
        }
        length--;
        LOG_DEBUG("data = ", data[length], ", length = ", length);
    }
    
    void DeleteTail() {
        LOG_FUNCTION_ENTRY;
        length--;
        LOG_DEBUG("data = ", data[length], ", length = ", length);
    }
    
    void Echo() {
        for (int i = 0; i < length; i++) {
            LOG_INFO(data[i]);
        }
    }
    
    friend std::ostream& operator << (std::ostream& os, const JList& jl) {
        for (int i = 0; i < jl.length; i++) {
            os << jl.data[i] << ", ";
        }
        return os;
    }
    
};

#endif /* JList_hpp */
