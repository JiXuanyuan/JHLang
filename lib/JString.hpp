//
//  JString.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JString_hpp
#define JString_hpp

#include "JLog.hpp"
#include "JList.hpp"

class JString : protected JList<char> {
protected:
    
    inline void _CopyTail(const char *str) {
        LOG_FUNCTION_ENTRY;
        for (int i = 0; str[i] != '\0'; i++) {
            JList<char>::Add(str[i]);
        }
    }
    
public:
    JString() : JList<char>() {
        LOG_FUNCTION_ENTRY;
    }
    
    JString(const char *str) : JList<char>() {
        LOG_FUNCTION_ENTRY;
        _CopyTail(str);
    }
    
    JString(const JString& jstr) : JList<char>(jstr)  {
        LOG_FUNCTION_ENTRY;
    }
    
    int Length() const {
        return JList<char>::Length();
    }
    
    char& Get(int index) const {
        return JList<char>::Get(index);
    }
    
    JString& Assign(const char *str) {
        LOG_FUNCTION_ENTRY;
        JList<char>::Clean();
        _CopyTail(str);
        return *this;
    }
    
    JString& Assign(const char *str, int length) {
        LOG_FUNCTION_ENTRY;
        JList<char>::Clean();
        for (int i = 0; i < length; i++) {
            JList<char>::Add(str[i]);
        }
        return *this;
    }
    
    JString& Assign(const JString& jstr) {
        LOG_FUNCTION_ENTRY;
        JList<char>::Clean();
        JList<char>::AddList(jstr);
        return *this;
    }
    
    JString& Assign(const JString& jstr, int start, int end) {
        LOG_FUNCTION_ENTRY;
        JList<char>::Clean();
        for (int i = start; i < end; i++) {
            JList<char>::Add(jstr.Get(i));
        }
        return *this;
    }
    
    JString& Merge(const char *str) {
        LOG_FUNCTION_ENTRY;
        _CopyTail(str);
        return *this;
    }
    
    JString& Merge(char ch) {
        LOG_FUNCTION_ENTRY;
        JList<char>::Add(ch);
        return *this;
    }
    
    JString& Merge(const JString& jstr) {
        LOG_FUNCTION_ENTRY;
        JList<char>::AddList(jstr);
        return *this;
    }
    
    void Clean() {
        JList<char>::Clean();
    }
    
    bool Equal(const JString& jstr) const {
        LOG_FUNCTION_ENTRY;
        int ls = jstr.Length();
        int lt = JList<char>::Length();
        if (ls != lt) {
            return false;
        }

        for (int i = 0; i < ls; i++) {
            if (jstr.Get(i) != Get(i)) {
                return false;
            }
        }
        return true;
    }
    
    JString& operator = (const JString& jstr) {
        return Assign(jstr);
    }
    
    JString& operator = (const char *str) {
        return Assign(str);
    }
    
    bool operator == (const JString& jstr) {
        return Equal(jstr);
    }
    
    friend std::ostream& operator << (std::ostream& os, const JString& jstr) {
        int l = jstr.Length();
        for (int i = 0; i < l; i++) {
            os << jstr.Get(i);
        }
        return os;
    }
};

/*
class JString {
private:
    static const int SIZE_INITIAL = 50;
    static const int SIZE_GROWTH = 20;
    
    char *str = NULL;
    int length = 0;
    int size = SIZE_INITIAL;
    
    bool Initialize();
    bool Expand();
    
public:
    JString();
    JString(const char *str);
    JString(JString& jstr);
    ~JString();
    
    bool Assign(const char *str);
    bool Merge(const char *str);
    int Length() const;
    char Get(int index) const;
    void Echo() const;
    
    friend std::ostream& operator << (std::ostream& os, const JString& s) {
        os << s.str;
        return os;
    }
    
};*/

#endif /* JString_hpp */
