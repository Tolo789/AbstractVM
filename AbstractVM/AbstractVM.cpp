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

	this->runVM();

	return ;
}

AbstractVM::AbstractVM(AbstractVM const & src) {
	// std::cout << "AbstractVM copy constructor called" << std::endl;
	*this = src;

	this->runVM();

	return ;
}

AbstractVM::~AbstractVM(void) {
	// std::cout << "AbstractVM destructor called" << std::endl;
	this->options.clear();
	this->program.clear();

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
	char okOptions[] = {'h', 'd'};

	int i = 0;
	while (++i < argc) {
		if (argv[i][0] == '-') {
			if (std::find(okOptions, okOptions + 2, argv[i][1]) != okOptions + 2) {
				if (argv[i][2] != 0) {
					throw AbstractVM::UnkownOptionException();
				}
				if (std::find(this->options.begin(), this->options.end(), argv[i][1]) == this->options.end())
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
	std::size_t index;
	std::size_t index_comment;
	for (std::list<std::string>::iterator it = this->program.begin(); it != this->program.end(); ++it) {
		if (*it == "")
			continue ;

		// trim spaces before arguments
		index = it->find(" ", 0);
		while (index == 0) {
			it->replace(index, 1, "");
			if (*it == "")
				break ;
			index = it->find(" ", 0);
		}

		// trim spaces between arguments
		if (*it == "")
			continue ;
		index = it->find("  ", 0);
		index_comment = it->find(";", 0);
		while (index != std::string::npos && index < index_comment) {
			it->replace(index, 2, " ");
			index = it->find("  ", index);
			index_comment = it->find(";", 0);
		}

		// trim spaces after arguments
		index = it->find_last_of(" ");
		index_comment = it->find(";", 0);
		if (index == it->size() - 1 && index_comment != std::string::npos)
			it->replace(index, 1, "");
	}

	return ;
}

void	AbstractVM::runVM(void) {
	this->trimProgram();

	if (find(options.begin(), options.end(), 'h') != options.end()) {
		std::cout << "AbstractVM Usage:" << std::endl;
		std::cout << "\t$> ./avm [-option1 -option2 .. -optionN] [filepath]" << std::endl << std::endl;

		std::cout << "Options:" << std::endl;
		std::cout << "\t-h  ->  displays this help menu and exit" << std::endl;
		std::cout << "\t-d  ->  debug mode while executing the program" << std::endl;
	}
	else {
		// TODO: lexer
		this->lexer.execute(this->program, this->options);

		// TODO: parser
		this->parser.execute(this->program, this->options);
	}

	// TODO: delete this line
	std::cout << std::endl << *this << std::endl;

	return ;
}

std::string const AbstractVM::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "AbstractVM:{";

	debugStr << "Options:{";
	for (std::list<char>::const_iterator it = this->options.begin(); it != this->options.end(); ++it) {
		if (it != this->options.begin())
			debugStr << ", ";
		debugStr << '-' << *it;
	}
	debugStr << "}";

	debugStr << ", Program:{";
	for (std::list<std::string>::const_iterator it = this->program.begin(); it != this->program.end(); ++it) {
		if (it != this->program.begin())
			debugStr << " \\n ";
		debugStr << *it;
	}
	debugStr << "}";

	debugStr << "}";

	return debugStr.str();
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
