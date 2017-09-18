#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <list>
# include <map>
# include "AProgramReader.hpp"
# include "IOperand.hpp"

class Parser : public AProgramReader {

public:
	~Parser(void);

	void execute(std::list<std::string> program, std::list<char> options);

	std::string const	serialize(void) const;

private:

    typedef void (Parser::*instrFuncPointer)(std::string param);
    static std::map<std::string, Parser::instrFuncPointer> const	INSTR_FUNC_MAP;
    static std::map<std::string, Parser::instrFuncPointer> const	create_instr_func_map(void);

    typedef IOperand const * (Parser::*opFuncCreatePointer)(std::string const & param) const;
    static std::map<eOperandType, opFuncCreatePointer> const	OP_FUNC_MAP;
    static std::map<eOperandType, opFuncCreatePointer> const	create_op_func_map(void);

	IOperand const * createOperand( eOperandType type, std::string const & value ) const;
	IOperand const * createInt8( std::string const & value ) const;
	IOperand const * createInt16( std::string const & value ) const;
	IOperand const * createInt32( std::string const & value ) const;
	IOperand const * createFloat( std::string const & value ) const;
	IOperand const * createDouble( std::string const & value ) const;

	void PushFunction(std::string param);
	void PopFunction(std::string param);
	void DumpFunction(std::string param);

	std::list<IOperand const *> values;
};

#endif
