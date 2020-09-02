#include <iostream>
#include "Lexer.h"

int main(int argc, char *argv[])
{
	Lexer lexer;

	lexer.Process("salut les amis!");
    std::deque<Token>::iterator token_itr_ = lexer.token_list_.begin();

	while (token_itr_ != lexer.token_list_.end())
	{
		std::cout << (*token_itr_).GetText() << std::endl;
		token_itr_++;
	}
}