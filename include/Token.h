/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#pragma once
#include <vector>

enum class TokenType {
    TokenUnknown,

    TokenAlphaNum,
    TokenNumber,
    TokenString,
    TokenChar,
    TokenComment,
    TokenTag,
    TokenSymbol,

    TokenEof,
};

class Token
{
public:
    std::string m_strText;
    TokenType m_eType;
    char *m_cstring;
    int m_iLength;
    int m_iLinesTraversed;

    Token();
    Token(TokenType type);
    Token(TokenType type, const char* beg, std::size_t len);
    Token(TokenType type, const char* beg, const char* end);
    ~Token();

    std::string GetText() const;
    char* GetCString();
    void SetText(std::string& strText);

    TokenType GetType();
    std::string TypeToString();
    void SetType(TokenType type);
    bool IsType(TokenType type);
    bool IsOneTypeOf(TokenType k1, TokenType k2);
    // int GetBinaryOpTypeFromToken(Lexer& oLexer);

    inline bool IsError() const
    {
        return (m_eType == TokenType::TokenUnknown);
    }
};