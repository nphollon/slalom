runtests : lambda_test.cpp node.cpp parse.cpp strutil.cpp tester.cpp
	clang++ -o runtests parse.cpp lambda_test.cpp node.cpp strutil.cpp tester.cpp
	./runtests

clean :
	rm runtests
