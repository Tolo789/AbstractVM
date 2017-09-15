#ifndef PARSER_HPP
# define PARSER_HPP
# include <iostream>
# include <list>
# include "AProgramReader.hpp"

class Parser : public AProgramReader {

public:

	void execute(std::list<std::string> program, std::list<char> options);

	std::string const	serialize(void) const;

private:

};

#endif
