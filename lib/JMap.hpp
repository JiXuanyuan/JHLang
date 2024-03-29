//
//  JMap.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/15.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JMap_hpp
#define JMap_hpp

#include "JLog.hpp"
#include "JList.hpp"

template<class K, class V>
class JMapPair {
public:
    K key;
    V value;
    
    friend std::ostream& operator << (std::ostream& os, const JMapPair& m) {
        os << "{ key: "<< m.key << "; value: " << m.value << " }";
        return os;
    }
};

template<class K, class V>
class JMap : public JList<JMapPair<K, V>> {
public:
    
    int Add(const K& key, const V& value) {
        LOG_FUNCTION_ENTRY;
        
        int i = Exist(key);
        if (i == JLIST_FALG_NOT_EXIST) {
            
            int i = JList<JMapPair<K, V>>::Create();
            JMapPair<K, V> &p = JList<JMapPair<K, V>>::Get(i);
            p.key = key;
            p.value = value;
            return JList<JMapPair<K, V>>::Length();
        }
        return i;
    }
    
    V& Pray(const K& key) {
        LOG_FUNCTION_ENTRY;
        int l = JList<JMapPair<K, V>>::Length();
        for (int i = 0; i < l; i++) {
            if(JList<JMapPair<K, V>>::Get(i).key == key) {
                return JList<JMapPair<K, V>>::Get(i).value;
            }
        }
        
        int i = JList<JMapPair<K, V>>::Create();
        JMapPair<K, V> &p = JList<JMapPair<K, V>>::Get(i);
        p.key = key;
        return p.value;
    }
    
    int Exist(const K& key) const {
        int l = JList<JMapPair<K, V>>::Length();
        for (int i = 0; i < l; i++) {
            if(JList<JMapPair<K, V>>::Get(i).key == key) {
                return i;
            }
        }
        return JLIST_FALG_NOT_EXIST;
    }
    
    V& GetByKey(const K& key) const {
        LOG_FUNCTION_ENTRY;
        int l = JList<JMapPair<K, V>>::Length();
        for (int i = 0; i < l; i++) {
            JMapPair<K, V>& p = JList<JMapPair<K, V>>::Get(i);
            if(p.key == key) {
                return p.value;
            }
        }
        
        return JList<JMapPair<K, V>>::Get(l).value;
    }
    
};

#endif /* JMap_hpp */
