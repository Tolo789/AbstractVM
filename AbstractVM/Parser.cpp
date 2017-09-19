#include <iostream>
#include <sstream>
#include "Parser.hpp"
#include "Operand.tpp"

// === CONSTRUCTOR =============================================================

Parser::~Parser (void) {
	for (std::list<IOperand const *>::iterator elem = this->values.begin(); elem != this->values.end(); ++elem) {
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
	this->options = options;
	bool debugMode = false;
	if (find(this->options.begin(), this->options.end(), 'd') != this->options.end()) {
		debugMode = true;
	}

	std::string								line = "";
	std::list<std::string>					elements;
	std::list<std::string>::const_iterator	elemIterator;
	std::size_t								lineCount = 0;
	std::size_t								elemCount = 0;

	Parser::instrFuncPointer	instrFunc;
	std::string					param;

	this->programEnded = false;
	for (std::list<std::string>::const_iterator lineIterator = program.begin(); lineIterator != program.end(); ++lineIterator) {
		lineCount++;
		if (*lineIterator == "" or lineIterator->substr(0, 1) == ";")
			continue ;

		if (debugMode) {
			if (lineCount > 1)
				std::cout << std::endl;
			std::cout << "Line n." << lineCount << " - " << *lineIterator;
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
			}
			else {
				param = *elemIterator;
			}
		}
		try {
			(this->*instrFunc)(param, options);
		}
		catch (std::exception & e) {
			throw Parser::ParserException(lineCount, e.what(), *lineIterator);
		}
	}

	if (!this->programEnded)
		throw Parser::ProgramWithoutEndException();
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
	if (param == "")
		return ;


	eOperandType opType = AProgramReader::getOperandType(param);
	std::string	valueStr = AProgramReader::getOperandValue(param, opType);

	IOperand const *newOperand = this->createOperand(opType, valueStr);
	this->values.push_front(newOperand);

	return;
}

void Parser::PopFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 1)
		throw Parser::StackSizeException();

	this->values.pop_front();

	return;
}

void Parser::DumpFunction(std::string param) {
	if (param != "")
		return ;

	for (std::list<IOperand const *>::iterator elem = this->values.begin(); elem != this->values.end(); ++elem) {
		std::cout << (*elem)->value << std::endl;
	}

	return;
}

void Parser::AssertFunction(std::string param) {
	if (param == "")
		return ;
	if (this->values.size() < 1)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();

	eOperandType opType = AProgramReader::getOperandType(param);
	std::string	valueStr = AProgramReader::getOperandValue(param, opType);

	IOperand const *checkOperand = this->createOperand(opType, valueStr);

	if (first.value != checkOperand.value)
		throw Parser::AssertException();

	if (find(this->options.begin(), this->options.end(), 's') != this->options.end()) {
		if (first.getPrecision() != checkOperand.getPrecision)
			throw Parser::AssertException();
	}


	return;
}

void Parser::AddFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++first;
	IOperand const *newOperand = (*first) + (*second);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

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
	m["assert"] = &Parser::AssertFunction;
	// m["add"] = &Parser::AddFunction;
	// m["sub"] = &Parser::SubFunction;
	// m["mul"] = &Parser::MulFunction;
	// m["div"] = &Parser::DivFunction;
	// m["mod"] = &Parser::ModFunction;
	// m["print"] = &Parser::PrintFunction;
	// m["exit"] = &Parser::ExitFunction;

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

Parser::ParserException::ParserException(int lineCount, std::string errorMsg, std::string strGiven) {
	this->errorMsg = "RUNTIME ERROR - Line " + std::to_string(lineCount) + " - " + errorMsg + " ('" + strGiven + "' given)";
}

Parser::ParserException::~ParserException()  _NOEXCEPT{
}

const char *Parser::ParserException::what() const throw() {
		return errorMsg.c_str();
}

const char *Parser::ProgramWithoutEndException::what() const throw() {
		return "RUNTIME ERROR - Program ended without 'exit' command";
}

const char *Parser::StackSizeException::what() const throw() {
		return "Instruction has not enough values on stack to execute";
}

const char *Parser::AssertException::what() const throw() {
		return "Assert instruction detected a difference";
}

// === END EXCEPTIONS ==========================================================
