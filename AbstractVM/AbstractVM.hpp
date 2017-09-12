#ifndef ABSTRACTVM_HPP
# define ABSTRACTVM_HPP
# include <iostream>

class AbstractVM {

public:

	AbstractVM(AbstractVM const & src);
	AbstractVM(int argc, char **argv);
	~AbstractVM(void);

	AbstractVM& operator=(AbstractVM const & rhs);

	char *getOptions() const;
	char *getProgram() const;
	
	int parseOptions(int argc, char **argv);

	std::string const  serialize(void) const;

	class FilepathNumberException : public std::exception
	{
		virtual const char* what() const throw();
	};
	class UnknownArgumentException : public std::exception
	{
		virtual const char* what() const throw();
	};

private:
	AbstractVM(void);

	char	*options;
	int		optionsSize;
	char	*program;
	int		programLines;

};

std::ostream & operator<<(std::ostream & o, AbstractVM const & rhs);

#endif
