/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#pragma once
#include <vector>

class Token
{
public:
    std::string m_strText;
    std::string m_strType;
    int m_iLength;
    int m_iLinesTraversed;

    Token();
    Token(const char* beg, std::size_t len);
    Token(const char* beg, const char* end);
    Token(std::string type, const char* beg, std::size_t len);
    Token(std::string type, const char* beg, std::size_t len, std::size_t lLines);
    Token(std::string type, const char* beg, const char* end);
    ~Token();

    std::string GetText() const;
    char* GetCString();
    void SetText(std::string& strText);

    // int GetBinaryOpTypeFromToken(Lexer& oLexer);

    inline bool IsError() const
    {
        return (false);
        // return (m_eType == TokenType::TokenUnknown);
    }
};