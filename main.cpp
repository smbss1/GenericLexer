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
    oLexer.Define("Alpha","[a-z_]+[0-9]*");
    oLexer.Define("Whitespace"," ");
    oLexer.Process("6494abcdab4854 484");

    helper::Dump(oLexer);

    return 0;
}