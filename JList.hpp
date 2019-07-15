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
private:
    static const int BLOCK_SIZE_INITIAL = 10;
    static const int BLOCK_TOTAL_MARGIN = 2;
    
    class Block {
    public:
        T *data = NULL;
        Block *next = NULL;
        Block *prev = NULL;
        Block() {
            data = new T[BLOCK_SIZE_INITIAL];
        }
    };
    
    Block *head = NULL;
    Block *tail = NULL;
    int blockTotal = 0;
    int length = 0;
    
//protected:
    bool _Initialize() {
        LOG_FUNCTION_ENTRY;
        head = new Block;
        tail = head;
        blockTotal = 1;
        length = 0;
        return true;
    }
    
    bool _Closure() {
        LOG_FUNCTION_ENTRY;
        Block *p = NULL;
        while (head != NULL) {
            p = head;
            head = head->next;
            delete p;
        }
        
        tail = NULL;
        blockTotal = 0;
        length = 0;
        return true;
    }
    
    bool _TryExpand() {
        LOG_FUNCTION_ENTRY;
        if (length >= BLOCK_SIZE_INITIAL * blockTotal) {
            Block *p = new Block;
            p->prev = tail;
            tail->next = p;
            tail = tail->next;
            
            blockTotal++;
            return true;
        }
        return false;
    }
    
    bool _TryReduce() {
        LOG_FUNCTION_ENTRY;
        if (length < BLOCK_SIZE_INITIAL * (blockTotal - BLOCK_TOTAL_MARGIN)) {
            Block *p = tail;
            tail = tail->prev;
            tail->next = NULL;
            delete p;
            
            blockTotal--;
            return true;
        }
        return false;
    }
    
    T& _Get(int index) const {
        LOG_FUNCTION_ENTRY;
        int j = index / BLOCK_SIZE_INITIAL;
        int i = index % BLOCK_SIZE_INITIAL;
        Block *p = head;
        for (int x = 0; x < j; x++) {
            p = p->next;
        }
        return p->data[i];
    }
    
    int _Add(const T& t) {
        LOG_FUNCTION_ENTRY;
        int i = length;
        
        _Get(length++) = t;
        
        _TryExpand();
        LOG_DEBUG("length = ", length, ", data = ", t);
        return i;
    }
    
    bool _Delete(int index) {
        LOG_FUNCTION_ENTRY;
        
        _Get(index) = _Get(--length);
        
        _TryReduce();
        LOG_DEBUG("length = ", length, ", index = ", index);
        return true;
    }
    
    bool _Set(int index, const T& t) {
        LOG_FUNCTION_ENTRY;
        
        _Get(index) = t;
        LOG_DEBUG("length = ", length, ", index = ", index, ", data = ", t);
        return true;
    }
    
public:
    JList() {
        LOG_FUNCTION_ENTRY;
        _Initialize();
    }
    
    ~JList() {
        LOG_FUNCTION_ENTRY;
        _Closure();
    }
    
    JList(const JList& jl) {
        LOG_FUNCTION_ENTRY;
        _Initialize();
        int j = jl.length / BLOCK_SIZE_INITIAL;
        int i = jl.length % BLOCK_SIZE_INITIAL;
        Block *p = jl.head;
        for (int t1 = 0; t1 < j; t1++, p = p->next) {
            for (int t2 = 0; t2 < BLOCK_SIZE_INITIAL; t2++) {
                this->_Add(p->data[t2]);
            }
        }
        for (int t = 0; t < i; t++) {
            this->_Add(p->data[t]);
        }
    }
    
    int Length() const {
        return length;
    }
    
    T& Get(int index) const {
        LOG_FUNCTION_ENTRY;
        if (index < 0 || index >= length) {
            LOG_WARN("length = ", length, ", index = ", index);
            return _Get(length);
        }
        return _Get(index);
    }
    
    T& GetTail() {
        LOG_FUNCTION_ENTRY;
        if (length <= 0) {
            LOG_WARN("length = ", length);
            return _Get(length);
        }
        return _Get(length - 1);
    }
    
    int Add() {
        LOG_FUNCTION_ENTRY;
        int i = length;
        
        length++;
        
        _TryExpand();
        LOG_DEBUG("length = ", length);
        return i;
    }
    
    int Add(const T& t) {
        LOG_FUNCTION_ENTRY;
        return _Add(t);
    }
    
    bool Delete(int index) {
        LOG_FUNCTION_ENTRY;
        if (index < 0 || index >= length) {
            LOG_WARN("length = ", length, ", index = ", index);
            return false;
        }
        return _Delete(index);
    }
    
    bool DeleteTail() {
        LOG_FUNCTION_ENTRY;
        LOG_DEBUG("length = ", length);
        if (length < 0) {
           return false;
        }
        length--;
        return true;
    }
    
    bool Set(int index, const T& t) {
        LOG_FUNCTION_ENTRY;
        if (index < 0 || index >= length) {
            LOG_WARN("length = ", length, ", index = ", index);
            return false;
        }
        return _Set(index, t);
    }
    
    void Echo() {
        LOG_FUNCTION_ENTRY;
        LOG_INFO("length = ", length, ", blockTotal = ", blockTotal);
        for (int i = 0; i < length; i++) {
            LOG_INFO("i", i, ": ", _Get(i));
        }
    }
    
    friend std::ostream& operator << (std::ostream& os, const JList& jl) {
//        int j = jl.length / BLOCK_SIZE_INITIAL;
//        int i = jl.length % BLOCK_SIZE_INITIAL;
//        Block *p = jl.head;
//        for (int t1 = 0; t1 < j; t1++, p = p->next) {
//            for (int t2 = 0; t2 < BLOCK_SIZE_INITIAL; t2++) {
//                os << p->data[t2] << ", ";
//            }
//        }
//        for (int t = 0; t < i; t++) {
//            os << p->data[t] << ", ";
//        }
        
        int l = jl.Length();
        for (int i = 0; i < l; i++) {
            os << jl._Get(i) << ", ";
        }
        
        return os;
    }
    
};

#endif /* JList_hpp */
