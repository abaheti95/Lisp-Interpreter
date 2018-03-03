# Lisp-Interpreter
Very basic version of Lisp Interpreter designed as a part of homework of CSE 6341: Programming Langauges

## Part 1
Implementation of the front-end of the Lisp Interpreter which deals with an input handler and an output handler. The Input handler should be able to identify multiline S-expressions which can be a mixture of dot `.` and list notation. Part 1 has to read multiple s-expression with mixed notation and output the dot-notation of the input. 
Following is the procedure of input routine
1. First the input is tokenized into many tokens where contiguous whitespaces are truncated into one whitespace
2. These tokens are then reduces to minimum necessary tokens by removing extra whitespace tokens
3. These tokens are used to generate the SExp structure using a recursive procedure
4. Finally the generated SExp structure is used to create the dot notation(as string) and outputted on the standard output.
At any point in the above 4 steps if there is some error then the code reports it. I have tested the code with many incorrect inputs. The code tries to give the reason for error as much as possible.

Testing command: 
1. `make`
2. `front_end.out < incorrect_inputs.txt`

### TODO: Part 1
- Free the memory of SExp using delete functions