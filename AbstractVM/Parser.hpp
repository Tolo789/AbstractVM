#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <list>
# include <map>
# include "AProgramReader.hpp"
# include "IOperand.hpp"

class Parser : public AProgramReader {

public:

	void execute(std::list<std::string> program, std::list<char> options);

	std::string const	serialize(void) const;

private:

    typedef void (Parser::*instrFuncPointer)(std::string param);
    static std::map<std::string, Parser::instrFuncPointer> const	INSTR_FUNC_MAP;
    static std::map<std::string, Parser::instrFuncPointer> const	create_instr_func_map(void);

	void PushFunction(std::string param);
	void PopFunction(std::string param);
	void DumpFunction(std::string param);

	// std::list<IOperand> values;
	// instrFuncPointer	instrFunc;
};

#endif
