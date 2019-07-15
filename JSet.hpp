//
//  JSet.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/14.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JSet_hpp
#define JSet_hpp

#include "JLog.hpp"
#include "JList.hpp"

template<class T>
class JSet : public JList<T> {
public:
    static const int FALG_EXIST = -1;
    
    int Add(const T& t) {
        LOG_FUNCTION_ENTRY;
        if (Exist(t)) {
            return FALG_EXIST;
        }
        
        return JList<T>::Add(t);
    }
    
    int Add(const JSet<T>& t) {
        LOG_FUNCTION_ENTRY;
        int l = t.JList<T>::Length();
        for (int i = 0; i < l; i++) {
            Add(t.JList<T>::Get(i));
        }
        
        return JList<T>::Length();
    }
    
    bool Exist(const T& t) const {
        int l = JList<T>::Length();
        for (int i = 0; i < l; i++) {
            if(JList<T>::Get(i) == t) {
                return true;
            }
        }
        return false;
    }
    
    bool Equal(const JSet<T>& s) const {
        int ls = s.Length();
        int lt = JList<T>::Length();
        if (ls != lt) {
            return false;
        }
        
        /*
            由于Set不存在相同的值，可用下面方式判断
            2019/07/15(待优化) 内部数据采用有序存储，可优化复杂度
         */
        for (int i = 0; i < ls; i++) {
            if (!Exist(s.Get(i))) {
                return false;
            }
        }
        
        return true;
    }
    
    friend bool operator == (const JSet& s1, const JSet& s2) {
        return s1.Equal(s2);
    }
    
};

#endif /* JSet_hpp */
