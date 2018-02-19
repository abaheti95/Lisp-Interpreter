front_end: lisp_front_end.cpp s_tokenizer.o SExp.o
	g++ -g s_tokenizer.o SExp.o lisp_front_end.cpp -o front_end.out
s_tokenizer.o: s_tokenizer.cpp s_tokenizer.h
	g++ -c s_tokenizer.cpp 
SExp.o: SExp.cpp SExp.h
	g++ -c SExp.cpp
clean:
	rm s_tokenizer.o front_end.out