#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "AbstractVM.hpp"

// === CONSTRUCTOR =============================================================

AbstractVM::AbstractVM(void) {
	// std::cout << "AbstractVM default constructor called" << std::endl;

	return ;
}

AbstractVM::AbstractVM(int argc, char **argv) {
	// std::cout << "AbstractVM standard constructor called" << std::endl;

	int filepathIndex;

	filepathIndex = this->parseOptions(argc, argv);
	if (filepathIndex == 0) {
		this->getProgramFromStdInput();
	}
	else {
		this->getProgramFromFile(argv[filepathIndex]);
	}
	this->trimProgram();

	// TODO: lexer
	std::list<std::string> errorList;
	errorList = this->lexerCheck();

	return ;
}

AbstractVM::AbstractVM(AbstractVM const & src) {
	// std::cout << "AbstractVM copy constructor called" << std::endl;
	*this = src;

	return ;
}

AbstractVM::~AbstractVM(void) {
	// std::cout << "AbstractVM destructor called" << std::endl;

	return ;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

AbstractVM& AbstractVM::operator=(AbstractVM const & rhs) {
	// std::cout << "AbstractVM assignation operator called" << std::endl;
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

void AbstractVM::getProgramFromStdInput(void) {
	std::string line = "";
	while (std::getline(std::cin, line)) {
		if (line == ";;")
			break;
		this->program.push_back(line);
	}

	return ;
}

void AbstractVM::getProgramFromFile(char *filePath) {
	std::string line;
	std::ifstream input_file (filePath);
	if (input_file.is_open()) {
		while (std::getline(input_file, line))
		{
			this->program.push_back(line);
		}
	}
	else
		throw AbstractVM::FileNameException();

	return ;
}

void	AbstractVM::trimProgram(void) {
	size_t index;
	for (std::list<std::string>::iterator it = this->program.begin(); it != this->program.end(); ++it) {
		// trim spaces before arguments
		index = it->find(" ", 0);
		while (index == 0) {
			it->replace(index, 1, "");
			index = it->find("  ", index);
		}

		// trim spaces between arguments
		index = it->find("  ", 0);
		while (index != std::string::npos) {
			it->replace(index, 2, " ");
			index = it->find("  ", index);
		}

		// trim spaces after arguments
		index = it->find_last_of(" ");
		if (index == it->size() - 1)
			it->replace(index, 1, "");
	}

	return ;
}

std::list<std::string>	AbstractVM::lexerCheck(void) {
	std::list<std::string> errorList;

	std::string first;
	std::string second;
	std::string third;
	std::size_t index_old;
	std::size_t index;
	for (std::list<std::string>::iterator it = this->program.begin(); it != this->program.end(); ++it) {
		index_old = 0;
		first = "";
		second = "";
		third = "";

		index = it->find(' ', index_old);
		first = it->substr(index_old, index);

		if (index != std::string::npos) {

			if (index != std::string::npos) {
				index_old = index;
				index = it->find(' ', index_old);
				second = it->substr(index_old, index);

				if (index != std::string::npos) {
					index_old = index;
					index = it->find(' ', index_old);
					third = it->substr(index_old, index);
				}
			}
		}

		// std::cout << "First: " << first << ", Second: " << second << ", Third: " << third << std::endl;
	}

	return errorList;
}

std::string const AbstractVM::serialize(void) const {
	std::stringstream debug_str;
	debug_str << "AbstractVM:{";

	debug_str << "Options:{";
	for (std::list<char>::const_iterator it = this->options.begin(); it != this->options.end(); ++it) {
		if (it != this->options.begin())
			debug_str << ", ";
		debug_str << '-' << *it;
	}
	debug_str << "}";

	debug_str << ", Program:{";
	for (std::list<std::string>::const_iterator it = this->program.begin(); it != this->program.end(); ++it) {
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

const char *AbstractVM::FileNameException::what() const throw() {
		return "Cannot open the file at the given path..!";
}

// === END EXCEPTIONS ==========================================================
