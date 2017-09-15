#include <iostream>
#include <sstream>
#include "AProgramReader.hpp"

// === CONSTRUCTOR =============================================================

AProgramReader::AProgramReader(void) {
	// std::cout << "AProgramReader default constructor called" << std::endl;

	return ;
}

AProgramReader::~AProgramReader(void) {
	// std::cout << "AProgramReader destructor called" << std::endl;

	return ;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================

std::ostream & operator<<(std::ostream & o, AProgramReader const & rhs) {
	o << rhs.serialize();

	return o;
}

// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================
// === END GET / SET ===========================================================

// === OTHERS ==================================================================

std::list<std::string>	AProgramReader::getLineElements(std::string line) {
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

std::string const AProgramReader::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "AProgramReader:{}";

	return debugStr.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================

std::map<std::string,int> const AProgramReader::create_map() {
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

std::map<std::string, int> const AProgramReader::INSTR = AProgramReader::create_map();

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================
// === END EXCEPTIONS ==========================================================
