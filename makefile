front_end: lisp_front_end.cpp lisp_back_end.o s_tokenizer.o SExp.o
	g++ -g s_tokenizer.o SExp.o lisp_back_end.o lisp_front_end.cpp -o front_end.out
lisp_back_end.o: lisp_back_end.cpp lisp_back_end.h
	g++ -c lisp_back_end.cpp
s_tokenizer.o: s_tokenizer.cpp s_tokenizer.h
	g++ -c s_tokenizer.cpp 
SExp.o: SExp.cpp SExp.h
	g++ -c SExp.cpp
clean:
	rm s_tokenizer.o SExp.o front_end.out lisp_back_end.o