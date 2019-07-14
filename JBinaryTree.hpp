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
        virtual void Visit(JBinaryTree<T> *tree) = 0;
    };
    
    class Root {
    private:
        JBinaryTree<T> *tree = NULL;
    public:
        Root() {}
        Root(JBinaryTree<T> *tree) : tree(tree) {}
        ~Root() {
            LOG_FUNCTION_ENTRY;
            JBinaryTree<T>::Destory(tree);
        }
        
        void TraversePreorder(Interface *impl) {
            LOG_FUNCTION_ENTRY;
            JBinaryTree<T>::TraversePreorder(tree, impl);
        }
        
        void TraverseInorder(Interface *impl) {
            LOG_FUNCTION_ENTRY;
            JBinaryTree<T>::TraverseInorder(tree, impl);
        }
        
        void TraversePostorder(Interface *impl) {
            LOG_FUNCTION_ENTRY;
            JBinaryTree<T>::TraversePostorder(tree, impl);
        }
        
        Root& operator = (JBinaryTree<T> *tree) {
            LOG_FUNCTION_ENTRY;
            this->tree = tree;
            return *this;
        }
    };
    
    JBinaryTree() {
        LOG_FUNCTION_ENTRY;
    }
    
    JBinaryTree(const T& node) : node(node) {
        LOG_FUNCTION_ENTRY;
    }
    
    ~JBinaryTree() {
        LOG_FUNCTION_ENTRY;
        LOG_DEBUG(" del ", Node());
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
    
    static void TraversePreorder(JBinaryTree<T> *tree, Interface *impl) {
        if (tree == NULL) return;
        if (impl == NULL) {
            LOG_WARN("impl == NULL");
            return;
        }
        impl->Visit(tree);
        TraversePreorder(tree->lchild, impl);
        TraversePreorder(tree->rchild, impl);
    }
    
    static void TraverseInorder(JBinaryTree<T> *tree, Interface *impl) {
        if (tree == NULL) return;
        if (impl == NULL) {
            LOG_WARN("impl == NULL");
            return;
        }
        TraverseInorder(tree->lchild, impl);
        impl->Visit(tree);
        TraverseInorder(tree->rchild, impl);
    }
    
    static void TraversePostorder(JBinaryTree<T> *tree, Interface *impl) {
        if (tree == NULL) return;
        if (impl == NULL) {
            LOG_WARN("impl == NULL");
            return;
        }
        TraversePostorder(tree->lchild, impl);
        TraversePostorder(tree->rchild, impl);
        impl->Visit(tree);
    }
    
    static void Traverse(JBinaryTree<T> *tree, Interface *impl) {
        TraversePreorder(tree, impl);
    }
    
    static void Destory(JBinaryTree<T> *tree) {
        if (tree == NULL) return;
        Destory(tree->lchild);
        Destory(tree->rchild);
        delete tree;
    }
    
};

#endif /* JBinaryTree_hpp */
