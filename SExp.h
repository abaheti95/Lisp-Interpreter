#ifndef SExp_H
#define SExp_H

#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <stdexcept>
using namespace std;

const int INTEGER_ATOM = 1;
const int SYMBOLIC_ATOM = 2;
const int NON_ATOM = 3;

class SExp
{
private:
	int type;
	int val;
	string name;
	SExp* left;
	SExp* right;
public:
	SExp();
	SExp(int value);
	SExp(string& str);
	SExp(SExp* lsexp, SExp* rsexp);
	~SExp() {};
	static map<string, SExp*> symbols;
	static SExp* get_symbol(string& str);
	int get_type();
	int get_value();
	string get_name();
	SExp* get_left();
	SExp* get_right();
	string get_dot_notation();			// Print the s-expression in lisp . notation

	bool atom();
	bool sym_atom();			// Checks whether it is a symbolic atom
	bool int_atom();			// Checks whether it is an integer atom
	bool null();
	bool isT();
	bool eq(SExp* atom);					// Checks if the two SExp are same
	SExp* car();
	SExp* cdr();

	bool in(SExp* list);		// Checks if current atom is present in the list and returns boolean
	SExp* getval(SExp* list);	// Checks if current atom present in the list. Returns its cdr if present.
	int len();					// Returns the length of list starting from current element
};

#endif