#include "lisp_back_end.h"

SExp* evlis(SExp* input, SExp* alist) {
	if(input == NULL) {
		cerr << "Error: WHY is this happenning??" << endl;
		return NULL;
	}
	if (input->null()) {
		return SExp::get_symbol(nil);
	} else {
		SExp* first = eval(input->car(), alist);
		SExp* second = evlis(input->cdr(), alist);
		if(first == NULL || second == NULL) {
			cerr << "Error: Couldn't evaluate all the arguments in the list" << endl;
			return NULL;
		}
		SExp* out = new SExp(first, second);
		return out;
	}
}

bool check_evcon(SExp* be_input) {
	// Check if all the elements in be_input are list of size 2
	if(be_input->null()) {
		return true;
	} else if(be_input->car()->len() != 2) {
		// Throw Error
		cerr << "Error: All elements of COND should be lists of size 2" << endl;
		return false;
	} else {
		return check_evcon(be_input->cdr());
	}
}

SExp* evcon(SExp* be_input, SExp* alist) {
	if (be_input->null()) {
		// Throw error
		cerr << "Error: None of the boolean expressions outputted T" << endl;
		return NULL;
	} else {
		// Evaluate the first boolean expression in be_input
		SExp* be = eval(be_input->car()->car(), alist);
		if(be == NULL) {
			cerr << "Error: A boolean expression in COND is not a well formed LISP expression" << endl;
			return NULL;
		}
		if(be->null()) {
			return evcon(be_input->cdr(), alist);
		} else {
			if(be_input->car()->len() == 2) {
				return eval(be_input->car()->cdr()->car(), alist);
			} else {
				cerr << "Error: COND is not well formed" << endl;
				return NULL;
			}
		}
	}
}

SExp* addpairs(SExp* parameters_list, SExp* x, SExp* alist) {
	if(parameters_list->null() && x->null())
		return alist;
	if(parameters_list->null() || x->null()) {
		// Only one of them is null other is not
		// Incorrect number of parameters passed
		// Throw error
		cerr << "Error: Incorrect number of parameters passed to the function" << endl;
		return NULL;
	}
	// Both are non null
	SExp* new_pair = new SExp(parameters_list->car(), x->car());
	SExp* new_alist = new SExp(new_pair, alist);
	return addpairs(parameters_list->cdr(), x->cdr(), new_alist);
}

SExp* apply(SExp* f, SExp* x, SExp* alist) {
	if(x == NULL) {
		// Evlis evaluation failed. Terminate execution
		return NULL;
	}
	if(f->sym_atom()) {
		// Check if primitive function
		string f_name = f->get_name();
		int length_x = x->len();
		if(length_x < 0) {
			// Throw error. X is not a list
			cerr << "Error: List of parameters is not a list" << endl;
			return NULL;
		}
		if(!f_name.compare("CAR")) {
			if(length_x != 1) {
				cerr << "Error: Expected number of parameters to CAR = 1, given " << length_x << endl;
				return NULL;
			}
			if (x->car()->atom()) {
				cerr << "Error: Input given to CAR is an ATOM" << endl;
				return NULL;
			}
			return x->car()->car();
		} else if(!f_name.compare("CDR")) {
			if(length_x != 1) {
				cerr << "Error: Expected number of parameters to CDR = 1, given " << length_x << endl;
				return NULL;
			}
			if (x->car()->atom()) {
				cerr << "Error: Input given to CDR is an ATOM" << endl;
				return NULL;
			}
			return x->car()->cdr();
		} else if(!f_name.compare("CONS")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to CONS = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* out = new SExp(x->car(), x->cdr()->car());
			return out;
		} else if(!f_name.compare("ATOM")) {
			if(length_x != 1) {
				cerr << "Error: Expected number of parameters to ATOM = 1, given " << length_x << endl;
				return NULL;
			}
			if(x->car()->atom()) {
				return SExp::get_symbol(T);
			}
			return SExp::get_symbol(nil);
		} else if(!f_name.compare("INT")) {
			if(length_x != 1) {
				cerr << "Error: Expected number of parameters to INT = 1, given " << length_x << endl;
				return NULL;
			}
			if(x->car()->atom()) {
				if(x->car()->int_atom())
					return x->car();
				else
					return SExp::get_symbol(nil);
			} else {
				// Not an Atom
				cout << x->car()->get_dot_notation() << " is not an atom" << endl;
				return NULL;
			}
		} else if(!f_name.compare("NULL")) {
			if(length_x != 1) {
				cerr << "Error: Expected number of parameters to NULL = 1, given " << length_x << endl;
				return NULL;
			}
			if(x->car()->null()) {
				return SExp::get_symbol(T);
			}
			return SExp::get_symbol(nil);
		} else if(!f_name.compare("EQ")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to EQ = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				if(first->get_value() == second->get_value()) {
					return SExp::get_symbol(T);
				}
				return SExp::get_symbol(nil);
			} else if(first == second) {		// Return T if they are the same pointer
				return SExp::get_symbol(T);
			} else if(!first->atom() || !second->atom()) {
				cerr << "Error: EQ can only handle atomic inputs" << endl;
				return NULL;
			} else {
				return SExp::get_symbol(nil);
			}
		} else if(!f_name.compare("PLUS")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to PLUS = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				SExp* result = new SExp(first->get_value() + second->get_value());
				return result;
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("MINUS")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to MINUS = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				SExp* result = new SExp(first->get_value() - second->get_value());
				return result;
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("TIMES")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to TIMES = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				SExp* result = new SExp(first->get_value() * second->get_value());
				return result;
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("QUOTIENT")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to QUOTIENT = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				SExp* result = new SExp(first->get_value() / second->get_value());
				return result;
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("REMAINDER")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to REMAINDER = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				SExp* result = new SExp(first->get_value() % second->get_value());
				return result;
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("LESS")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to LESS = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				if(first->get_value() < second->get_value()) {
					return SExp::get_symbol(T);
				}
				return SExp::get_symbol(nil);
			} else {
				// Throw Error
				return NULL;
			}
		} else if(!f_name.compare("GREATER")) {
			if(length_x != 2) {
				cerr << "Error: Expected number of parameters to GREATER = 2, given " << length_x << endl;
				return NULL;
			}
			SExp* first = x->car();
			SExp* second = x->cdr()->car();
			if(first->int_atom() && second->int_atom()) {
				if(first->get_value() > second->get_value()) {
					return SExp::get_symbol(T);
				}
				return SExp::get_symbol(nil);
			} else {
				// Throw Error
				return NULL;
			}
		} else {
			SExp* f_pair = f->getval(dlist);
			if(f_pair == NULL) {
				// f is not in the dlist
				cerr << "Error: " << f->get_dot_notation() << " is not present in the dlist" << endl;
				return NULL;
			}
			SExp* f_definition = f_pair->cdr();
			SExp* f_parameter_list = f_pair->car();
			SExp* new_alist = addpairs(f_parameter_list, x, alist);
			if(new_alist == NULL) {
				// Terminate Execution. Incorrect number of parameters
				return NULL;
			}
			return eval(f_definition, new_alist);
		}
	} else {
		// F is not a symbolic atom
		cerr << "Error: " << f->get_dot_notation() << " is not a valid function" << endl;
		// Throw Error
		return NULL;
	}
}

