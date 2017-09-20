#include <iostream>
#include <sstream>
#include "Color.hpp"

// === CONSTRUCTOR =============================================================

Color::Color(void) {
	// std::cout << "Color default constructor called" << std::endl;

	return ;
}

Color::~Color(void) {
	// std::cout << "Color destructor called" << std::endl;

	return ;
}

// === ENDCONSTRUCTOR ==========================================================

// === OPERATORS ===============================================================
// === ENDOPERATORS ============================================================

// === GET / SET ===============================================================
// === END GET / SET ===========================================================

// === OTHERS ==================================================================

std::string Color::Reset(void) {
	return "\033[0m";
}

std::string Color::Red(void) {
	return "\033[1;31m";
}

std::string Color::Yellow(void) {
	return "\033[1;33m";
}


std::string Color::Blue(void) {
	return "\033[1;34m";
}
// === ENDOTHERS ===============================================================

// === STATICVARS ==============================================================
// === END STATICVARS ==========================================================

// === EXCEPTIONS ==============================================================
// === END EXCEPTIONS ==========================================================
