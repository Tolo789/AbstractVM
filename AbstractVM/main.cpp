#include <iostream>

#include "AbstractVM.hpp"

int main(int argc, char **argv) {

	try {
		AbstractVM avm = AbstractVM(argc, argv);

		std::cout << avm << std::endl;

		// TODO: parser (exec)
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl ;
		exit (EXIT_FAILURE);
	}

	return 0;
}
