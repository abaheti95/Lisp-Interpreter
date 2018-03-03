#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include "s_tokenizer.h"
#include "SExp.h"

using namespace std;

/* This is the main front-end program which will read the input, send it to tokenizer, create the tree 
	from the tokenized input and finally print the output dot notation.
*/
const int INT_ATOM = 1;
const int S_ATOM = 2;

int get_atom_type(string token) {
	int token_length = token.length();
	bool flag = false;
	for(int i = 0; i < token_length; i++)
		flag |= isalpha(token[i]);
	if(flag)
		return S_ATOM;
	else
		return INT_ATOM;
}

void print_tokens(vector<string> &tokens) {
	for(int i = 0; i < tokens.size(); i++) {
		cout << tokens[i] << ",";
	}
	cout << endl;
}

int stoi(string& s) {
	int i;
	istringstream(s) >> i;
	return i;
}

SExp* generate_binary_tree_from_tokens(vector<string> &tokens, vector<int> positions, 
	int start_pos, int end_pos) {
	// returns a pointer to the SExp* which represents the binary tree of the input tokens
	string start_token = tokens[start_pos];
	int start_token_type = token_type(start_token);
	string end_token = tokens[end_pos];
	int end_token_type = token_type(end_token);
	// if start and end position are the same and the token is an atom then return that atom
	if(start_pos == end_pos && start_token_type == ATOM) {
		int atom_type = get_atom_type(start_token);
		SExp* atom = (atom_type == INT_ATOM)? new SExp(stoi(start_token)): SExp::get_symbol(start_token);
		return atom;
	} else if(start_token_type == O_BRACKET && end_token_type == C_BRACKET && (end_pos - start_pos) == 1) {
		// This is the NIL atom
		string nil = "NIL";
		SExp* atom = SExp::get_symbol(nil);
		return atom;
	} else if(start_token_type == O_BRACKET && end_token_type == C_BRACKET) {
		// This can be either a dot notation or a list notation
		// We need to find the position of the dot or whitespace
		int pos_dot_or_space = -1;
		// if first element is a bracketed s-expression
		if(positions[start_pos+1] != -1) {
			pos_dot_or_space = positions[start_pos+1] + 1;
		} else
			// else if first element is an atom
			pos_dot_or_space = start_pos + 2;
		int dot_or_space_token_type = token_type(tokens[pos_dot_or_space]);
		if(dot_or_space_token_type == DOT) {
			SExp* non_atom;
			try {
				non_atom = new SExp(
								generate_binary_tree_from_tokens(tokens, positions, start_pos+1, pos_dot_or_space-1), 
								generate_binary_tree_from_tokens(tokens, positions, pos_dot_or_space+1, end_pos-1)
								);
			} catch(std::invalid_argument& e) {
				return NULL;
			}
			return non_atom;
		} else if(dot_or_space_token_type == WHITESPACE) {
			//Compute the expression in for loop
			// For each entry check if it is a bracket expression or an atom and generate list according to that
			int temp_start_pos = start_pos+1;
			int temp_end_pos = end_pos;
			int next_space_pos = pos_dot_or_space;
			vector<SExp*> list;
			while(temp_start_pos < end_pos) {
				// Check if the current element is a bracket expression or not
				list.push_back(generate_binary_tree_from_tokens(tokens, positions, temp_start_pos, next_space_pos-1));
				// Update next position if any
				temp_start_pos = next_space_pos + 1;
				if(positions[temp_start_pos] != -1 && temp_start_pos <= end_pos && token_type(tokens[temp_start_pos]) == O_BRACKET)
					next_space_pos = positions[temp_start_pos] + 1;
				else
					next_space_pos = temp_start_pos + 1;
				// cout << "DEBUGGING LIST" << endl;
				// cout << temp_start_pos << " " << tokens[temp_start_pos] << endl;
				// cout << next_space_pos << " " << tokens[next_space_pos] << endl;
			}
			// Vector to list starting from the end
			SExp* current_element = NULL;
			for(int i = list.size() - 1; i >= 0; i--) {
				if(i == (list.size() - 1)) {
					string nil = "NIL";
					SExp* nil_atom = SExp::get_symbol(nil);
					current_element = new SExp(list[i], nil_atom);
				} else {
					current_element = new SExp(list[i], current_element);
				}
			}
			return current_element;
		} else if(pos_dot_or_space == end_pos) {
			// List expression with only 1 element so call and save it with nil
			int temp_start_pos = start_pos + 1;
			int temp_end_pos = -1;
			if(positions[temp_start_pos] != -1 && token_type(tokens[temp_start_pos]) == O_BRACKET)
				temp_end_pos = positions[temp_start_pos];
			else
				temp_end_pos = temp_start_pos;
			string nil = "NIL";
			SExp* nil_atom = SExp::get_symbol(nil);
			SExp* non_atom;
			try {
				non_atom = new SExp(
						generate_binary_tree_from_tokens(tokens, positions, temp_start_pos, temp_end_pos),
						nil_atom);
			} catch(std::invalid_argument& e) {
				return NULL;
			}
			return non_atom;
		} else {
			// Print the unexpected result
			if(start_pos == end_pos)
				cout << "ERROR!!! unexpected token " << tokens[start_pos] << endl;
			else
				cout << "ERROR!!! unexpected tokens " << tokens[start_pos] << " and " << tokens[end_pos] << endl;
			print_tokens(tokens);
			cout << start_pos << " " << tokens[start_pos] << endl;
			cout << end_pos << " " << tokens[end_pos] << endl;
			cout << pos_dot_or_space << " " << tokens[pos_dot_or_space] << ";" << endl;
			return NULL;
		}
	} else {
		// Print the unexpected result
		if(start_pos == end_pos)
			cout << "ERROR!! unexpected token " << tokens[start_pos] << endl;
		else
			cout << "ERROR!! unexpected tokens " << tokens[start_pos] << " and " << tokens[end_pos] << endl;
		// print_tokens(tokens);
		// cout << start_pos << " " << tokens[start_pos] << endl;
		// cout << end_pos << " " << tokens[end_pos] << endl;
		return NULL;
	}
}


int main() {
	string raw_s_expression = "";
	bool flag = true;
	while(flag) {
		string line="";
		getline(cin, line);
		if(!line.empty() && *line.rbegin() == '\r') {
			line.erase( line.length()-1, 1);
		}
		if(!line.compare("$") || !line.compare("$$")) {
			// Last s-expression. End the loop after processing this
			if(!line.compare("$$"))
				flag = false;
			// we have the complete raw_s_expression.
			// Send it for tokenization
			cout << "raw:" << raw_s_expression << endl;
			vector<string> tokens = s_tokenize(raw_s_expression);
			if(tokens.empty()) {
				raw_s_expression = "";
				continue;
			}
			// print_tokens(tokens);
			vector<string> filtered_tokens = filter_whitespace_tokens(tokens);
			print_tokens(filtered_tokens);
			vector<int> positions = compute_bracket_locations(filtered_tokens);
			if(positions.empty()) {
				raw_s_expression = "";
				continue;
			}
			SExp* exp = generate_binary_tree_from_tokens(filtered_tokens, positions, 0, filtered_tokens.size()-1);
			if(exp == NULL) {
				raw_s_expression = "";
				continue;
			}
			// cout << "Trying to generate dot notation for : " << raw_s_expression << endl;
			string exp_dot_notation = exp->get_dot_notation();
			cout << "Dot Notation: " << exp_dot_notation << endl;
			raw_s_expression = "";
		} else {
			raw_s_expression += line;
		}
	}
}