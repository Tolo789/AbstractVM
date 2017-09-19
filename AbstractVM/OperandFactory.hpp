#ifndef OPERAND_FACTORY_HPP
# define OPERAND_FACTORY_HPP
# include <iostream>
# include <map>
# include "eOperandType.hpp"
# include "IOperand.hpp"

class OperandFactory {

public:

	OperandFactory(void);
	~OperandFactory(void);

	IOperand const * createOperand( eOperandType type, std::string const & value ) const;

	class OperandOverflowException : public std::exception
	{
		virtual const char* what() const throw();
	};

private:
    typedef IOperand const * (OperandFactory::*opFuncCreatePointer)(std::string const & param) const;
    static std::map<eOperandType, opFuncCreatePointer> const	OP_FUNC_MAP;
    static std::map<eOperandType, opFuncCreatePointer> const	create_op_func_map(void);

	static double const		MAX_VAL[5];

	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;

};

#endif
