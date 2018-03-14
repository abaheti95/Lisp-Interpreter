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

bool SExp::atom() {
	return type == INTEGER_ATOM || type == SYMBOLIC_ATOM;
}

bool SExp::sym_atom() {
	return type == SYMBOLIC_ATOM;
}

bool SExp::int_atom() {
	return type == INTEGER_ATOM;
}

bool SExp::null() {
	return (type == SYMBOLIC_ATOM) && !(name.compare("NIL"));
}

bool SExp::isT() {
	return (type == SYMBOLIC_ATOM) && !(name.compare("T"));
}

bool SExp::eq(SExp* atom) {
	if(type == atom->type) {
		if(type == INTEGER_ATOM)
			return val == atom->get_value();
		else if(type == SYMBOLIC_ATOM)
			return !name.compare(atom->get_name());
		else
			// TODO: Throw error
			return false;
	}
	return false;
}

SExp* SExp::car() {
	if(type == NON_ATOM)
		return left;
	return NULL;
}

SExp* SExp::cdr() {
	if(type == NON_ATOM)
		return right;
	return NULL;
}

bool SExp::in(SExp* list) {
	if(list->null())
		return false;
	else if(this->eq(list->car()->car())) {
		return true;
	} else {
		return in(list->cdr());
	}
}

SExp* SExp::getval(SExp* list) {
	if(list->null()) {
		// This is error. Atom not present in the list
		return NULL;
	} else if(this->eq(list->car()->car())) {
		return list->car()->cdr();
	} else {
		return getval(list->cdr());
	}
}

int SExp::len() {
	if(this->null()) {
		return 0;
	} else if(type == NON_ATOM) {
		return 1 + this->cdr()->len();
	} else {
		// No NIL found. Not a list
		return -10000000;
	}
}