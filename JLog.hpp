//
//  JLog.hpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#ifndef JLog_hpp
#define JLog_hpp

#ifdef DEBUG
    #undef DEBUG
    #define INFO
#endif

#include <iostream>

/*
    程序实现了不定参数的JLog，输入任意类型显示日志，示例：
        JLog::Rope(45, 'q', 34, "sad", 124, "dqwd", true, "89", false);
        输出显示    45q34sad124dqwdtrue89false
    1.实现这个功能，用到了C++多态的特性与C++11标准的变长参数模板
    2.由于template特性，代码只能放到头文件
    3.虽然代码在头文件，但JLog函数必须包括声明，否则函数只能匹配到其上面的模版
    4.JLog实现了部分类型的输出，class类型可通过友元机制、重载运算符实现
         friend ostream& operator << (ostream& os, const T& t) {
            os << t...;
            return os;
         }
    5.2019.07.12:
        由于函数实现部分在头文件，多处引用，可分开编译但无法链接，所以使用class封装
    6.2019.07.12:
        Rope(T v, Types... args) ->   Rope(T& v, Types&... args)
        修改：其它类型参数值传递改为引用
        解决：class类型显示时多次复制、析构引起的问题
    7.2019.07.12:
        _Rope(T& v, Types&... args)
        修改：增加私有函数，参数使用引用，对第6点做相应修改
        解决：避免多次创建参数，特别是class
        由于基础类型int、char引用时，需要变量名，无法匹配全部参数都引用的情况
        目前无法避免第一次传参时创建class
    8.2019.07.12:
        Rope(T v)  ->  Rope(T& v)
        Rope(T v, Types... args) ->   Rope(T& v, Types... args)
        修改：第一个参数是其它类型时，使用引用
        解决：class放在第一个参数时，可避免再创建
 */

class JLog {
public:
    /*
        匹配单个参数
     */
    static void Rope(int v) {
        std::cout << v << std::endl;
    }
    static void Rope(bool v) {
        std::cout << (v? "true" : "false") << std::endl;
    }
    static void Rope(char v) {
        std::cout << v << std::endl;
    }
    static void Rope(char * v) {
        std::cout << v << std::endl;
    }
    template<class T>
    static void Rope(T v) {
        std::cout << v << std::endl;
    }
    
    /*
        匹配多个参数
     */
    template<class... Types>
    static void Rope(int v, Types... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class... Types>
    static void Rope(bool v, Types... args) {
        std::cout << (v? "true" : "false");
        _Rope(args...);
    }
    template<class... Types>
    static void Rope(char v, Types... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class... Types>
    static void Rope(char * v, Types... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class T, class... Types>
    static void Rope(T v, Types... args) {
        std::cout << v;
        _Rope(args...);
    }
    
private:
    /*
        匹配单个参数
        内部私有函数，使用引用，避免多次创建参数，特别class
     */
    static void _Rope(int& v) {
        std::cout << v << std::endl;
    }
    static void _Rope(bool& v) {
        std::cout << (v? "true" : "false") << std::endl;
    }
    static void _Rope(char& v) {
        std::cout << v << std::endl;
    }
    static void _Rope(char * & v) {
        std::cout << v << std::endl;
    }
    template<class T>
    static void _Rope(T& v) {
        std::cout << v << std::endl;
    }
    
    /*
        匹配多个参数
     */
    template<class... Types>
    static void _Rope(int& v, Types&... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class... Types>
    static void _Rope(bool& v, Types&... args) {
        std::cout << (v? "true" : "false");
        _Rope(args...);
    }
    template<class... Types>
    static void _Rope(char& v, Types&... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class... Types>
    static void _Rope(char * & v, Types&... args) {
        std::cout << v;
        _Rope(args...);
    }
    template<class T, class... Types>
    static void _Rope(T& v, Types&... args) {
        std::cout << v;
        _Rope(args...);
    }
    
};

/*
    控制日志输出等级
 */

//#ifdef  __cplusplus
//extern "C" {
//#endif

#include <stdio.h>

#define LOG_LEVEL_ALL           0
#define LOG_LEVEL_DEBUG         1
#define LOG_LEVEL_INFO          2
#define LOG_LEVEL_WARN          3
#define LOG_LEVEL_ERROR         4
#define LOG_LEVEL_OFF           9
#define LOG_ACROSS_LEVEL        LOG_LEVEL_DEBUG

#define _LOG(level, tag, ...)    \
        do { \
            if ((level) >= LOG_ACROSS_LEVEL) { \
                printf("# %s, %s, %d, %s     #     ", \
                        tag, __FILE__, __LINE__, __FUNCTION__); \
                JLog::Rope(__VA_ARGS__); \
            } \
        } while(0)

#define LOG_DEBUG(...)          _LOG(LOG_LEVEL_DEBUG, "DEBUG", __VA_ARGS__)
#define LOG_INFO(...)           _LOG(LOG_LEVEL_INFO, "INFO", __VA_ARGS__)
#define LOG_WARN(...)           _LOG(LOG_LEVEL_WARN, "WARN", __VA_ARGS__)
#define LOG_ERROR(...)          _LOG(LOG_LEVEL_ERROR, "ERROR", __VA_ARGS__)
#define LOG_FUNCTION_ENTRY      _LOG(LOG_LEVEL_ALL, "Function entry", "")

/*
    开发模式：DEBUG（调试）、INFO、 其他
 */
#ifdef ALL
    #undef LOG_ACROSS_LEVEL
    #define LOG_ACROSS_LEVEL        LOG_LEVEL_ALL
#else
#ifdef DEBUG
    #undef LOG_ACROSS_LEVEL
    #define LOG_ACROSS_LEVEL        LOG_LEVEL_DEBUG
#else
#ifdef INFO
    #undef LOG_ACROSS_LEVEL
    #define LOG_ACROSS_LEVEL        LOG_LEVEL_INFO
#else
    #undef LOG_ACROSS_LEVEL
    #define LOG_ACROSS_LEVEL        LOG_LEVEL_WARN
    #undef LOG_DEBUG
    #define LOG_DEBUG(...)
    #undef LOG_INFO
    #define LOG_INFO(...)
    #undef LOG_FUNCTION_ENTRY
    #define LOG_FUNCTION_ENTRY
#endif
#endif
#endif

//#ifdef  __cplusplus
//}
//#endif  /* end of __cplusplus */

#endif /* JLog_hpp */
