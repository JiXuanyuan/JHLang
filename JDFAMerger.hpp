//
//  JDFAMerger.hpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/17.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JDFAMerger_hpp
#define JDFAMerger_hpp

#include "JLog.hpp"
#include "JDFA.hpp"
#include "JList.hpp"
#include "JString.hpp"

class JDFAMerger {
    
    
public:
    int length = 0;
    const char* reg[][2];
    
    void Prepare(const char *regulation, const char *separator) {
        reg[length][0] = regulation;
        reg[length][1] = separator;
    }
    
    void Merger() {
        
    }
};



#endif /* JDFAMerger_hpp */
