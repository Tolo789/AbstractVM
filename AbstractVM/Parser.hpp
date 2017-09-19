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

	class ParserException : public std::exception
	{
		public:
			ParserException(int lineCount, std::string errorMsg, std::string strGiven);
			~ParserException() _NOEXCEPT;

		private:

			virtual const char* what() const throw();

			std::string errorMsg;
	};
	class ProgramWithoutEndException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class StackSizeException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class AssertException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class PrintException : public std::exception
	{
		virtual const char* what() const throw();
	};

private:

    typedef void (Parser::*instrFuncPointer)(std::string param);
    static std::map<std::string, Parser::instrFuncPointer> const	INSTR_FUNC_MAP;
    static std::map<std::string, Parser::instrFuncPointer> const	create_instr_func_map(void);

	void PushFunction(std::string param);
	void PopFunction(std::string param);
	void DumpFunction(std::string param);
	void AssertFunction(std::string param);
	void AddFunction(std::string param);
	void SubFunction(std::string param);
	void MulFunction(std::string param);
	void DivFunction(std::string param);
	void ModFunction(std::string param);
	void PrintFunction(std::string param);
	void ExitFunction(std::string param);
	// Bonus
	void ClearFunction(std::string param);
	void AndFunction(std::string param);
	void OrFunction(std::string param);
	void XorFunction(std::string param);
	void PowFunction(std::string param);
	void SqrtFunction(std::string param);

	std::list<char>				options;
	std::list<IOperand const *>	values;
	bool						programEnded;
};

#endif
