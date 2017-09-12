#ifndef ABSTRACTVM_HPP
# define ABSTRACTVM_HPP
# include <iostream>
# include <list>

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

private:
	AbstractVM(void);

	int	 parseOptions(int argc, char **argv);
	void getProgramFromFile(char *filePath);
	void getProgramFromStdInput(void);

	std::list<char>			options;
	std::list<std::string>	program;

};

std::ostream & operator<<(std::ostream & o, AbstractVM const & rhs);

#endif
