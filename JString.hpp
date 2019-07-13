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
    int Length();
    char Get(int index);
    void Echo() const;
    
    friend std::ostream& operator << (std::ostream& os, const JString& s) {
        os << s.str;
        return os;
    }
    
};

#endif /* JString_hpp */
