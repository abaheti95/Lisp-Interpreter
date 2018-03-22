#ifndef LISP_BACK_END_H
#define LISP_BACK_END_H

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "SExp.h"

extern string T;
extern string nil;
extern SExp* dlist;
const string QUOTE = "QUOTE";
const string COND = "COND";
const string DEFUN = "DEFUN";

// Predefined functions
string itostring(int i);

SExp* eval(SExp* input, SExp* alist);
SExp* evlis(SExp* input, SExp* alist);
SExp* evcon(SExp* be_input, SExp* alist);
SExp* addpairs(SExp* parameters_list, SExp* x, SExp* alist);
SExp* apply(SExp* f, SExp* x, SExp* alist);

struct LispException : public std::exception
{
   std::string s;
   LispException(std::string ss) : s(ss) {}
   ~LispException() throw () {} // Updated
   const char* what() const throw() { return s.c_str(); }
};

#endif