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
	this->options = rhs.getOptions();
	this->program = rhs.getProgram();

	return *this;
}

std::ostream & operator<<(std::ostream & o, AbstractVM const & rhs) {
	o << rhs.serialize();

	return o;
}

// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================

std::list<char>			AbstractVM::getOptions() const {
	return this->options;
}

std::list<std::string>	AbstractVM::getProgram() const {
	return this->program;
}

// === END GET / SET ===========================================================

// === OTHERS ==================================================================

int AbstractVM::parseOptions(int argc, char **argv) {
	int filepathIndex = 0;

	int i = 0;
	while (++i < argc) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'd') {
				if (argv[i][2] != 0) {
					throw AbstractVM::UnkownOptionException();
				}
				this->options.push_front(argv[i][1]);
			}
			else {
				throw AbstractVM::UnkownOptionException();
			}
		}
		else {
			if (filepathIndex == 0) {
				filepathIndex = i;
			}
			else {
				throw AbstractVM::FilepathNumberException();
			}
		}
	}

	return filepathIndex;
}

std::string const AbstractVM::serialize(void) const {
	std::stringstream debug_str;
	debug_str << "AbstractVM{";

	debug_str << "Options:{";
	for (std::list<char>::iterator it = this->options.begin(); it != this->options.end(); ++it) {
		if (it != this->options.begin())
			debug_str << ", ";
		debug_str << '-' << *it;
	}
	debug_str << "}";

	debug_str << ", Program:{";
	for (std::list<std::string>::iterator it = this->program.begin(); it != this->program.end(); ++it) {
		if (it != this->program.begin())
			debug_str << " \\n ";
		debug_str << *it;
	}
	debug_str << "}";

	debug_str << "}";

	return debug_str.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================
// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

const char *AbstractVM::UnkownOptionException::what() const throw() {
		return "Unknown option given..!";
}

const char *AbstractVM::FilepathNumberException::what() const throw() {
		return "More than one filepath given..!";
}

// === END EXCEPTIONS ==========================================================
