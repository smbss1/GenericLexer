/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#include <string>
#include <iostream>
#include "Lexer.h"
#include "regex.h"

int main(int argc, char *argv[])
{
    Lexer oLexer;
    oLexer.Define("Alpha","a*");
    // oLexer.Define("Integer","[0-9]");

    std::string lexeme;
    std::string lex;
    std::string m_strText = "ababab";
    RegEx re;
//    re.Compile("(ab)+");
//    std::cout << re.Count("abb") << std::endl;


    for(auto itChar = m_strText.begin(); itChar < m_strText.end(); itChar++)
    {
        lexeme += *itChar;
        char next = *(itChar + 1);
        re.Compile("(ab)*");
        if (re.Match(lexeme))
        {
            lex = lexeme;
            lex += next;
            std::cout << "\"" << lex << "\"" << std::endl;
            std::cout << "\"" << re.Count(lex) << "\"" << std::endl;
            if (!re.Match(lex))
            {
                std::cout << "'" << lexeme << "'" << std::endl;
                lexeme.clear();
            }
        }
    }

     // oLexer.Process("aaab");

    return 0;
}