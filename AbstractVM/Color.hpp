#ifndef COLOR_HPP
# define COLOR_HPP
# include <iostream>

class Color {

public:

	Color(void);
	~Color(void);

	static std::string Reset(void);
	static std::string Red(void);
	static std::string Yellow(void);
	static std::string Blue(void);

};


#endif
