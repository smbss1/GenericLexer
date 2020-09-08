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

void Regex(DFA& oDfa, const std::string& strId, std::string strValue)
{
    // m_oAllDefines.insert(make_pair(strId, strValue));
    if (!oDfa.StateExist(strId))
        oDfa.AddState(DFAState(false, strId));

    int iState = oDfa.GetStateID(strId);
    int nextId;
    int prevId = 0;

    for (int i = 0; i < strValue.size(); i++) {
        if (strValue[i] == '[')
        {
            while (strValue[i] != ']' && strValue[i])
            {
                char first = strValue[i];
                if (strValue[i + 1] == '-')
                {
                    char last = strValue[i + 2];
                    for (char c = first; c <= last; c++)
                        oDfa.AddTransition(prevId, c, iState);
//                    if (bStar) {
//                        bStar = false;
//                        for (char c = first; c <= last; c++)
//                            oDfa.AddTransition(iState, c, iState);
//                    }
                    i += 2;
                }
                else if (first != '[' && first != ']') {
                    oDfa.AddTransition(prevId, first, iState);
                }
                i++;
            }
        }
        else
        {
            if (i > 0 && i < strValue.size() - 1) {
                nextId = oDfa.AddState(DFAState(false, string(1, strValue[i])));
            } else if (i < strValue.size()) {
                nextId = oDfa.AddState(DFAState(false, string(1, strValue[i]), true));
            } else {
                nextId = iState;
            }
            oDfa.AddTransition(prevId, strValue[i], nextId);
            prevId = nextId;
        }
    }
}

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

//    oLexer.Define("Alpha","[a-z]*[0-9]*");
//    oLexer.Define("WHITESPACE"," ");
//    oLexer.Define("Integer","[0-9]*");
//    oLexer.Define("Plus","+");
//    oLexer.Process("19 + 2 + d + 4bdfc484");
//
//    auto tk = oLexer.oTokenList;
//    for(auto i = tk.begin(); i < tk.end(); i++)
//        std::cout<< i->m_strText << ": " << i->m_strType << std::endl;

    DFA dfa;
    Regex(dfa, "Action0", "[a-f]");

    std::string lexeme;
    dfa.Reset();
    std::string m_strText = "a";

    for(auto itChar = m_strText.begin(); itChar < m_strText.end(); itChar++)
    {
        //std::cout << dfa.m_iCurrentStateID << std::endl;
        lexeme += *itChar;
        char next = *(itChar + 1);
        dfa.MakeNextTransition(*itChar);
        if(dfa.IsAccepting() && !dfa.IsAccepting(next))
        {
            std::cout << lexeme << std::endl;
            lexeme.clear();
            dfa.Reset();
        }
    }
}