SExp* eval(SExp* input, SExp* alist) {
	SExp* T_exp = SExp::get_symbol(T);
	SExp* NIL = SExp::get_symbol(nil);
	if(input->atom()) {
		if(input->int_atom()) {
			return input;
		} else if(input->eq(T_exp)) {
			return T_exp;
		} else if(input->eq(NIL)) {
			return NIL;
		} else if(input->in(alist)) {
			return input->getval(alist);
		} else {
			// Unbounded Atom
			// Throw Error!!
			cerr << "Error: " << input->get_dot_notation() << " is an Unbounded atom" << endl;
			return NULL;
		}
	} else if(input->car()->sym_atom()) {
		string f_name = input->car()->get_name();
		if(!f_name.compare(QUOTE)) {
			// QUOTE should only have one argument
			if(input->cdr()->cdr()->null()) {
				return input->cdr()->car();
			}
			// Incorrect number of arguments given to QUOTE or there is no NIL atom
			cerr << "Error: QUOTE is not written properly!" << endl;
			return NULL;
		} else if(!f_name.compare(COND)) {
			if(check_evcon(input->cdr()))
				return evcon(input->cdr(), alist);
			return NULL;
		} else if(!f_name.compare(DEFUN)) {
			// Add the function definition to dlist
			SExp* exp = input->cdr();
			if(exp == NULL) {
				cerr << "Error: DEFUN is not written properly" << endl;
				return NULL;
			}
			SExp* f = exp->car();
			if(f == NULL) {
				cerr << "Error: DEFUN is not written properly" << endl;
				return NULL;
			}
			SExp* parameters_list = exp->cdr()->car();
			SExp* fb = exp->cdr()->cdr()->car();
			if (parameters_list == NULL || fb == NULL) {
				cerr << "Error: DEFUN is not written properly" << endl;
				return NULL;
			}
			SExp* parameters_and_fb = new SExp(parameters_list, fb);
			SExp* pair = new SExp(f, parameters_and_fb);
			dlist = new SExp(pair, dlist);
			cout << f->get_name() << " added to the dList" << endl;
			return pair;
		} else {
			return apply(input->car(), evlis(input->cdr(), alist), alist);
		}
	} else {
		// NOT a valid LISP expression
		// Throw Error!!
		cerr << "Error: " << input->car()->get_dot_notation() << " is not a special type or function name" << endl;
		return NULL;
	}
}
