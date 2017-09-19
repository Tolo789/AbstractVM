#ifndef APROGRAM_READER_HPP
# define APROGRAM_READER_HPP
# include <iostream>
# include <list>
# include <map>
# include "eOperandType.hpp"
# include "OperandFactory.hpp"

class AProgramReader {

public:

	AProgramReader(void);
	~AProgramReader(void);

	virtual void execute(std::list<std::string> program, std::list<char> options) = 0;

	std::string const	serialize(void) const;

	class UnkownOperandException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class ParenthesisException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class OperandValueException : public std::exception
	{
		virtual const char* what() const throw();
	};

protected:
	static std::map<std::string, bool> const	INSTR;
    static std::map<std::string, bool> const	create_instr_map(void);

	static std::map<std::string, eOperandType> const	OPERAND;
    static std::map<std::string, eOperandType> const	create_operand_map(void);

	eOperandType			getOperandType(std::string opStr);
	std::string				getOperandValue(std::string opStr, eOperandType opType);
	std::list<std::string>	getLineElements(std::string line);

	OperandFactory				factory;

};

std::ostream & operator<<(std::ostream & o, AProgramReader const & rhs);

#endif
