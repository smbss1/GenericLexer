#include <iostream>
#include <vector>
#include <cstring>
#include "Lexer.h"

using namespace std;

Lexer::Lexer() { }
Lexer::~Lexer() { }

// Lexer::Lexer(char* strText)
// {
// 	m_strBegin = strText;
// }

// ---------------------------------------------------------------

/**
 * @brief Cette fonction permet de parser un mot alphanumérique
 * Exemple: myVar ou My_var ou my_var1
 * @return un token contenant le mot sous forme de string et le type "tokenAlphaNum"
 */
Token Lexer::ParseAlpha()
{
    const char *start = m_strCurrent;

    while (isalpha(*m_strCurrent) || isdigit(*m_strCurrent) || *m_strCurrent == '_') {
        m_strCurrent++;
    }
    return (Token(TokenType::TokenAlphaNum, start, m_strCurrent));
}

/**
 * @brief Cette fonction permet de parser un nombre
 * @return un token contenant le nombre sous forme de string et le type "tokenNumber"
 */
Token Lexer::ParseNumber()
{
    const char *start = m_strCurrent;

    while (isdigit(*m_strCurrent))
        m_strCurrent++;
    return (Token(TokenType::TokenNumber, start, m_strCurrent));
}

/**
 * @brief Cette fonction permet de parser un symbole
 * @return un token contenant le symbol sous forme de string et le type "tokenSymbol"
 * @note voici les symboles actuellement supportés: "(", ")", "[", "]", "{", "}",
 *                                                  ";", "::", "->", ":", "*",
 *                                                  "+", "-", "/", "<<", ">>"
 */
Token Lexer::ParseSymbol()
{
    const char *start = m_strCurrent;
    // static char *symbols[] = {
    //     "(", ")", "[", "]",
    //     "{", "}", ";",
    //     "::", "->", ":",
    //     "*", "+", "-",
    //     "/", "<<", ">>", ",","!"
    // };
    
    for (; *m_strCurrent; m_strCurrent++)
        if (isalnum(*m_strCurrent))
            break;
    for (int k = 0; k < m_oSymbols.size(); ++k) {
        int len = m_oSymbols[k].size();
        if (m_oSymbols[k].compare(0, len, start, len) == 0) {
            m_strCurrent = (char *) start + len;
            return (Token(TokenType::TokenSymbol, start, len));
        }
    }
    return (Token(TokenType::TokenUnknown, start, distance(start, m_strCurrent)));
}

/**
 * @brief Cette fonction permet de parser un tag identifiable grâce au '@'
 * @return un token contenant le nom du tag (sans le '@') et le type "tokenTag"
 * @note Un tag peut avoir des paramètres
 */
Token Lexer::ParseTag()
{
    const char *start = ++m_strCurrent;
    while (isalpha(*m_strCurrent) || isdigit(*m_strCurrent) || *m_strCurrent == '_')
        m_strCurrent++;
    return (Token(TokenType::TokenTag, start, m_strCurrent));
}

/**
 * @brief Cette fonction permet de parser une string
 * @return un token contenant la string et le type "tokenString"
 */
Token Lexer::ParseString()
{
    const char *start = m_strCurrent;

    while (*m_strCurrent && *m_strCurrent != '"') {
        if (*m_strCurrent == '\\' && m_strCurrent[1])
            ++m_strCurrent;
        ++m_strCurrent;
    }
    if (*m_strCurrent == '"')
        ++m_strCurrent;
    return (Token(TokenType::TokenString, start, m_strCurrent));
}

/**
 * @brief Cette fonction permet de vérifier si le caractère est un symbole ou pas
 * @param c C'est le caractère qui sera vérifié
 * @return true si le caractère est un symbole et false dans le cas contraire
 */
int Lexer::IsSymbol(int c)
{
    return (c == '~' || c == '`' || c == '!' ||
            c == '#' || c == '$' || c == '%' ||
            c == '^' || c == '&' || c == '*' ||
            c == '(' || c == ')' || c == '-' ||
            c == '+' || c == '=' || c == '{' ||
            c == '}' || c == '[' || c == ']' ||
            c == ':' || c == ';' || c == '<' ||
            c == '>' || c == ',' || c == '.' || c == '?' || c == '/');
}

