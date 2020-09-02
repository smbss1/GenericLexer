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

	lexer.Process("void print(string strText);[]{}>>>");
	helper::Dump(lexer);
}