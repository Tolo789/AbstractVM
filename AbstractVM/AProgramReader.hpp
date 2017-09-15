#ifndef APROGRAM_READER_HPP
# define APROGRAM_READER_HPP
# include <iostream>
# include <list>
# include <map>

class AProgramReader {

public:

	AProgramReader(void);
	~AProgramReader(void);

	virtual void execute(std::list<std::string> program, std::list<char> options) = 0;

	std::string const	serialize(void) const;

protected:
    static std::map<std::string,int> const	create_map(void);
	static std::map<std::string, int> const	INSTR;

	std::list<std::string>	getLineElements(std::string line);

};

std::ostream & operator<<(std::ostream & o, AProgramReader const & rhs);

#endif
