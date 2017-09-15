#include <iostream>
#include <sstream>
#include "Parser.hpp"

// === CONSTRUCTOR =============================================================
// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================
// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================
// === END GET / SET ===========================================================

// === OTHERS ==================================================================

void Parser::execute(std::list<std::string> program, std::list<char> options) {
	if (find(options.begin(), options.end(), 'h') != options.end())
		return ;

	std::list<std::string>::const_iterator		lineIterator;

	for (lineIterator = program.begin(); lineIterator != program.end(); ++lineIterator) {
	}
}

std::string const Parser::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "Parser:{}";

	return debugStr.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================
// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================
// === END EXCEPTIONS ==========================================================
