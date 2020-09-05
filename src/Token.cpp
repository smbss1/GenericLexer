
#include <iostream>
#include "Lexer.h"
#include "Parser.h"

using namespace std;

Token::Token()
{
    m_iLength = 0;
    m_strText = string("");
    // m_strvText = string_view(m_strText);
    m_cstring = (char *) m_strText.c_str();
}

Token::Token(TokenType type)
{
    m_eType = type;
}

Token::Token(TokenType type, const char* beg, size_t len)
{
    m_eType = type;
    m_iLength = len;
    m_strText = string(beg, m_iLength);
    // m_strvText = string_view(m_strText);
    m_cstring = (char *) m_strText.c_str();
}

Token::Token(TokenType type, const char* beg, const char* end)
{
    m_eType = type;
    m_iLength = distance(beg, end);
    m_strText = string(beg, m_iLength);
    // m_strvText = string_view(m_strText);
    m_cstring = (char *) m_strText.c_str();
}

Token::~Token()
{
}

std::string Token::GetText() const {
    return m_strText;
}

void Token::SetText(std::string& strText)
{
    m_strText.assign(strText);
    // m_strvText = string_view(m_strText);
}

TokenType Token::GetType()
{
    return m_eType;
}

string Token::TypeToString()
{
    switch (m_eType)
    {
    case TokenType::TokenUnknown :
        return "Unknown";
    case TokenType::TokenAlphaNum :
        return "Identifier";
    case TokenType::TokenNumber :
        return "Number";
    case TokenType::TokenString :
        return "String";
    case TokenType::TokenChar :
        return "Char";
    case TokenType::TokenSymbol :
        return "Symbol";
    case TokenType::TokenEof :
        return "Eof";
    default:
        break;
    }
}

void Token::SetType(TokenType type)
{
    m_eType = type;
}

/**
 * @brief Cette méthode compare le type de classe avec le type passé en paramètre et renvoie true si le token correspond au type voulu
 * @param type le type de token voulu
 * @return renvoie true si le token correspond à un des deux types passés en paramètre et false dans le cas contraire
 */
bool Token::IsType(TokenType type)
{
    return m_eType == type;
}

/**
 * @brief Cette méthode compare le type du token avec les deux autres types et renvoie true si le token correspond à un des deux types
 * @param k1 le premier type de token
 * @param k2 le second type de token
 * @return renvoie true si le token correspond à un des deux types passés en paramètre et false dans le cas contraire
 */
bool Token::IsOneTypeOf(TokenType k1, TokenType k2)
{
    return IsType(k1) || IsType(k2);
}

/**
 * @brief Cette méthode renvoie une chaine de caractère du texte du token
 * @return une chaine de caractère
 */
char* Token::GetCString()
{
    return (char *) m_strText.c_str();
}

char* GetBinaryOpStrFromType(int iType)
{
    char* pBinaryOperatorStrings[] = {
        "INVALID_BINARY_OPERATOR",
        "+", "-", "*", "/", "%",
        "<<", ">>",
        "&", "|", "&&", "||",
    };

    return pBinaryOperatorStrings[iType];
}

// int Token::GetBinaryOpTypeFromToken(Lexer& oLexer)
// {
//     int iType = (int) EnodeBinaryOpType::Invalid;

//     for (int i = 1; i < (int) EnodeBinaryOpType::Max; ++i) {
//         if (oLexer.TokenMatch(*this, GetBinaryOpStrFromType(i))) {
//             iType = i;
//             break;
//         }
//     }
//     return iType;
// }
