/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#include "Lexer.h"

int main(int argc, char *argv[])
{
    Lexer oLexer;
    oLexer.Define("Integer","[0-9]+");
    oLexer.Define("Identifier","[A-Za-z]+[0-9]*");
    oLexer.Define("Whitespace","[ \t\n\r\b]+");
    oLexer.Define("Symbol","[(){};]");
    oLexer.Process("int myFunction() { return 0; } for (");

    helper::Dump(oLexer);

    return 0;
}