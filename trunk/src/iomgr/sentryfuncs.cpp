/***************************************************************
 * Name:      sentryfuncs.cpp
 * Purpose:   Implementation of various Sentry functions
 * Author:    Ricardo Garcia (rick.g777 {at} gmail {dot} com)
 * Date :     2008-09-11
 * Copyright: (c) Ricardo Garcia
 * License:   WxWindows License
 ***************************************************************/

#include "sentryfuncs.h"

syBoolSetter::syBoolSetter(bool& flag,bool newvalue) :
m_Flag(flag) {
    m_Old = m_Flag;
    m_Flag = newvalue;
}

syBoolSetter::syBoolSetter(volatile bool& flag,bool newvalue) :
m_Flag((bool&)flag) {
    m_Old = m_Flag;
    m_Flag = newvalue;
}

syBoolSetter::~syBoolSetter() {
    m_Flag = m_Old;
}

syIntSetter::syIntSetter(int& dest,int newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syIntSetter::syIntSetter(volatile int& dest,int newvalue) :
m_Var((int&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syIntSetter::~syIntSetter() {
    m_Var = m_Old;
}

syUintSetter::syUintSetter(unsigned int& dest,unsigned int newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syUintSetter::syUintSetter(volatile unsigned int& dest,unsigned int newvalue) :
m_Var((unsigned int&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syUintSetter::~syUintSetter() {
    m_Var = m_Old;
}

syLongSetter::syLongSetter(long& dest,long newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syLongSetter::syLongSetter(volatile long& dest,long newvalue) :
m_Var((long&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syLongSetter::~syLongSetter() {
    m_Var = m_Old;
}

syULongSetter::syULongSetter(unsigned long& dest,unsigned long newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syULongSetter::syULongSetter(volatile unsigned long& dest,unsigned long newvalue) :
m_Var((unsigned long&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syULongSetter::~syULongSetter() {
    m_Var = m_Old;
}

syCharSetter::syCharSetter(char& dest,char newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syCharSetter::syCharSetter(volatile char& dest,char newvalue) :
m_Var((char&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syCharSetter::~syCharSetter() {
    m_Var = m_Old;
}

syUcharSetter::syUcharSetter(unsigned char& dest,unsigned char newvalue) :
m_Var(dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syUcharSetter::syUcharSetter(volatile unsigned char& dest,unsigned char newvalue) :
m_Var((unsigned char&)dest) {
    m_Old = m_Var;
    m_Var = newvalue;
}

syUcharSetter::~syUcharSetter() {
    m_Var = m_Old;
}
