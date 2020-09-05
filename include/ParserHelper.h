//
// Created by besseausamuel on 05/09/2020.
//

#ifndef DIRECTORY_TEST_PARSERHELPER_H
#define DIRECTORY_TEST_PARSERHELPER_H

#include <string>
#include "Lexer.h"
#include "Token.h"


class ParserHelper
{
private:

    Lexer oLexer;
    Token oCurrentToken;

    enum token_advance_mode
    {
        e_hold    = 0,
        e_advance = 1
    };

public:

    bool Init(const std::string& str);

    Lexer& GetLexer();

    const Lexer& GetLexer() const;

    void NextToken();

    const Token& CurrentToken() const;

    void AdvanceToken(token_advance_mode mode);

    bool IsToken(TokenType eType, bool bAdvance = true);

    bool IsToken(TokenType eType, const std::string& strValue, bool bAdvance = true);
    bool IsToken(TokenType eType, const char *strValue, bool bAdvance = true);

    bool IsTokenThenAssign(TokenType eType, std::string& strToken, bool bAdvance = true);

    template <typename Allocator, template <typename, typename> class Container>
    bool IsTokenThenAssign(TokenType eType, Container<std::string,Allocator>& oTokenList, bool bAdvance = true);

    bool peek_token_is(TokenType eType);

    bool peek_token_is(const std::string& s);

};


#endif //DIRECTORY_TEST_PARSERHELPER_H
