//
//  main.cpp
//  JHLang
//
//  Created by 陈佳辉 on 2019/7/13.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include <iostream>
#include "Hello.hpp"
#include "JBinaryTree.hpp"
#include "DFA.hpp"
#include "JLog.hpp"
#include "JString.hpp"

using namespace std;

//#ifdef DEBUG
//#undef DEBUG
//#define BATE
//#endif

class Impl {
public:
    virtual void fun() = 0;
};

class Test : public Impl {
private:
    int i;
public:
    void set(int i) {
        this->i = i;
    }
    void print() {
        std::cout << "i=" << i << "\n";
    }
    virtual void fun() {
        std::cout << "Impl\n";
    }
};

void testImpl(Impl& impl) {
    impl.fun();
}

class Test1 : public Test {
public:
    virtual void fun() {
        std::cout << "Impl111\n";
    }
    
    Test1() {
        std::cout << "Test()\n";
    }
    
    Test1(Test1& t) {
        std::cout << "~Test(Test1& t)\n";
    }
    
    ~Test1() {
        std::cout << "~Test1()\n";
    }
    
    friend ostream& operator << (ostream& os, const Test1& t) {
        os << "0000000000";
        return os;
    }
};

class Pf : public JBinaryTree<int>::Interface {
public:
    virtual void func(JBinaryTree<int> *tree) {
        std::cout << tree->Node() << std::endl;
    }
};

void aaaa(int& a) {
    std::cout << a << std::endl;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    //    Test t;
    //    t.set(99);
    //    t.print();
    //    t.fun();
    //    testImpl(t);
    //
    //    Test1 t1;
    //    t1.fun();
    //    testImpl(t1);
    //
    //    Hello h;
    //    h.set(6664);
    //    h.print();
    //
    
    
    //    JBinaryTree<int> bt1(114), bt2(13), bt3(34);
    //    Pf pf;
    //    bt1.Merge(&bt2, &bt3);
    //    bt1.Traverse(&bt1, &pf);
    //
    //    JBinaryTree<int> *fc = NULL;
    //    JBinaryTree<int> *rc = NULL;
    //    JBinaryTree<int> *lc = NULL;
    //    fc = new JBinaryTree<int>(100);
    //    for (int i = 0; i < 5; i++) {
    //        lc = fc;
    //        fc = new JBinaryTree<int>(0);
    //        rc = new JBinaryTree<int>(i);
    //        fc = fc->Merge(lc, rc);
    //    }
    //
    //    bt1.Traverse(fc, &pf);
    
    //
    //
    //    LOG_DEBUG("qwdqw");
    //    LOG_INFO("asdafadw");
    //    LOG_WARN("dasdqw");
    //    LOG_ERROR("fewfew");
    //
    //    JLog::Rope(45, 'q', 34, "sad", 124, t1, "dqwd", true, "89", false, t1);
    //
    //    JString jstr;
    //    jstr.Assign("sadasdwq");
    //    jstr.Echo();
    //
    //    JString jstr1("wd2d");
    //    jstr1.Echo();
    //    JString jstr1("wd2d");
    //    jstr1.Echo();
    //    LOG_DEBUG(t1, "qwdqw", "sdasd", 11, "---wqeqw");
    //    LOG_DEBUG(45);
    
    
    DFA dfa;
    dfa.Reg("(ada(qw)*)");
    dfa.Reg2NFA();
    
    return 0;
}

