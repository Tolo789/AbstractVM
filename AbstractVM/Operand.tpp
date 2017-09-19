#ifndef OPERAND_TPP
# define OPERAND_TPP
# include <iostream>
# include <sstream>
# include <math.h>
# include "IOperand.hpp"
# include "eOperandType.hpp"
# include "OperandFactory.hpp"

template<eOperandType T>
class Operand : public IOperand {

public:
	Operand(Operand const & src) {
		this->value = src->getValue();
		this->type = src->getType();
	}
	Operand(std::string value) : type(T) {
		if (T >= Float)
			this->value = std::stod(value);
		else
			this->value = round(std::stod(value));
	}
	~Operand(void) {}


	int getPrecision(void) const {
		return this->type;
	}
	eOperandType getType(void) const {
		return this->type;
	}
	double getValue(void) const {
		return this->value;
	}


	IOperand const * operator+( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return this->factory.createOperand(this->getType(), std::to_string(this->value + static_cast<Operand<Double> const &>(rhs).getValue()));
		}

		return this->factory.createOperand(rhs.getType(), std::to_string(this->value + static_cast<Operand<Double> const &>(rhs).getValue()));
	}

	IOperand const * operator-( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return this->factory.createOperand(this->getType(), std::to_string(this->value - static_cast<Operand<Double> const &>(rhs).getValue()));
		}

		return this->factory.createOperand(rhs.getType(), std::to_string(this->value - static_cast<Operand<Double> const &>(rhs).getValue()));
	}

	IOperand const * operator*( IOperand const & rhs ) const {
		if (rhs.getType() < this->getType()) {
			return this->factory.createOperand(this->getType(), std::to_string(this->value *static_cast<Operand<Double> const &>(rhs).getValue()));
		}

		return this->factory.createOperand(rhs.getType(), std::to_string(this->value * static_cast<Operand<Double> const &>(rhs).getValue()));
	}

	IOperand const * operator/( IOperand const & rhs ) const {
		if (static_cast<Operand<Double> const &>(rhs).getValue() == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {

			return this->factory.createOperand(this->getType(), std::to_string(this->value / static_cast<Operand<Double> const &>(rhs).getValue()));
		}

		return this->factory.createOperand(rhs.getType(), std::to_string(this->value / static_cast<Operand<Double> const &>(rhs).getValue()));
	}

	IOperand const * operator%( IOperand const & rhs ) const {
		if (static_cast<Operand<Double> const &>(rhs).getValue() == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {
			if (this->getType() >= Float)
				throw Operand::IntegerOnlyException();

			return this->factory.createOperand(this->getType(), std::to_string(static_cast<int>(this->value) % static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
		}
		if (rhs.getType() >= Float)
			throw Operand::IntegerOnlyException();

		return this->factory.createOperand(rhs.getType(), std::to_string(static_cast<int>(this->value) % static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
	}

	IOperand const * operator&( IOperand const & rhs ) const {
		if (static_cast<Operand<Double> const &>(rhs).getValue() == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {
			if (this->getType() >= Float)
				throw Operand::IntegerOnlyException();

			return this->factory.createOperand(this->getType(), std::to_string(static_cast<int>(this->value) & static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
		}
		if (rhs.getType() >= Float)
			throw Operand::IntegerOnlyException();

		return this->factory.createOperand(rhs.getType(), std::to_string(static_cast<int>(this->value) & static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
	}

	IOperand const * operator|( IOperand const & rhs ) const {
		if (static_cast<Operand<Double> const &>(rhs).getValue() == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {
			if (this->getType() >= Float)
				throw Operand::IntegerOnlyException();

			return this->factory.createOperand(this->getType(), std::to_string(static_cast<int>(this->value) | static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
		}
		if (rhs.getType() >= Float)
			throw Operand::IntegerOnlyException();

		return this->factory.createOperand(rhs.getType(), std::to_string(static_cast<int>(this->value) | static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
	}

	IOperand const * operator^( IOperand const & rhs ) const {
		if (static_cast<Operand<Double> const &>(rhs).getValue() == 0)
			throw ZeroDivisionException();

		if (rhs.getType() < this->getType()) {
			if (this->getType() >= Float)
				throw Operand::IntegerOnlyException();

			return this->factory.createOperand(this->getType(), std::to_string(static_cast<int>(this->value) ^ static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
		}
		if (rhs.getType() >= Float)
			throw Operand::IntegerOnlyException();

		return this->factory.createOperand(rhs.getType(), std::to_string(static_cast<int>(this->value) ^ static_cast<int>(static_cast<Operand<Double> const &>(rhs).getValue())));
	}

	std::string const & toString( void ) const {
			std::stringstream debugStr;
			debugStr << "Operand:{value:" << this->value << ", type:" << this->type << "}";

			std::string const *retStr = new std::string(debugStr.str());

			std::string const &refStr = *retStr;

			return refStr;
	}

	class IntegerOnlyException : public std::exception
	{
		virtual const char* what() const throw() {return "Operation for Int-only has benn called with Float/Double values";}
	};
	class ZeroDivisionException : public std::exception
	{
		virtual const char* what() const throw() {return "Zero division detected";}
	};

private:
	Operand(void);

	eOperandType	type;
	double			value;
	OperandFactory	factory;
};

#endif
