//
//  Hello.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/6/23.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef Hello_hpp
#define Hello_hpp
#include "JList.hpp"

#include <stdio.h>
#include <iostream>

class Hello {
private:
    int i;
public:
//    void print();
//    void set(int i);
    void print() {
        std::cout << "i = " << i << "\n";
    }
    
    void set(int i) {
        this->i = i;
        
    }
};



#endif /* Hello_hpp */
