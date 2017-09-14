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
	if (errorList.size() != 0)
		throw AbstractVM::LexerException(errorList);

	return ;
}

AbstractVM::AbstractVM(AbstractVM const & src) {
	// std::cout << "AbstractVM copy constructor called" << std::endl;
	*this = src;

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

std::list<std::string>	AbstractVM::lexerCheck(void) {
	std::list<std::string>	errorList;
	std::list<std::string>	elements;

	std::list<std::string>::const_iterator		lineIterator;
	std::list<std::string>::const_iterator		elemIterator;
	std::map<std::string,int>::const_iterator	mapResult;

	std::size_t lineCount = 0;
	std::size_t elemCount = 0;

	bool param_needed;

	for (lineIterator = this->program.begin(); lineIterator != this->program.end(); ++lineIterator) {
		lineCount++;
		if (*lineIterator == "" or lineIterator->substr(0, 1) == ";")
			continue ;

		elements = this->getLineElements(*lineIterator);
		elemCount = 0;
		for (elemIterator = elements.begin(); elemIterator != elements.end(); ++elemIterator) {
			elemCount++;

			if (elemCount == 1) {
				param_needed = false;
				mapResult = AbstractVM::INSTR.find(*elemIterator);
				if (mapResult == AbstractVM::INSTR.end()) {
					std::stringstream errorStr;
					errorStr << "ERROR - Line " << lineCount << " - Unknown instruction ('" << *elemIterator << "' given)";
					errorList.push_back(errorStr.str());

					break ;
				}
				else if (mapResult->second == 1)
					param_needed = true;
			}
			else if (elemCount == 2) {
				if (param_needed == true) {
					if (*elemIterator == "" or elemIterator->substr(0, 1) == ";") {
						std::stringstream errorStr;
						errorStr << "ERROR - Line " << lineCount << " - Instruction needs a parameter";
						errorList.push_back(errorStr.str());

						break ;
					}
					// TODO: check that the given param is good
				}
				else {
					if (*elemIterator != "" and elemIterator->substr(0, 1) != ";") {
						std::stringstream errorStr;
						errorStr << "ERROR - Line " << lineCount << " - Instruction doesnt need any parameter ('" << *elemIterator << "' given)";
						errorList.push_back(errorStr.str());

						break ;
					}
				}
			}
			else if (*elemIterator != "" and elemIterator->substr(0, 1) != ";") {
				std::stringstream errorStr;
				errorStr << "ERROR - Line " << lineCount << " - Instruction doesnt need any other parameter ('" << *elemIterator << "' given)";
				errorList.push_back(errorStr.str());
			}
		}
	}

	return errorList;
}

std::list<std::string>	AbstractVM::getLineElements(std::string line) {
	std::list<std::string> elements;

	std::string first = "";
	std::string second = "";
	std::string third = "";

	std::size_t index;
	std::size_t indexOld = 0;
	std::size_t indexComment;

	if (line != "") {
		index = line.find(' ', indexOld);
		indexComment = line.find(";", 0);

		if (index != std::string::npos && index < indexComment) {
			first = line.substr(indexOld, index - indexOld);

			indexOld = index + 1;
			index = line.find(' ', indexOld);

			if (index != std::string::npos && index < indexComment) {
				second = line.substr(indexOld, index - indexOld);

				indexOld = index + 1;
				third = line.substr(indexOld, line.size() - indexOld);
			}
			else {
				if (indexComment != std::string::npos && indexComment != indexOld) {
					second = line.substr(indexOld, indexComment - indexOld);
					third = line.substr(indexComment, line.size() - indexComment);
				}
				else {
					second = line.substr(indexOld, line.size() - indexOld);
				}
			}
		}
		else {
			if (indexComment != std::string::npos && indexComment != indexOld) {
				first = line.substr(indexOld, indexComment - indexOld);
				second = line.substr(indexComment, line.size() - indexComment);
			}
			else {
				first = line.substr(indexOld, line.size() - indexOld);
			}
		}
	}

	elements.push_back(first);
	elements.push_back(second);
	elements.push_back(third);

	return elements;
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

std::map<std::string,int> const AbstractVM::create_map() {
	std::map<std::string,int> m;

	m["push"] = 1;
	m["pop"] = 0;
	m["dump"] = 0;
	m["assert"] = 1;
	m["add"] = 0;
	m["sub"] = 0;
	m["mul"] = 0;
	m["div"] = 0;
	m["mod"] = 0;
	m["print"] = 0;
	m["exit"] = 0;

	return m;
}

std::map<std::string, int> const AbstractVM::INSTR = AbstractVM::create_map();

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

const char *AbstractVM::UnkownOptionException::what() const throw() {
		return "Unknown option given..!\n";
}

const char *AbstractVM::FilepathNumberException::what() const throw() {
		return "More than one filepath given..!\n";
}

const char *AbstractVM::FileNameException::what() const throw() {
		return "Cannot open the file at the given path..!\n";
}

AbstractVM::LexerException::LexerException(std::list<std::string> errorList) {
	for (std::list<std::string>::const_iterator it = errorList.begin(); it != errorList.end(); ++it) {
		if (*it != "")
			this->errorMsg += *it + "\n";
	}
}
AbstractVM::LexerException::~LexerException()  _NOEXCEPT{
}
const char *AbstractVM::LexerException::what() const throw() {
		return errorMsg.c_str();
}

// === END EXCEPTIONS ==========================================================
