#include <iostream>
#include <sstream>
#include <math.h>
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
	for (std::list<std::string>::const_iterator lineIterator = program.begin(); lineIterator != program.end() && !this->programEnded; ++lineIterator) {
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
			(this->*instrFunc)(param);
		}
		catch (std::exception & e) {
			throw Parser::ParserException(lineCount, e.what(), *lineIterator);
		}
	}

	if (!this->programEnded)
		throw Parser::ProgramWithoutEndException();
}


void Parser::PushFunction(std::string param) {
	if (param == "")
		return ;


	eOperandType opType = this->getOperandType(param);
	std::string	valueStr = this->getOperandValue(param, opType);

	IOperand const *newOperand = this->factory.createOperand(opType, valueStr);
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
		std::cout << static_cast<Operand<Double> const *>(*elem)->getValue() << std::endl;
	}

	return;
}

void Parser::AssertFunction(std::string param) {
	if (param == "")
		return ;
	if (this->values.size() < 1)
		throw Parser::StackSizeException();

	std::list<IOperand const *>::iterator first = this->values.begin();

	eOperandType opType = this->getOperandType(param);
	std::string	valueStr = this->getOperandValue(param, opType);
	IOperand const *checkOperand = this->factory.createOperand(opType, valueStr);

	double firstVal = static_cast<Operand<Double> const *>(*first)->getValue();
	double checkVal = static_cast<Operand<Double> const *>(checkOperand)->getValue();
	if (firstVal != checkVal)
		throw Parser::AssertException();

	if (find(this->options.begin(), this->options.end(), 's') != this->options.end()) {
		if ((*first)->getPrecision() != checkOperand->getPrecision())
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
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**first) + (**second);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::SubFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**first) - (**second);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::MulFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**first) * (**second);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::DivFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**second) / (**first);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::ModFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**second) % (**first);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::PrintFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 1)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	if ((*first)->getType() != Int8)
		throw Parser::PrintException();

	std::cout << static_cast<char>(static_cast<Operand<Double> const *>(*first)->getValue());

	return;
}

void Parser::ExitFunction(std::string param) {
	if (param != "")
		return ;

	this->programEnded = true;

	return;
}

void Parser::ClearFunction(std::string param) {
	if (param != "")
		return ;

	for (std::list<IOperand const *>::iterator elem = this->values.begin(); elem != this->values.end(); ++elem) {
		delete(*elem);
	}
	this->values.clear();

	return;
}

void Parser::AndFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**second) & (**first);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::OrFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**second) | (**first);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::XorFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());
	IOperand const *newOperand = (**second) ^ (**first);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::PowFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 2)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();
	std::list<IOperand const *>::iterator second = ++(this->values.begin());

	eOperandType opType = ((*first)->getType() > (*second)->getType()) ? (*first)->getType() : (*second)->getType();
	double firstVal = static_cast<Operand<Double> const *>(*first)->getValue();
	double secondVal = static_cast<Operand<Double> const *>(*second)->getValue();
	std::string	valueStr = std::to_string(pow(secondVal, firstVal));

	IOperand const *newOperand = this->factory.createOperand(opType, valueStr);

	this->values.pop_front();
	this->values.pop_front();
	this->values.push_front(newOperand);

	return;
}

void Parser::SqrtFunction(std::string param) {
	if (param != "")
		return ;
	if (this->values.size() < 1)
		throw Parser::StackSizeException();


	std::list<IOperand const *>::iterator first = this->values.begin();

	eOperandType opType = (*first)->getType();
	std::string	valueStr = std::to_string(sqrt(static_cast<Operand<Double> const *>(*first)->getValue()));

	IOperand const *newOperand = this->factory.createOperand(opType, valueStr);

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
	m["add"] = &Parser::AddFunction;
	m["sub"] = &Parser::SubFunction;
	m["mul"] = &Parser::MulFunction;
	m["div"] = &Parser::DivFunction;
	m["mod"] = &Parser::ModFunction;
	m["print"] = &Parser::PrintFunction;
	m["exit"] = &Parser::ExitFunction;
	// Bonus
	m["clear"] = &Parser::ClearFunction;
	m["and"] = &Parser::AndFunction;
	m["or"] = &Parser::OrFunction;
	m["xor"] = &Parser::XorFunction;
	m["pow"] = &Parser::PowFunction;
	m["sqrt"] = &Parser::SqrtFunction;

	return m;
}

std::map<std::string, Parser::instrFuncPointer> const Parser::INSTR_FUNC_MAP = Parser::create_instr_func_map();

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

const char *Parser::PrintException::what() const throw() {
		return "Cannot print, element on stack must be Int8";
}

// === END EXCEPTIONS ==========================================================
