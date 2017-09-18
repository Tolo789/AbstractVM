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

	std::string								line = "";
	std::list<std::string>					elements;
	std::list<std::string>::const_iterator	elemIterator;
	std::size_t								lineCount = 0;
	std::size_t								elemCount = 0;

	instrFuncPointer	instrFunc;
	std::string			param;

	for (std::list<std::string>::const_iterator lineIterator = program.begin(); lineIterator != program.end(); ++lineIterator) {
		lineCount++;
		if (*lineIterator == "" or lineIterator->substr(0, 1) == ";")
			continue ;

		if (find(options.begin(), options.end(), 'd') != options.end()) {
			if (lineCount > 1)
				std::cout << std::endl;
			std::cout << "Line n." << lineCount << " - " << *lineIterator << std::endl;
			std::getline(std::cin, line);
		}


		elements = this->getLineElements(*lineIterator);
		elemCount = 0;
		param = "";
		for (elemIterator = elements.begin(); elemIterator != elements.end(); ++elemIterator) {
			elemCount++;
			if (*elemIterator == "" or elemIterator->substr(0, 1) == ";")
				break ;

			if (elemCount == 1) {
				instrFunc = Parser::INSTR_FUNC_MAP.find(*elemIterator)->second;
				param = Parser::INSTR_FUNC_MAP.find(*elemIterator)->first;
			}
			else {
				param = *elemIterator;
			}
		}
		std::cout << "Func: " << instrFunc << ", param: " << param << std::endl;
		// this->instrFunc(param);
	}
}

void Parser::PushFunction(std::string param) {
	std::cout << "Push" << std::endl;
	if (param != "")
		return ;

	return;
}

void Parser::PopFunction(std::string param) {
	std::cout << "Pop" << std::endl;
	if (param != "")
		return ;

	return;
}

void Parser::DumpFunction(std::string param) {
	std::cout << "Dump" << std::endl;
	if (param != "")
		return ;

	return;
}

std::string const Parser::serialize(void) const {
	std::stringstream debugStr;
	debugStr << "Parser:{}";

	return debugStr.str();
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================

std::map<std::string, Parser::instrFuncPointer> const Parser::create_instr_func_map(void) {
	std::map<std::string, instrFuncPointer> m;

	m["push"] = &Parser::PushFunction;
	m["pop"] = &Parser::PopFunction;
	m["dump"] = &Parser::DumpFunction;
	// m["assert"] = 1;
	// m["add"] = 0;
	// m["sub"] = 0;
	// m["mul"] = 0;
	// m["div"] = 0;
	// m["mod"] = 0;
	// m["print"] = 0;
	// m["exit"] = 0;

	return m;
}

std::map<std::string, Parser::instrFuncPointer> const Parser::INSTR_FUNC_MAP = Parser::create_instr_func_map();

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================
// === END EXCEPTIONS ==========================================================
