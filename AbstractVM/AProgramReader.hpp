#ifndef APROGRAM_READER_HPP
# define APROGRAM_READER_HPP
# include <iostream>
# include <list>
# include <map>
# include "eOperandType.hpp"

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
	class OperandOverflowException : public std::exception
	{
		virtual const char* what() const throw();
	};

protected:
	static std::map<std::string, int> const	INSTR;
    static std::map<std::string, int> const	create_instr_map(void);

	static double const		MAX_VAL[5];
	static std::map<std::string, eOperandType> const	OPERAND;
    static std::map<std::string, eOperandType> const	create_operand_map(void);

	static eOperandType		getOperandType(std::string opStr);
	static std::string		getOperandValue(std::string opStr, eOperandType opType);

	std::list<std::string>	getLineElements(std::string line);

};

std::ostream & operator<<(std::ostream & o, AProgramReader const & rhs);

#endif
