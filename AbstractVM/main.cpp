#include <iostream>
#include "AbstractVM.hpp"

int main(int argc, char **argv) {

	try {
		AbstractVM avm = AbstractVM(argc, argv);
	}
	catch (std::exception & e)
	{
		std::cout << e.what() ;
		exit (EXIT_FAILURE);
	}

	return 0;
}
