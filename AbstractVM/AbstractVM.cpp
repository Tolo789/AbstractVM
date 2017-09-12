#include <iostream>
#include <sstream>
#include "AbstractVM.hpp"

// === CONSTRUCTOR =============================================================

AbstractVM::AbstractVM(void) {
	std::cout << "AbstractVM default constructor called" << std::endl;

	return ;
}

AbstractVM::AbstractVM(int argc, char **argv) {
	// TODO
	std::cout << "AbstractVM standard constructor called" << std::endl;

	// TODO: may switch to list/forward_list ?
	this->options = NULL;
	this->optionsSize = 0;
	this->program = NULL;
	this->programLines = 0;

	// TODO: get options
	int filepathIndex = this->parseOptions(argc, argv);
	// TODO: get program
	if (filepathIndex == 0) {
		// TODO: get program from standar input
	}
	else {
		// TODO: get program from file (argv[filepathIndex])
	}

	// TODO: lexer

	return ;
}

AbstractVM::AbstractVM(AbstractVM const & src) {
	std::cout << "AbstractVM copy constructor called" << std::endl;
	*this = src;

	return ;
}

AbstractVM::~AbstractVM(void) {
	std::cout << "AbstractVM destructor called" << std::endl;

	return ;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

AbstractVM& AbstractVM::operator=(AbstractVM const & rhs) {
	std::cout << "AbstractVM assignation operator called" << std::endl;
	// TODO
	this->options = rhs.getOptions();
	this->program = rhs.getProgram();

	return *this;
}

std::ostream & operator<<(std::ostream & o, AbstractVM const & rhs) {
	// TODO
	o << rhs.serialize();

	return o;
}

// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================

	char *AbstractVM::getOptions() const {
		return this->options;
	}

	char *AbstractVM::getProgram() const {
		return this->program;
	}

// === END GET / SET ===========================================================

// === OTHERS ==================================================================

int AbstractVM::parseOptions(int argc, char **argv) {
	int filepathIndex = 0;

	int i = 0;
	while (++i < argc) {
		if (argv[i][0] == '.') {
			if (filepathIndex == 0) {
				filepathIndex = i;
			}
			else {
				throw AbstractVM::FilepathNumberException();
			}
		}
		else if (argv[i][0] == '-') {
			// TODO: check if valid option
		}
		else {
			throw AbstractVM::UnknownArgumentException();
		}
	}
	return filepathIndex;
}

std::string const AbstractVM::serialize(void) const {
	// TODO
	std::stringstream debug_str;
	debug_str << "AbstractVM{}";

	return debug_str.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================
// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

const char *AbstractVM::FilepathNumberException::what() const throw() {
		return "More than one filepath given..!";
}

const char *AbstractVM::UnknownArgumentException::what() const throw() {
		return "Unknown argument given..!";
}

// === END EXCEPTIONS ==========================================================
