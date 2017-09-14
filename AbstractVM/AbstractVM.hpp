#ifndef ABSTRACTVM_HPP
# define ABSTRACTVM_HPP
# include <iostream>
# include <list>
# include <map>

class AbstractVM {

public:

	AbstractVM(AbstractVM const & src);
	AbstractVM(int argc, char **argv);
	~AbstractVM(void);

	AbstractVM& operator=(AbstractVM const & rhs);

	std::list<char>			getOptions() const;
	std::list<std::string>	getProgram() const;

	std::string const	serialize(void) const;

	class UnkownOptionException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class FilepathNumberException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class FileNameException : public std::exception
	{
		virtual const char* what() const throw();
	};
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

    static std::map<std::string,int> const create_map(void);
	static std::map<std::string, int> const INSTR;

	AbstractVM(void);

	int						parseOptions(int argc, char **argv);
	void					getProgramFromFile(char *filePath);
	void					getProgramFromStdInput(void);
	void					trimProgram(void);
	std::list<std::string>	lexerCheck(void);
	std::list<std::string>	getLineElements(std::string line);

	std::list<char>			options;
	std::list<std::string>	program;

};

std::ostream & operator<<(std::ostream & o, AbstractVM const & rhs);

#endif
