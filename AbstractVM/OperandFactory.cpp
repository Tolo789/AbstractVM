#include <iostream>
#include <sstream>
#include "OperandFactory.hpp"
#include "Operand.tpp"

// === CONSTRUCTOR =============================================================

OperandFactory::OperandFactory(void) {
	// std::cout << "OperandFactory default constructor called" << std::endl;

	return ;
}

OperandFactory::~OperandFactory(void) {
	// std::cout << "OperandFactory destructor called" << std::endl;

	return ;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================
// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================
// === END GET / SET ===========================================================

// === OTHERS ==================================================================

IOperand const * OperandFactory::createOperand( eOperandType type, std::string const & value ) const {
	double valueCheck;
	try {
	 	valueCheck = std::stod(value);
	}
	catch (std::exception & e) {
		throw OperandFactory::OperandOverflowException();
	}
	if (valueCheck > OperandFactory::MAX_VAL[type] || valueCheck < (-OperandFactory::MAX_VAL[type] - 1))
		throw OperandFactory::OperandOverflowException();

	OperandFactory::opFuncCreatePointer opCreateFunc = OperandFactory::OP_FUNC_MAP.find(type)->second;

	return (this->*opCreateFunc)(value);
}
IOperand const * OperandFactory::createInt8( std::string const & value ) const {
	return new Operand<Int8>(value);
}
IOperand const * OperandFactory::createInt16( std::string const & value ) const {
	return new Operand<Int16>(value);
}
IOperand const * OperandFactory::createInt32( std::string const & value ) const {
	return new Operand<Int32>(value);
}
IOperand const * OperandFactory::createFloat( std::string const & value ) const {
	return new Operand<Float>(value);
}
IOperand const * OperandFactory::createDouble( std::string const & value ) const {
	return new Operand<Double>(value);
}

// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================

std::map<eOperandType, OperandFactory::opFuncCreatePointer> const OperandFactory::create_op_func_map(void) {
	std::map<eOperandType, OperandFactory::opFuncCreatePointer> m;

	m[Int8] = &OperandFactory::createInt8;
	m[Int16] = &OperandFactory::createInt16;
	m[Int32] = &OperandFactory::createInt32;
	m[Float] = &OperandFactory::createFloat;
	m[Double] = &OperandFactory::createDouble;

	return m;
}

std::map<eOperandType, OperandFactory::opFuncCreatePointer> const OperandFactory::OP_FUNC_MAP = OperandFactory::create_op_func_map();

double const OperandFactory::MAX_VAL[5] = {std::numeric_limits<char>::max(), std::numeric_limits<short>::max(), std::numeric_limits<int>::max(), std::numeric_limits<float>::max(), std::numeric_limits<double>::max()};

// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================

const char *OperandFactory::OperandOverflowException::what() const throw() {
		return "Overflow of operand detected";
}

// === END EXCEPTIONS ==========================================================
