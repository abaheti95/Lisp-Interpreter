#include "SExp.h"

SExp::SExp() {
	type = 0;
	name = "";
	left = NULL;
	right = NULL;
}

SExp::SExp(int value) {
	type = INTEGER_ATOM;
	val = value;
}

SExp::SExp(string& str) {
	type = SYMBOLIC_ATOM;
	name.assign(str);
}

SExp::SExp(SExp* lsexp, SExp* rsexp) {
	type = NON_ATOM;
	left = lsexp;
	right = rsexp;
}

int SExp::get_type() {
	return type;
}

int SExp::get_value() {
	if(type == INTEGER_ATOM) {
		return val;
	} else {
		cerr << "This s-expression doesn't have a value" << endl;
		return 0;
	}
}

string SExp::get_name() {
	if(type == SYMBOLIC_ATOM) {
		return name;
	} else {
		cerr << "This s-expression is not a symbolic atom" << endl;
		return "";
	}
}

SExp* SExp::get_left() {
	return left;
}

void SExp::set_left(SExp* lsexp) {
	left = lsexp;
}

SExp* SExp::get_right() {
	return right;
}

void SExp::set_right(SExp* rsexp) {
	right = rsexp;
}

string SExp::get_dot_notation() {
	// Basic inorder traversal
	string dot_notation = "";
	if(type == INTEGER_ATOM) {
		stringstream out;
		out << val;
		dot_notation = out.str();
	} else if(type == SYMBOLIC_ATOM) {
		dot_notation = name;
	} else if(type == NON_ATOM) {
		dot_notation += "(";
		dot_notation += left->get_dot_notation();
		dot_notation += ".";
		dot_notation += right->get_dot_notation();
		dot_notation += ")";
	} else {
		cerr << "ERROR!!" << endl;
	}
	return dot_notation;
}
