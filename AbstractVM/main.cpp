#include <iostream>

#include "AbstractVM.hpp"

int main(int argc, char **argv) {

	try {
		AbstractVM avm = AbstractVM(argc, argv);

		std::cout << avm << std::endl;
		// TODO: parser (exec)

		// TODO: create a Lexer class and a Parser class which both inherite from the same abstract class
		// Every VM will have a Lexer and a Parser attribute, the Lexer is run by construct while the Parser is run by user
		// Call this functions runLexer() and runParser(), which will use specific functions of both classes
	}
	catch (std::exception & e)
	{
		std::cout << e.what() ;
		exit (EXIT_FAILURE);
	}

	return 0;
}
