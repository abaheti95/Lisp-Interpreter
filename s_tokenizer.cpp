#include "s_tokenizer.h"

vector<string> s_tokenize(string& input) {
	// cout << "Tokenizing...." << endl;
	int input_length = input.length();
	vector<string> tokens;
	for(int i = 0; i < input_length; i++) {
		char c = input[i];
		string token;
		if(c == '(') {
			token = "(";
		} else if(c == ')') {
			token = ")";
		} else if(c == '.') {
			token = ".";
		} else if(isspace(c)) {
			// In an internal loop compute the series of whitespaces and increment i
			while(isspace(c)) {
				i++;
				c = input[i];
			}
			i--;
			token = " ";
		} else if(c == '+' || c == '-' || isdigit(c)) {
			// In an internal loop compute the number
			int sign = 0;
			if(c == '-') {
				token = "-";
				sign = -1;
			} else if(c == '+') {
				token = "+";
				sign = 1;
			}
			if(sign != 0) {
				i++;
			}
			c = input[i];
			if(!isdigit(c)) {
				cerr << "Invalid character in the input string: " << c << ":" << int(c) << endl;
				return vector<string>();
			}
			int number = 0;
			bool i_incremented_flag = false;
			while(isdigit(c)) {
				token += c;
				i++;
				i_incremented_flag = true;
				c = input[i];
			}
			// We need to check that the next character is either space, bracket or . or end of sentence
			if(c != '(' && c != ')' && c != '.' && !isspace(c) && c != 0) {
				cerr << "Invalid character in the input string: " << c << ":" << int(c) << endl;
				return vector<string>();
			}
			if(i_incremented_flag)
				i--;
		} else if(isalpha(c)) {
			// In an internal loop find the symbolic entity
			token = "";
			while(isalnum(c)) {
				token += c;
				i++;
				c = input[i];
			}
			// We need to check that the next character is either space, bracket or .
			if(c != '(' && c != ')' && c != '.' && !isspace(c) && c != 0) {
				cerr << "Invalid character in the input string: " << c << endl;
				return vector<string>();
			}
			i--;
		} else {
			cerr << "Invalid character in the input string: " << c << endl;
			return vector<string>();
		}
		tokens.push_back(token);
	}
	return tokens;
}

int token_type(string token) {
	if(!token.compare("("))
		return O_BRACKET;
	if(!token.compare(")"))
		return C_BRACKET;
	if(!token.compare("."))
		return DOT;
	if(!token.compare(" "))
		return WHITESPACE;
	// Check if integer
	// bool is_int = 
	// for(int i = 0; i < 0; i++) {
	// 	char c = token[i];
	// 	if(!isdigit(c)) {
	// 		if(i==0 && )
	// 	}
	// }
	return ATOM;
}

vector<string> filter_whitespace_tokens(vector<string>& tokens) {
	vector<string> filtered_tokens;
	// We want to filter whitespace tokens at certain places
	/* Following are the types of whitespaces which can be ignored
		1 - "(", " " - whitespace after opening bracket
		2 - " ", ")" - whitespace before closing bracket
		3 - ".", " " - whitespace after .
		4 - " ", "." - whitespace before .
		5 - if whitespace is the first or last token in the list
	*/
	int prev_token_type = 0;
	for(int i = 0; i < tokens.size(); i++) {
		string token = tokens[i];
		int current_token_type = token_type(token);
		// cout << i << " " << token << " " << current_token_type << " " << prev_token_type << endl;
		if(current_token_type == WHITESPACE) {
			// Condition 5
			if(i==0 || i == (tokens.size()-1)) {
				continue;
			}
			// Condition 1
			if(prev_token_type == O_BRACKET) {
				continue;
			// Condition 3
			} else if(prev_token_type == DOT) {
				continue;
			} else if (i != (tokens.size()-1)) {
				int next_token_type = token_type(tokens[i+1]);
				// Condition 2
				if(next_token_type == C_BRACKET) {
					continue;
				// Condition 4
				} else if(next_token_type == DOT) {
					continue;
				}
			}
		}
		prev_token_type = current_token_type;
		filtered_tokens.push_back(token);
	}
	return filtered_tokens;
}

vector<int> compute_bracket_locations(vector<string>& tokens) {
	// We will store the opening bracket positions in the stack and 
	// whenever we encounter a closing bracket we will pair the positions and save both of them
	stack<int> bracket_positions;
	vector<int> positions(tokens.size(), -1);
	for(int i = 0; i < tokens.size(); i++) {
		int current_token_type = token_type(tokens[i]);
		if(current_token_type == O_BRACKET)
			bracket_positions.push(i);
		else if(current_token_type == C_BRACKET) {
			if(bracket_positions.empty()) {
				cout << "Error!! Extra closing bracket" << endl;
				return vector<int>();
			}
			int o_bracket_pos = bracket_positions.top();
			bracket_positions.pop();
			positions[i] = o_bracket_pos;
			positions[o_bracket_pos] = i;
		}
	}
	if(!bracket_positions.empty()) {
		// Still some opening bracket left in the tokens
		cout << "Error!! Extra opening bracket" << endl;
		return vector<int>();
	}
	return positions;
}
