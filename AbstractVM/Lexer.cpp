#include <iostream>
#include <sstream>
#include "Lexer.hpp"

// === CONSTRUCTOR =============================================================
// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================
// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================
// === END GET / SET ===========================================================

// === OTHERS ==================================================================

std::string Lexer::createError(int lineCount, std::string errorMsg, std::string strGiven) {
	std::stringstream errorStr;
	errorStr << "ERROR - Line " << lineCount << " - " << errorMsg << " ('" << strGiven << "' given)";

	return errorStr.str();
}

void Lexer::execute(std::list<std::string> program, std::list<char> options) {
	if (find(options.begin(), options.end(), 'h') != options.end())
		return ;

	std::list<std::string>	errorList;
	std::list<std::string>	elements;

	std::list<std::string>::const_iterator		lineIterator;
	std::list<std::string>::const_iterator		elemIterator;
	std::map<std::string,int>::const_iterator	mapResult;

	std::size_t lineCount = 0;
	std::size_t elemCount = 0;

	bool param_needed;

	for (lineIterator = program.begin(); lineIterator != program.end(); ++lineIterator) {
		lineCount++;
		if (*lineIterator == "" or lineIterator->substr(0, 1) == ";")
			continue ;

		elements = this->getLineElements(*lineIterator);
		elemCount = 0;
		for (elemIterator = elements.begin(); elemIterator != elements.end(); ++elemIterator) {
			elemCount++;

			if (elemCount == 1) {
				param_needed = false;
				mapResult = Lexer::INSTR.find(*elemIterator);
				if (mapResult == Lexer::INSTR.end()) {
					errorList.push_back(Lexer::createError(lineCount, "Unknown instruction", *elemIterator));

					break ;
				}
				else if (mapResult->second == 1)
					param_needed = true;
			}
			else if (elemCount == 2) {
				if (param_needed == true) {
					if (*elemIterator == "" or elemIterator->substr(0, 1) == ";") {
						errorList.push_back(Lexer::createError(lineCount, "Instruction needs a parameter", *elemIterator));

						break ;
					}
					// TODO: check that the given param is good
				}
				else {
					if (*elemIterator != "" and elemIterator->substr(0, 1) != ";") {
						errorList.push_back(Lexer::createError(lineCount, "Instruction doesnt need any parameter", *elemIterator));

						break ;
					}
				}
			}
			else if (*elemIterator != "" and elemIterator->substr(0, 1) != ";") {
				errorList.push_back(Lexer::createError(lineCount, "Instruction doesnt need any other parameter", *elemIterator));
			}
		}
	}

	if (errorList.size() != 0)
		throw Lexer::LexerException(errorList);
	return ;
}

std::string const Lexer::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "Lexer:{}";

	return debugStr.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================
// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

Lexer::LexerException::LexerException(std::list<std::string> errorList) {
	for (std::list<std::string>::const_iterator it = errorList.begin(); it != errorList.end(); ++it) {
		if (*it != "")
			this->errorMsg += *it + "\n";
	}
}

Lexer::LexerException::~LexerException()  _NOEXCEPT{
}

const char *Lexer::LexerException::what() const throw() {
		return errorMsg.c_str();
}

// === END EXCEPTIONS ==========================================================
