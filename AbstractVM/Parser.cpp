#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Operand.tpp"

// === CONSTRUCTOR =============================================================

Parser::~Parser (void) {
	for (std::list<IOperand*>::iterator elem = this->values.begin(); elem != this->values.end(); ++elem) {
		delete(*elem);
	}
	this->values.clear();
}
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

	std::map<std::string, Parser::instrFuncPointer>::const_iterator	instrMapResult;
	Parser::instrFuncPointer	instrFunc;
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
				instrMapResult = Parser::INSTR_FUNC_MAP.find(*elemIterator);
				instrFunc = instrMapResult->second;
				param = instrMapResult->first;
			}
			else {
				param = *elemIterator;
			}
		}
		// std::cout << "Func: " << instrMapResult->first << ", funcName: " << instrMapResult->second << std::endl;
		(this->*instrFunc)(param);
	}
}


IOperand const * Parser::createOperand( eOperandType type, std::string const & value ) const {
	Parser::opFuncCreatePointer opCreateFunc = Parser::OP_FUNC_MAP.find(type)->second;

	return (this->*opCreateFunc)(value);
}
IOperand const * Parser::createInt8( std::string const & value ) const {
	return new Operand<Int8>(value);
}
IOperand const * Parser::createInt16( std::string const & value ) const {
	return new Operand<Int16>(value);
}
IOperand const * Parser::createInt32( std::string const & value ) const {
	return new Operand<Int32>(value);
}
IOperand const * Parser::createFloat( std::string const & value ) const {
	return new Operand<Float>(value);
}
IOperand const * Parser::createDouble( std::string const & value ) const {
	return new Operand<Double>(value);
}


void Parser::PushFunction(std::string param) {
	std::cout << "Push" << std::endl;
	if (param != "")
		return ;


	// eOperandType opType = AProgramReader::getOperandType(param);
	// std::string	valueStr = AProgramReader::getOperandValue(param, opType);
	//
	// IOperand newOperand = this->createOperand(opType, valueStr);
	// std::cout << "Test: " << newOperand->toString() << std::endl;



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
	std::map<std::string, Parser::instrFuncPointer> m;

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

std::map<eOperandType, Parser::opFuncCreatePointer> const Parser::create_op_func_map(void) {
	std::map<eOperandType, Parser::opFuncCreatePointer> m;

	m[Int8] = &Parser::createInt8;
	m[Int16] = &Parser::createInt16;
	m[Int32] = &Parser::createInt32;
	m[Float] = &Parser::createFloat;
	m[Double] = &Parser::createDouble;

	return m;
}

std::map<eOperandType, Parser::opFuncCreatePointer> const Parser::OP_FUNC_MAP = Parser::create_op_func_map();

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================
// === END EXCEPTIONS ==========================================================
