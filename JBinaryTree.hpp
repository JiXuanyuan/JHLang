//
//  JBinaryTree.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JBinaryTree_hpp
#define JBinaryTree_hpp

#include "JLog.hpp"

template<class T>
class JBinaryTree {
private:
    T node;
    JBinaryTree<T> *lchild = NULL;
    JBinaryTree<T> *rchild = NULL;
public:
    class Interface {
    public:
        virtual void func(JBinaryTree<T> *tree) = 0;
    };
    
    JBinaryTree() {
        LOG_FUNCTION_ENTRY;
    }
    
    JBinaryTree(const T& node) : node(node) {
        LOG_FUNCTION_ENTRY;
    }
    
    ~JBinaryTree() {
        LOG_FUNCTION_ENTRY;
        LOG_INFO(" del ", Node());
    }

    T& Node() {
        return node;
    }
    
    JBinaryTree<T>* LeftChild() {
        return lchild;
    }
    
    JBinaryTree<T>* RightChild() {
        return rchild;
    }
    
    JBinaryTree<T> * Merge(JBinaryTree<T> *lchild, JBinaryTree<T> *rchild) {
        LOG_FUNCTION_ENTRY;
        this->lchild = lchild;
        this->rchild = rchild;
        return this;
    }
    
    static void Traverse(JBinaryTree<T> *tree, Interface *impl) {
        if (tree == NULL) return;
        impl->func(tree);
        Traverse(tree->lchild, impl);
        Traverse(tree->rchild, impl);
    }
    
    static void Destory(JBinaryTree<T> *tree) {
        if (tree == NULL) return;
        Destory(tree->lchild);
        Destory(tree->rchild);
        delete tree;
    }
};

//template<class T>
//BinaryTree<T>* BinaryTree<T>::Merge(BinaryTree<T> *lchild, BinaryTree<T> *rchild)
//{
//    this->lchild = lchild;
//    this->rchild = rchild;
//    return this;
//}
//
//template<class T>
//void BinaryTree<T>::Traverse(BinaryTree<T> *tree)
//{
//    if (tree == NULL) return;
//    Traverse(tree->lchild);
//    Traverse(tree->rchild);
//    cout << tree->Node() << endl;
//}
//
//template<class T>
//void BinaryTree<T>::Destory(BinaryTree<T> *tree)
//{
//    if (tree == NULL) return;
//    delete tree;
//    Destory(tree->lchild);
//    Destory(tree->rchild);
//}

#endif /* JBinaryTree_hpp */
