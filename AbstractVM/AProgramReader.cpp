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

eOperandType AProgramReader::getOperandType(std::string opStr) {
	bool 	opFound = false;
	std::map<std::string, eOperandType>::const_iterator it;
	std::string compareStr;

	for (it = AProgramReader::OPERAND.begin(); it != AProgramReader::OPERAND.end(); it++) {
		compareStr = opStr.substr(0, it->first.size());
		if (compareStr.compare(it->first) == 0) {
			opFound = true;
			break ;
		}
	}
	if (!opFound)
		throw AProgramReader::UnkownOperandException();

	return it->second;
}

std::string AProgramReader::getOperandValue(std::string opStr, eOperandType opType) {
	std::string value = "";
	std::size_t index;

	// cut first part
	index = opStr.find("(");
	opStr = opStr.substr(index + 1, opStr.size() - index);

	// cut end part
	index = opStr.find(")");
	if (index != opStr.size() - 1 || index == 0)
		throw AProgramReader::ParenthesisException();
	value = opStr.substr(0, index);

	// checks on value
	index = value.find(".");
	if (opType == Float || opType == Double) {
		if (index == std::string::npos || index != value.find_last_of("."))
			throw AProgramReader::OperandValueException();
	}
	else if (index != std::string::npos) {
		throw AProgramReader::OperandValueException();
	}

    std::string::const_iterator it = value.begin();
	bool	integer = false;
	bool	separator = false;
	bool	decimal = false;
    while (it != value.end()) {
		if (std::isdigit(*it) != 0) {
			if (!integer)
				integer = true;
			else {
				if (separator)
					decimal = true;
			}
		}
		else {
			if (*it == '.' && integer)
			 	separator = true;
			else if (!(*it == '-' && !integer))
				throw AProgramReader::OperandValueException();
		}
		it++;
	}
	if (!integer || ((opType == Float || opType == Double) && !decimal))
		throw AProgramReader::OperandValueException();

	this->factory.createOperand(opType, value);

	return value;
}

std::string const AProgramReader::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "AProgramReader:{}";

	return debugStr.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================

std::map<std::string, bool> const AProgramReader::create_instr_map(void) {
	std::map<std::string, bool> m;

	m["push"] = true;
	m["pop"] = false;
	m["dump"] = false;
	m["assert"] = true;
	m["add"] = false;
	m["sub"] = false;
	m["mul"] = false;
	m["div"] = false;
	m["mod"] = false;
	m["print"] = false;
	m["exit"] = false;
	// Bonus
	m["clear"] = false;
	m["and"] = false;
	m["or"] = false;
	m["xor"] = false;
	m["pow"] = false;
	m["sqrt"] = false;

	return m;
}

std::map<std::string, bool> const AProgramReader::INSTR = AProgramReader::create_instr_map();


std::map<std::string, eOperandType> const AProgramReader::create_operand_map(void) {
	std::map<std::string, eOperandType> m;

	m["int8("] = Int8;
	m["int16("] = Int16;
	m["int32("] = Int32;
	m["float("] = Float;
	m["double("] = Double;

	return m;
}

std::map<std::string, eOperandType> const AProgramReader::OPERAND = AProgramReader::create_operand_map();

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

const char *AProgramReader::UnkownOperandException::what() const throw() {
		return "Unknown operand given";
}

const char *AProgramReader::ParenthesisException::what() const throw() {
		return "Error with parenthesis";
}

const char *AProgramReader::OperandValueException::what() const throw() {
		return "Error in the operand value";
}

// === END EXCEPTIONS ==========================================================
