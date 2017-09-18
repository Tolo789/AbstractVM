#ifndef OPERAND_TPP
# define OPERAND_TPP
# include <iostream>
# include <sstream>
# include "IOperand.hpp"
# include "eOperandType.hpp"

template<eOperandType T>
class Operand : public IOperand {

public:
	Operand(Operand const & src) {
		this->value = src->getValue();
		this->type = src->getType();
	}
	Operand(std::string value) : type(T) {
		// TODO: check for overflows
		// TODO: take only int part for ints
		this->value = std::stod(value);
	}
	Operand(double value) : type(T) {
		// TODO: check for overflows
		// TODO: take only int part for ints
		this->value = value;

	}
	~Operand(void) {}


	int getPrecision(void) const {
		return this->type;
	}
	eOperandType getType(void) const {
		return this->type;
	}


	IOperand const * operator+( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return new Operand<Float>(this->value + rhs.value);
		}

		return new Operand<Double>(this->value + rhs.value);
	}

	IOperand const * operator-( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return new Operand<Float>(this->value - rhs.value);
		}

		return new Operand<Double>(this->value - rhs.value);
	}

	IOperand const * operator*( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return new Operand<Float>(this->value *rhs.value);
		}

		return new Operand<Double>(this->value * rhs.value);
	}

	IOperand const * operator/( IOperand const & rhs ) const {
		if (rhs.value == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {

			return new Operand<Float>(this->value / rhs.value);
		}

		return new Operand<Double>(this->value / rhs.value);
	}

	IOperand const * operator%( IOperand const & rhs ) const {
		if (rhs.value == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {
			if (this->getType() >= Float)
				throw Operand::IntegerOnlyException();

			return new Operand<Float>(static_cast<double>(static_cast<int>(this->value) % static_cast<int>(rhs.value)));
		}
		if (rhs.getType() >= Float)
			throw Operand::IntegerOnlyException();

		return new Operand<Double>(static_cast<double>(static_cast<int>(this->value) % static_cast<int>(rhs.value)));
	}

	std::string const & toString( void ) const {
			std::stringstream debugStr;
			debugStr << "Operand:{value:" << this->value << ", type:" << this->type << "}";

			std::string const *retStr = new std::string(debugStr.str());

			return retStr;
	}

	class IntegerOnlyException : public std::exception
	{
		virtual const char* what() const throw() {return "Operation for Int only has benn called with Float/Double values";}
	};
	class ZeroDivisionException : public std::exception
	{
		virtual const char* what() const throw() {return "Zero division detected";}
	};

private:
	Operand(void);

	eOperandType	type;
};

#endif
