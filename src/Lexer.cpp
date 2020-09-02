#include <iostream>
#include <vector>
#include <cstring>
#include "Lexer.h"

using namespace std;

Lexer::Lexer() { }
Lexer::~Lexer() { }

Lexer::Lexer(char* strText)
{
	m_strBegin = strText;
}

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
    static char *symbols[] = {
        "(", ")", "[", "]",
        "{", "}", ";",
        "::", "->", ":",
        "*", "+", "-",
        "/", "<<", ">>", ",","!"
    };
    for (; *m_strCurrent; m_strCurrent++)
        if (!IsSymbol(*m_strCurrent))
            break;
    for (int k = 0; k < (sizeof(symbols) / sizeof(symbols[0])); ++k) {
        int len = strlen(symbols[k]);
        if (!strncmp(symbols[k], start, len)) {
            m_strCurrent = (char *) start + len;
            return (Token(TokenType::TokenSymbol, start, len));
        }
    }
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
Token Lexer::GetToken()
{
    m_strCurrent = (char *) m_strBegin;
    int line = SkipWhitespace(&m_strCurrent);
    Token token;

    if (*m_strCurrent == '\0') {
		return (Token(TokenType::TokenEof, m_strCurrent, 1));
	}
    if (*m_strCurrent == '@') {
        return ParseTag();
    }
    if (IsSymbol(*m_strCurrent))
        return ParseSymbol();
    if (*m_strCurrent == '"') {
        return ParseString();
    } else {
        if (isalpha(*m_strCurrent)) {
        	return ParseAlpha();
		} else if (isdigit(*m_strCurrent)) {
			return ParseNumber();
		} else {
            return (Token(TokenType::TokenUnknown));
		}
	}
    token.m_iLinesTraversed = line;
    return (token);
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
	Token token = GetToken();
    m_strBegin = m_strCurrent;
    //this->m_iLines += token.iLinesTraversed;
    return token;
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

bool Lexer::TokenMatch(Token oToken, string strString)
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
        m_strBegin = m_strCurrent;
        //Lexer->lines += token.lines_traversed;
        result = true;
        if (pToken) {
            pToken->m_iLength = oToken.m_iLength;
            pToken->SetText(string(oToken.GetText()));
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
        m_strBegin = m_strCurrent;
        //Lexer->lines += token.lines_traversed;
        if (pToken) {
            pToken->m_iLength = oToken.m_iLength;
            pToken->SetText(string(oToken.GetText()));
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

int Lexer::SkipComment(char** c, bool long_comment)
{
    int line = 0;
    (*c)++;
    (*c)++;
    if (long_comment) {
        while (**c != '*' && *(*c + 1) != '/' && **c) {
            if (**c == '\n' || **c == '\0')
                ++line;
            (*c)++;
        }
        (*c)++;
    } else {
        while (**c != '\n' && **c) {
            (*c)++;
        }
        ++line;
    }

    return (line);
}

int Lexer::SkipWhitespace(char** c)
{
    int line = 0;

    while (IsWhitespace(**c) && **c) {
        if (**c == '\n' || **c == '\0')
            ++line;
        if (**c == '/' && *(*c + 1) == '/') {
            line += SkipComment(c, false);
            continue;
        }

        if (**c == '/' && *(*c + 1) == '*') {
            line += SkipComment(c, true);
            continue;
        }
        (*c)++;
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

bool Lexer::IsToken(const TokenType eType, const string strValue, bool bAdvance)
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

bool Lexer::Process(const string strText)
{
    base_itr_ = strText.data();
    s_itr_    = strText.data();
    s_end_    = strText.data() + strText.size();
	m_strBegin = strText.c_str();

    // eof_token_.set_operator(token_t::e_eof,s_end_,s_end_,base_itr_);
    token_list_.clear();

    while (!PeekToken().IsType(TokenType::TokenEof))
    {
        // scan_token();

        // if (token_list_.empty())
        //     return true;
        // else if (token_list_.back().is_error())
        // {
        //     return false;
        // }
        token_list_.push_back(NextToken());
    }
    return true;
}

void Lexer::Begin()
{
    token_itr_ = token_list_.begin();
    store_token_itr_ = token_list_.begin();
}

bool Lexer::IsEnd(const char* strItr)
{
    return (s_end_ == strItr);
}