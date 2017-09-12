#include <iostream>
#include "AbstractVM.hpp"

int main(int argc, char **argv) {

	try {
		AbstractVM avm = AbstractVM(argc, argv);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() << std::endl ;
		exit (EXIT_FAILURE);
	}

	// TODO: parser (exec)

	return 0;
}
