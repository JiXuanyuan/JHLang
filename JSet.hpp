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
    int Add(const T& t) {
        LOG_FUNCTION_ENTRY;
        int l = JList<T>::length;
        for (int i = 0; i < l; i++) {
            if (t == JList<T>::data[i]) {
                return i;
            }
        }
        
        return JList<T>::Add(t);
    }
    
    int Add(const JSet<T>& t) {
        LOG_FUNCTION_ENTRY;
        int l = t.length;
        for (int i = 0; i < l; i++) {
            Add(t.JList<T>::data[i]);
        }
        
        return JList<T>::length;
    }
    
};

#endif /* JSet_hpp */
