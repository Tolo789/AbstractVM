#ifndef LEXER_HPP
# define LEXER_HPP
# include <iostream>
# include <list>
# include "AProgramReader.hpp"

class Lexer : public AProgramReader {

public:

	void execute(std::list<std::string> program, std::list<char> options);

	std::string const	serialize(void) const;

	class LexerException : public std::exception
	{
		public:
			LexerException(std::list<std::string> errorList);
			~LexerException() _NOEXCEPT;

		private:

			virtual const char* what() const throw();

			std::string errorMsg;
	};

private:
	static std::string createError(int lineCount, std::string errorMsg, std::string strGiven);
};

#endif
