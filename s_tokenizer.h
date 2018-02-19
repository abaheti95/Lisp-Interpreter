#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <stack>
#include <cstdio>
#include <cstdlib>
#include <cctype>

using namespace std;

/* Given an input string we want to tokenize the string at various locations and return the tokenized list of strings
	We want to ouput tokens for following format
	1 - Opening bracket '('
	2 - Closing bracket ')'
	3 - dot character '.'
	4 - whitespace character - if multiple whitespace exists then we should truncate it into 1 ' '
	5 - numerical entity "(\\+|-)?[[:digit:]]+"
	6 - string
*/

const int O_BRACKET = 1;		// Opening bracket
const int C_BRACKET = 2;		// Closing bracket
const int DOT = 3;				// Dot symbol
const int WHITESPACE = 4;		// Whitespace
const int ATOM = 5;				// Atom: Anythin which is not the above 4

vector<string> s_tokenize(string& input);
int token_type(string token);	// Returns the type of token
vector<string> filter_whitespace_tokens(vector<string>& tokens);
// Given tokens for every opening bracket find the position of its corresponding closing bracket
// and similartly for every closing bracket find the position of its corresponding opening bracket
vector<int> compute_bracket_locations(vector<string>& tokens);