// ---------------------------------------------------------

/**
 * @brief Cette fonction renvoie un token avec son type approprié
 * @return un token avec le type correspondant au mot parsé
 */
void Lexer::ScanToken()
{
    // m_strCurrent = (char *) m_strBegin;
    int line = SkipWhitespace();
    // Token token;

    if (IsEnd(m_strCurrent)) {
        return;
	}
    if (!isalnum(*m_strCurrent)) {
        oTokenList.push_back(ParseSymbol());
    }
    else if (*m_strCurrent == '"') {
        oTokenList.push_back(ParseString());
    } else {
        if (isalpha(*m_strCurrent)) {
        	oTokenList.push_back(ParseAlpha());
		} else if (isdigit(*m_strCurrent)) {
			oTokenList.push_back(ParseNumber());
		} else {
            oTokenList.push_back(Token(TokenType::TokenUnknown));
		}
	}
    // token.m_iLinesTraversed = line;
    // oTokenList.push_back(token);
}

Token Lexer::GetToken()
{

}

/**
 * @brief Cette fonction permet de récuperer le token actuel sans passer au suivant
 * @return un token avec le type correspondant au mot parsé
 */
Token Lexer::PeekToken()
{
    return GetToken();
}

/**
 * @brief Cette fonction permet de récuperer le token actuel et de passer au suivant
 * @return un token avec le type correspondant au mot parsé
 */
Token Lexer::NextToken()
{
    if (oTokenList.end() != oTokenIterator)
    {
        return *oTokenIterator++;
    }
    else
        return oEofToken;
}

/**
 * @brief Cette fonction permet de vérifier que le token correspond à la string passé en paramètre
 * @param oToken Le token qui sera comparer au texte
 * @param string Le texte qui sera comparer au token
 * @return true si le token et la string correspondent et false dans le cas contraire
 */
bool Lexer::TokenMatch(Token oToken, char* string)
{
    return (!strncmp(oToken.GetCString(), string, oToken.m_iLength) && string[oToken.m_iLength] == 0);
}

bool Lexer::TokenMatch(Token oToken, const string& strString)
{
    return (oToken.GetText() == strString);
}

/**
 * @brief Cette fonction permet de vérifier que le token correspond à la string passé en paramètre
 * @param strText Le texte qui sera comparer au prochain token
 * @param pToken si non null et que la fonction retourne true, le prochain token sera stocké dans cette variable
 * @return true si le prochain token et le texte passé en paramètre correspondent et false dans le cas contraire
 */
bool Lexer::RequireToken(char* strText, Token* pToken)
{
    Token oToken = GetToken();
    bool result = false;

    if (TokenMatch(oToken, strText)) {
        // m_strBegin = m_strCurrent;
        //Lexer->lines += token.lines_traversed;
        result = true;
        if (pToken) {
            pToken->m_iLength = oToken.m_iLength;
            string strTemp = oToken.GetText();
            pToken->SetText(strTemp);
            pToken->SetType(oToken.GetType());
        }
    }
    return (result);
}

/**
 * @brief Cette fonction permet de vérifier que le token correspond à la string passé en paramètre
 * @param type Le type qui sera comparer au prochain token
 * @param pToken si non null et que la fonction retourne true, le prochain token sera stocké dans cette variable
 * @return true si le prochain token et le type correspondent et false dans le cas contraire
 */
bool Lexer::RequireTokenType(TokenType eType, Token* pToken)
{
    Token oToken = GetToken();

    if (oToken.IsType(eType)) {
        // m_strBegin = m_strCurrent;
        //Lexer->lines += token.lines_traversed;
        if (pToken) {
            pToken->m_iLength = oToken.m_iLength;
            string strTemp = oToken.GetText();
            pToken->SetText(strTemp);
            pToken->SetType(oToken.GetType());
        }
        return (true);
    }
    return (false);
}

bool Lexer::IsWhitespace(char c)
{
    return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '/');
}

