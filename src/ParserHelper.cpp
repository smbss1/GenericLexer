//
// Created by besseausamuel on 05/09/2020.
//

#include "ParserHelper.h"
#include <cstring>

bool ParserHelper::Init(const std::string& str)
{
    if (!oLexer.Process(str))
        return false;

    oLexer.Begin();
    NextToken();

    return true;
}

Lexer& ParserHelper::GetLexer()
{
    return oLexer;
}

const Lexer& ParserHelper::GetLexer() const
{
    return oLexer;
}

void ParserHelper::NextToken()
{
    oCurrentToken = oLexer.NextToken();
}

const Token& ParserHelper::CurrentToken() const
{
    return oCurrentToken;
}

void ParserHelper::AdvanceToken(const token_advance_mode mode)
{
    if (e_advance == mode)
    {
        NextToken();
    }
}

bool ParserHelper::IsToken(const TokenType eType, bool bAdvance)
{
    if (oCurrentToken.IsType(eType) && bAdvance) {
        NextToken();
        return (true);
    }
    return (false);
}

bool ParserHelper::IsToken(const TokenType eType, const std::string& strValue, bool bAdvance)
{
    if (oCurrentToken.IsType(eType) && oLexer.TokenMatch(oCurrentToken, strValue) && bAdvance)
    {
        NextToken();
        return true;
    }
    return false;
}

bool ParserHelper::IsToken(const TokenType eType, const char* strValue, bool bAdvance)
{
    if (oCurrentToken.IsType(eType) && oLexer.TokenMatch(oCurrentToken, strValue) && bAdvance)
    {
        NextToken();
        return true;
    }
    return false;
}

bool ParserHelper::IsTokenThenAssign(const TokenType eType, std::string& strToken, bool bAdvance)
{
    if (oCurrentToken.IsType(eType) && bAdvance)
    {
        strToken = oCurrentToken.GetText();
        NextToken();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool ParserHelper::IsTokenThenAssign(const TokenType eType, Container<std::string,Allocator>& oTokenList, bool bAdvance)
{
    if (oCurrentToken.IsType(eType) && bAdvance)
    {
        oTokenList.push_back(oCurrentToken.GetText());
        NextToken();
        return true;
    }
    return false;
}

bool ParserHelper::peek_token_is(const TokenType eType)
{
    return (oCurrentToken.IsType(eType));
}

bool ParserHelper::peek_token_is(const std::string& s)
{
    return (oLexer.TokenMatch(CurrentToken(), s));
}