#include <iostream>
#include <vector>
#include <string>
#include "Lexer.h"

int main(int argc, char *argv[])
{
	Lexer lexer;
	std::vector<std::string> oSymbols = {
		"(", ")", "[", "]",
		"{", "}", ";",
		"::", "->", ":",
		"*", "+", "-",
		"/", "<<", ">>", ",","!", ">"
	};
	lexer.AddSymbols(oSymbols);

	lexer.Process("1 (2variable1 / 3variable2) 4 + variable3");
	helper::CommutativeInserter ci;
   	ci.Process(lexer);
	helper::Dump(lexer);
}