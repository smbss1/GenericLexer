/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#include <string>
#include <vector>
#include <iostream>
#include "Lexer.h"
#include "DFA.h"

int main(int argc, char *argv[]) {
     Lexer oLexer;
//	std::vector<std::string> oSymbols = {
//		"(", ")", "[", "]",
//		"{", "}", ";",
//		"::", "->", ":",
//		"*", "+", "-",
//		"/", "<<", ">>", ",","!", ">"
//	};
//	lexer.AddSymbols(oSymbols);
//	lexer.LoadGrammar(
//	    "terminal:"
//                   "<Whitespace>:[ ]"
//                   "<Word>: [a-zA-Z0-9]"
//                   "<Number>: [0-9]"
//                   "<Comma>: ','"
//                   "<RBraces>: '('"
//                   "<LBraces>: ')'"
//                   "<Div>: '/'"
//                   "<Plus>: '+'"
//                   "<Expression>: <Number> + <Number>");
//    lexer.AddWhitespace(' ');
//    lexer.Define("Alpha","abcdefghijklmnopqrstuvwxyz0123456789");
//    lexer.Define("Integer","0123456789");
//    lexer.Define("Plus","+");
//	lexer.Process("19 + 2 + d + 4bdfc484");
//	helper::Dump(lexer);

//    DFA<char> dfa(0, false);
//    dfa.AddState(TokenType::TK_INTEGER, true);
//    dfa.AddState(TokenType::TK_ID, true);
//    dfa.AddState(TokenType::TK_OP_PLUS, true);
//    dfa.AddState(TokenType::TK_WHITESPACE, true);
//
//    dfa.AddTransition(0, '+', TokenType::TK_OP_PLUS);
//
//    dfa.AddTransition(0, ' ', TokenType::TK_WHITESPACE);
//    dfa.AddTransition(TokenType::TK_WHITESPACE, ' ', TokenType::TK_WHITESPACE);
//
//    for(char c='a'; c<='z'; c++)
//    {
//        dfa.AddTransition(0, c, TokenType::TK_ID);
//        dfa.AddTransition(TokenType::TK_ID, c, TokenType::TK_ID);
//    }
//
//    for(char c='0'; c<='9'; c++)
//    {
//        dfa.AddTransition(TokenType::TK_ID, c, TokenType::TK_ID);
//    }
//
//
//    for(char c = '0'; c <= '9'; c++)
//    {
//        dfa.AddTransition(0, c, TokenType::TK_INTEGER);
//        dfa.AddTransition(TokenType::TK_INTEGER, c, TokenType::TK_INTEGER);
//    }
//
//    std::string inp;
//    getline(std::cin, inp);
//
//    auto tk = lex(dfa, inp);
//    for(auto i = tk.begin(); i < tk.end(); i++)
//        std::cout<< i->lexeme << " : " << token_name(i->type) << std::endl;

    oLexer.Define("Alpha","abcdefghijklmnopqrstuvwxyz0123456789");
    oLexer.Define("Alpha","0123456789", false);
    oLexer.Define("Whitespace"," ", false);
    oLexer.Define("Integer","0123456789");
    oLexer.Define("Plus","+");
    oLexer.Process("19 + 2 + d + 4bdfc484");

    auto tk = oLexer.oTokenList;
    for(auto i = tk.begin(); i < tk.end(); i++)
        std::cout<< i->m_strText << ": " << i->m_strType << std::endl;
}