int Lexer::SkipComment(bool long_comment)
{
    int line = 0;
    m_strCurrent += 2;
    if (long_comment) {
        while (*m_strCurrent != '*' && *(m_strCurrent + 1) != '/' && *m_strCurrent) {
            if (*m_strCurrent == '\n' || *m_strCurrent == '\0')
                ++line;
            m_strCurrent++;
        }
        m_strCurrent++;
    } else {
        while (*m_strCurrent != '\n' && *m_strCurrent) {
            m_strCurrent++;
        }
        ++line;
    }

    return (line);
}

int Lexer::SkipWhitespace()
{
    int line = 0;

    while (IsWhitespace(*m_strCurrent) && *m_strCurrent) {
        if (*m_strCurrent == '\n' || *m_strCurrent == '\0')
            ++line;
        // if (*m_strCurrent == '/' && *(m_strCurrent + 1) == '/') {
        //     line += SkipComment(false);
        //     continue;
        // }

        // if (*m_strCurrent == '/' && *(m_strCurrent + 1) == '*') {
        //     line += SkipComment(true);
        //     continue;
        // }
        m_strCurrent++;
    }
    return (line);
}

bool Lexer::IsToken(const TokenType eType, bool bAdvance)
{
    Token oToken = GetToken();

    if (oToken.IsType(eType) && bAdvance) {
        NextToken();
        return (true);
    }
    return (false);
}

bool Lexer::IsToken(const TokenType eType, const string& strValue, bool bAdvance)
{
    Token oToken = GetToken();
    if (oToken.IsType(eType) && TokenMatch(oToken, strValue) && bAdvance)
    {
        NextToken();
        return true;
    }
    return false;
}

bool Lexer::IsTokenThenAssign(const TokenType eType, string& strToken, bool bAdvance)
{
    Token oToken = GetToken();
    if (oToken.IsType(eType) && bAdvance)
    {
        strToken = oToken.GetText();
        NextToken();
        return true;
    }
    return false;
}

template <typename Allocator, template <typename, typename> class Container>
bool Lexer::IsTokenThenAssign(const TokenType eType, Container<string,Allocator>& oTokenList, bool bAdvance)
{
    Token oToken = GetToken();
    if (oToken.IsType(eType) && bAdvance)
    {
        oTokenList.push_back(oToken.GetText());
        NextToken();
        return true;
    }
    return false;
}

bool Lexer::Process(const string& strText)
{
    strBaseIterator = strText.data();
    m_strCurrent    = strText.data();
    strEnd    = strText.data() + strText.size();
	// m_strBegin = strText.c_str();

    oEofToken.m_strText.assign(strEnd, strEnd);
    // if (strBaseIterator)
    //     position = std::distance(strBaseIterator, strEnd);
    oTokenList.clear();

    while (!IsEnd(m_strCurrent))
    {
        ScanToken();

        if (oTokenList.empty())
            return true;
        else if (oTokenList.back().IsError())
            return false;
    }
    return true;
}

void Lexer::Begin()
{
    oTokenIterator = oTokenList.begin();
    oStoreTokenIterator = oTokenList.begin();
}

bool Lexer::IsEnd(const char* strItr)
{
    return (strItr == strEnd);
}

void Lexer::Store()
{
    oStoreTokenIterator = oTokenIterator;
}

void Lexer::Restore()
{
    oTokenIterator = oStoreTokenIterator;
}

Token& Lexer::PeekNextToken()
{
    if (oTokenList.end() != oTokenIterator)
    {
        return *oTokenIterator;
    }
    else
        return oEofToken;
}

bool Lexer::Empty() const
{
    return oTokenList.empty();
}

std::size_t Lexer::Size() const
{
    return oTokenList.size();
}

void Lexer::Clear()
{
    strBaseIterator = 0;
    m_strCurrent = 0;
    strEnd = 0;
    oTokenList.clear();
    oTokenIterator = oTokenList.end();
    oStoreTokenIterator = oTokenList.end();
}

bool Lexer::Finished() const
{
    return (oTokenList.end() == oTokenIterator);
}

void Lexer::AddSymbol(string& oSymbol)
{
    m_oSymbols.push_back(oSymbol);
}

void Lexer::AddSymbol(const char* oSymbol)
{
    m_oSymbols.push_back(oSymbol);
}

void Lexer::AddSymbols(vector<string> oSymbols)
{
    for (auto symbol : oSymbols)
    {
        m_oSymbols.push_back(symbol);
    }
}