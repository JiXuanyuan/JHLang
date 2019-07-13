//
//  JString.cpp
//  demo
//
//  Created by 陈佳辉 on 2019/7/11.
//  Copyright © 2019 陈佳辉. All rights reserved.
//

#include "JString.hpp"
#include "JLog.hpp"

bool JString::Initialize() {
    LOG_FUNCTION_ENTRY;
    str = new char[SIZE_INITIAL];
    for (int i = 0; i < SIZE_INITIAL; i++) {
        str[i] = '\0';
    }
    return true;
}

bool JString::Expand() {
    LOG_FUNCTION_ENTRY;
    int nsize = size + SIZE_GROWTH;
    char *p = new char[nsize];
    for (int i = 0; i < length; i++) {
        p[i] = str[i];
    }
    for (int j = length; j < nsize; j++) {
        p[j] = '\0';
    }
    delete [] str;
    str = p;
    return true;
}

JString::JString() {
    LOG_FUNCTION_ENTRY;
    Initialize();
}

JString::JString(const char *str) {
    LOG_FUNCTION_ENTRY;
    Initialize();
    Assign(str);
}

JString::JString(JString& s) {
    LOG_FUNCTION_ENTRY;
    Initialize();
    Assign(s.str);
}

JString::~JString() {
    LOG_FUNCTION_ENTRY;
    delete [] str;
}

bool JString::Assign(const char *str) {
    LOG_FUNCTION_ENTRY;
    if (length != 0) {
        LOG_WARN("length = ", length);
        return false;
    }
    
    Merge(str);
    return true;
}

bool JString::Merge(const char *str) {
    LOG_FUNCTION_ENTRY;
    for (int i = 0; str[i] != '\0'; i++) {
        if (length >= size) {
            Expand();
        }
        this->str[length] = str[i];
        length++;
    }
    return true;
}

int JString::Length() {
    return length;
}

char JString::Get(int index) {
    if (index < 0 || index >= length) {
        LOG_WARN("length = ", length, ", index = ", index);
        return '\0';
    }
    return str[index];
}

void JString::Echo() const {
    LOG_INFO("length = ", length, ", size = ", size);
    LOG_INFO("str = ", str);
}
