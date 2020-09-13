
#include <iostream>
#include <utility>

#include "Token.h"
#include "Lexer.h"

using namespace std;

Token::Token()
{
    m_iLength = 0;
    m_strText = string("");
    // m_strvText = string_view(m_strText);
}

Token::Token(const char* beg, size_t len)
{
    m_iLength = len;
    m_strText = string(beg, m_iLength);
    // m_strvText = string_view(m_strText);
}

Token::Token(const char* beg, const char* end)
{
    m_iLength = distance(beg, end);
    m_strText = string(beg, m_iLength);
    // m_strvText = string_view(m_strText);
}

Token::Token(std::string type, const char* beg, size_t len)
{
    m_strType = type;
    m_iLength = len;
    m_strText = string(beg, m_iLength);
    // m_strvText = string_view(m_strText);
}

Token::Token(std::string type, const char *beg, std::size_t len, std::size_t lLines)
{
    m_strType = type;
    m_iLength = len;
    m_strText = string(beg, m_iLength);
    m_iLinesTraversed = lLines;
}

Token::Token(std::string type, const char *beg, const char *end)
{
    m_strType = std::move(type);
    m_iLength = distance(beg, end);
    m_strText = string(beg, m_iLength);
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


/**
 * @brief Cette méthode renvoie une chaine de caractère du texte du token
 * @return une chaine de caractère
 */
char* Token::GetCString()
{
    return (char *) m_strText.c_str();
}
