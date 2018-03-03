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
	if(lsexp == NULL || rsexp == NULL)
		throw std::invalid_argument("one of left or right s-expression is NULL");
	type = NON_ATOM;
	left = lsexp;
	right = rsexp;
}

map<string, SExp*> SExp::symbols;
SExp* SExp::get_symbol(string& str) {
	if(symbols.find(str) == symbols.end()) {
		// cout << endl << endl << "CREATING A NEW ATOM FOR " << str << endl << endl << endl;
		SExp* new_symbolic_atom = new SExp(str);
		symbols[str] = new_symbolic_atom;
		return new_symbolic_atom;
	}
	// cout << endl << endl << "RETURNING FROM MAP INSTEAD " << str << endl << endl << endl;
	return symbols[str];
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

SExp* SExp::get_right() {
	return right;
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
	 	cerr << "SExp type = " << type << endl;
		return "";
	}
	return dot_notation;
}
