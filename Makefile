runtests : lambda_test.cpp node.cpp parse.cpp strutil.cpp tester.cpp
	clang++ -o runtests lambda_test.cpp node.cpp parse.cpp strutil.cpp tester.cpp
	./runtests

clean :
	rm runtests
