#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Lexer.h"
#include "GrammarParser.h"

using namespace std;

Lexer::Lexer() : oDfa()
{
    // oDfa.AddState(DFAState(true, "sc"));
//    oDfa.AddState(TokenType::TK_PARAN_OPEN    , true);
//    oDfa.AddState(TokenType::TK_PARAN_CLOSE   , true);
//    oDfa.AddState(TokenType::TK_OP_PLUS       , true);
//    oDfa.AddState(TokenType::TK_OP_TIMES      , true);
//    oDfa.AddState(TokenType::TK_OP_ASSIGNMENT , true);
//    oDfa.AddState(TokenType::TK_WHITESPACE    , true);
//    oDfa.AddState(TokenType::TK_ID            , true);
//    oDfa.AddState(TokenType::TK_INTEGER       , true);
//
//    oDfa.AddTransition(0, '(', TokenType::TK_PARAN_OPEN);
//    oDfa.AddTransition(0, ')', TokenType::TK_PARAN_CLOSE);
//    oDfa.AddTransition(0, '+', TokenType::TK_OP_PLUS);
//    oDfa.AddTransition(0, '*', TokenType::TK_OP_TIMES);
//    oDfa.AddTransition(0, '=', TokenType::TK_OP_ASSIGNMENT);
//
//    oDfa.AddTransition(0, ' ', TokenType::TK_WHITESPACE);
//    oDfa.AddTransition(TokenType::TK_WHITESPACE, ' ', TokenType::TK_WHITESPACE);
//
//    for(char c='a'; c<='z'; c++)
//    {
//        oDfa.AddTransition(0, c, TokenType::TK_ID);
//        oDfa.AddTransition(TokenType::TK_ID, c, TokenType::TK_ID);
//    }
//
//    for(char c='0'; c<='9'; c++)
//    {
//        oDfa.AddTransition(TokenType::TK_ID, c, TokenType::TK_ID);
//    }
//
//    for(char c='0'; c<='9'; c++)
//    {
//        oDfa.AddTransition(0, c, TokenType::TK_INTEGER);
//        oDfa.AddTransition(TokenType::TK_INTEGER, c, TokenType::TK_INTEGER);
//    }
}
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
    while (std::find(m_oIdentifierCharacters.begin(), m_oIdentifierCharacters.end(), *m_strCurrent) != m_oIdentifierCharacters.end())
        m_strCurrent++;
    return (Token(TokenType::TokenAlphaNum, start, m_strCurrent));
}

/**
 * @brief Cette fonction permet de parser un nombre
 * @return un token contenant le nombre sous forme de string et le type "tokenNumber"
 */
Token Lexer::ParseNumber()
{
    const char *start = m_strCurrent;

    while (std::find(m_oNumbers.begin(), m_oNumbers.end(), *m_strCurrent) != m_oNumbers.end())
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
 * @brief Cette fonction permet de parser une string
 * @return un token contenant la string et le type "tokenString"
 */
Token Lexer::ParseString()
{
    const char *strFirstArea = m_strCurrent;
    // const char *start = ++m_strCurrent;
    const char *start = m_strCurrent++;
//    std::map<std::string, std::string>::const_iterator it;
//    it = std::find_if(m_oAreas.begin(), m_oAreas.end(), std::bind2nd(IsInA(), *strFirstArea));
//
//    while (*m_strCurrent != it->second[1] && *m_strCurrent) {
//        if (*m_strCurrent == '\\' && m_strCurrent[1])
//            ++m_strCurrent;
//        ++m_strCurrent;
//    }
//
//    // Token oToken = Token(TokenType::TokenString, start, m_strCurrent);
//    if (*m_strCurrent == it->second[1]) {
//        ++m_strCurrent;
//    }
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

auto MapContain(std::map<std::string, std::string>& oMap, char c)
{
    for (auto it = oMap.begin(); it != oMap.end(); it++)
        for (auto& ch : it->second)
            if (ch == c)
                return it;
    return oMap.end();
}

/**
 * @brief Cette fonction renvoie un token avec son type approprié
 * @return un token avec le type correspondant au mot parsé
 */
void Lexer::ScanToken()
{
    // int line = SkipWhitespace();
    // Token token;

//    if (IsEnd(m_strCurrent))
//        return;

//    std::string lexeme;
//    oDfa.Reset();
//    m_strCurrent += '$';
//    for(auto i = m_strText.begin(); i < m_strText.end()-1; i++)
//    {
//        lexeme += *i;
//        // dfa.Input(*i);
//        char next = *(i + 1);
//        if(oDfa.MakeNextTransition(*i) && !oDfa.MakeNextTransition(next))
//        {
//            Token tk;
//            tk.m_strText = lexeme;
//            tk.m_strType = oDfa.GetStateName(oDfa.m_iPreviousStateID);
//            if (oDfa.GetStateName(oDfa.m_iPreviousStateID) != "WHITESPACE")
//                oTokenList.push_back(tk);
//            lexeme.clear();
//            oDfa.Reset();
//        }
//    }
//    Token eos;
//    eos.m_strType = "Eof";
//    oTokenList.push_back(eos);

    // std::vector<std::pair<char, char>>::const_iterator itArea = std::find_if(m_oAreas.begin(), m_oAreas.end(), std::bind2nd(IsInArea(), *m_strCurrent));
//    auto itDefine = MapContain(m_oAllDefines, *m_strCurrent);
//    std::cout << itDefine->first << std::endl;
//    // Si on trouve le bon type
//    if (itDefine != m_oAllDefines.end())
//    {
//        const char *start = m_strCurrent;
//        // std::vector<std::pair<std::string, std::string>>::const_iterator it = std::find_if(m_oAllDefines.begin(), m_oAllDefines.end(), std::bind2nd(IsInDefine(), *m_strCurrent));
//        while (itDefine->second.find(*m_strCurrent) != std::string::npos)
//            m_strCurrent++;
//        oTokenList.emplace_back(itDefine->first, start, m_strCurrent);
//    }

//    if (isalpha(*m_strCurrent)) {
//        oTokenList.push_back(ParseAlpha());
//    } else if (isdigit(*m_strCurrent)) {
//        oTokenList.push_back(ParseNumber());
//    } else {
//        if (itArea != m_oAreas.end() && *m_strCurrent == itArea->first)
//            oTokenList.push_back(ParseString());
//        else if (!isalnum(*m_strCurrent))
//            oTokenList.push_back(ParseSymbol());
//        else
//            oTokenList.emplace_back(TokenType::TokenUnknown);
//    }

    // token.m_iLinesTraversed = line;
    // oTokenList.push_back(token);
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
bool Lexer::TokenMatch(Token oToken, const char* string)
{
    return (!strncmp(oToken.GetCString(), string, oToken.m_iLength) && string[oToken.m_iLength] == 0);
}

bool Lexer::TokenMatch(Token oToken, const string& strString)
{
    return (oToken.GetText() == strString);
}

bool Lexer::IsWhitespace(char c)
{
    // return (c == ' ' || c == '\n' || c == '\t' || c == '\r' || c == '/');
    return std::find(m_oWhitespaces.begin(), m_oWhitespaces.end(), c) != m_oWhitespaces.end();
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

bool Lexer::Process(const string& strText)
{
    m_strText    = strText;
    oTokenList.clear();

    if (strText.empty())
        return false;

//    while (!IsEnd(m_strCurrent))
//    {
//        ScanToken();
//
//        if (oTokenList.empty())
//            return true;
//        else if (oTokenList.back().IsError())
//            return false;
//    }
    std::string lexeme;
    oDfa.Reset();
    m_strText += '$';
    for(auto itChar = m_strText.begin(); itChar < m_strText.end() - 1; itChar++)
    {
        lexeme += *itChar;
        char next = *(itChar + 1);
        if(oDfa.MakeNextTransition(*itChar) && !oDfa.MakeNextTransition(next))
        {
            Token tk;
            tk.m_strText = lexeme;
            tk.m_strType = oDfa.GetStateName(oDfa.m_iPreviousStateID);
            if (tk.m_strType != "WHITESPACE") {
                oTokenList.push_back(tk);
            }
            lexeme.clear();
            oDfa.Reset();

            if (oTokenList.back().IsError())
                return false;
        }

    }
    return !oTokenList.empty();
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
    if (!(std::find(m_oSymbols.begin(), m_oSymbols.end(), oSymbol) != m_oSymbols.end()))
        m_oSymbols.push_back(oSymbol);
}

void Lexer::AddSymbol(const char* oSymbol)
{
    if (!(std::find(m_oSymbols.begin(), m_oSymbols.end(), oSymbol) != m_oSymbols.end()))
        m_oSymbols.emplace_back(oSymbol);
}

void Lexer::AddSymbols(const vector<string>& oSymbols)
{
    for (const auto& symbol : oSymbols)
        m_oSymbols.push_back(symbol);
}

void Lexer::AddWhitespace(char cWhitespace)
{
    m_oWhitespaces.push_back(cWhitespace);
}

void Lexer::AddArea(std::pair<char, char> cRange)
{
    // m_oAreas.emplace_back(cRange);
}

void Lexer::AddIdentiferCharacter(const char c)
{
    if (!(std::find(m_oIdentifierCharacters.begin(), m_oIdentifierCharacters.end(), c) != m_oIdentifierCharacters.end()))
        m_oIdentifierCharacters.emplace_back(c);
}

void Lexer::AddIdentiferRange(const char cStart, const char cEnd)
{
    for (char i = cStart; i <= cEnd; i++)
        AddIdentiferCharacter(i);
}

void Lexer::AddNumber(const char c)
{
    if (!(std::find(m_oNumbers.begin(), m_oNumbers.end(), c) != m_oNumbers.end()))
        m_oNumbers.emplace_back(c);
}

void Lexer::AddNumberRange(const char cStart, const char cEnd)
{
    for (char i = cStart; i <= cEnd; i++)
        AddNumber(i);
}

void Lexer::Define(std::string strId, std::string strValue)
{
    // m_oAllDefines.insert(make_pair(strId, strValue));
    if (!oDfa.StateExist(strId))
        oDfa.AddState(DFAState(false, strId));

    int iState = oDfa.GetStateID(strId);

    int iOpenSquareBracketPos = 0;
    bool bStar = false;
    for (int i = 0; i < strValue.size(); i++) {
        if (strValue[i] == '[')
        {
            iOpenSquareBracketPos = i;
            bool bFirstPosition = i == 0;
            while (strValue[i] != ']' && strValue[i])
            {
                char first = strValue[i];
                if (strValue[i + 1] == '-')
                {
                    char last = strValue[i + 2];
                    i += 2;
                    if (bFirstPosition && !bStar) {
                        for (char c = first; c <= last; c++)
                            oDfa.AddTransition(0, c, iState);
                    }
                    if (bStar) {
                        bStar = false;
                        for (char c = first; c <= last; c++)
                            oDfa.AddTransition(iState, c, iState);
                    }
                }
                else if (first == '.') {
                    for (int j = 32; j < 127; j++) {
                        if (j != '$')
                            oDfa.AddTransition(iState, (char) j, iState);
                    }
                }
                else if (first != '[' && first != ']') {
                    if (bFirstPosition)
                        oDfa.AddTransition(0, first, iState);
                    oDfa.AddTransition(iState, first, iState);
                }
                i++;
            }
        }
        else if (strValue[i] == '.') {
            for (int j = 32; j <= 127; j++) {
                oDfa.AddTransition(0, (char) j, iState);
                oDfa.AddTransition(iState, (char) j, iState);
            }
        }
        else if (strValue[i] == '*') {
            bStar = true;
            strValue.erase(strValue.begin() + i);
            if (strValue[i - 1] == ']') {
                i = iOpenSquareBracketPos - 1;
                std::cout << "BStar" << std::endl;
                std::cout << strId << std::endl;
            }
            else
                i--;
        }
        else {
            oDfa.AddTransition(0, strValue[i], iState);
            if (bStar)
                oDfa.AddTransition(iState, strValue[i], iState);
        }
    }
}

void Lexer::DefineArea(const std::string strId, char cStart, char cEnd)
{
    // m_oAreas.insert(make_pair(strId, std::string(cStart, cEnd)));
    if (!oDfa.StateExist(strId))
        oDfa.AddState(DFAState(false, strId));
    int iState = oDfa.GetStateID(strId);
    oDfa.AddTransition(0, cStart, iState);
    oDfa.AddTransition(iState, cStart, iState);
    oDfa.AddTransition(iState, cEnd, iState);
    for (int j = 32; j <= 127; j++) {
        if (j != cStart && j != cEnd)
            oDfa.AddTransition(iState, (char) j, iState);
    }
}

bool Lexer::LoadGrammar(const string& strText)
{
    GrammarParser grammar;

    grammar.GetLexer().AddSymbol("<");
    grammar.GetLexer().AddSymbol(">");
    grammar.GetLexer().AddSymbol("'");
    grammar.GetLexer().AddSymbol(":");
    grammar.GetLexer().AddSymbol("+");
    grammar.GetLexer().AddSymbol("-");
    grammar.GetLexer().AddSymbol("*");
    grammar.GetLexer().AddSymbol(";");
    grammar.GetLexer().AddWhitespace('\n');
    grammar.GetLexer().AddWhitespace(' ');
    grammar.GetLexer().AddWhitespace('\t');
    grammar.GetLexer().AddWhitespace('\r');
    grammar.GetLexer().AddArea(make_pair<char, char>('[', ']'));
    grammar.GetLexer().AddArea(make_pair<char, char>('\'', '\''));
    grammar.GetLexer().AddIdentiferRange('a', 'z');
    grammar.GetLexer().AddIdentiferRange('A', 'Z');
    grammar.GetLexer().AddIdentiferRange('0', '9');
    grammar.GetLexer().AddIdentiferCharacter('_');

    if (!grammar.Process(strText))
        return false;

    m_oTerminalNames = grammar.GetLexer().m_oTerminalNames;
    m_oNonTerminalNames = grammar.GetLexer().m_oNonTerminalNames;

    string whitespaces = grammar.GetLexer().m_oTerminalNames["Whitespace"].m_strValue;
    string areaWord = grammar.GetLexer().m_oTerminalNames["Word"].m_strValue;
    string numbers = grammar.GetLexer().m_oTerminalNames["Number"].m_strValue;

    for (char& whitespace : whitespaces)
        m_oWhitespaces.emplace_back(whitespace);

    for (int i = 0; i < areaWord.size(); i++)
    {
        if (isalnum(areaWord[i]) && areaWord[i + 1] == '-' && isalnum(areaWord[i + 2]))
        {
            AddIdentiferRange(areaWord[i], areaWord[i + 2]);
            i += 2;
        }
        else if (areaWord[i])
            AddIdentiferCharacter(areaWord[i]);
    }

    for (int i = 0; i < numbers.size(); i++)
    {
        if (numbers[i] && numbers[i + 1] == '-' && numbers[i + 2])
        {
            AddNumberRange(numbers[i], numbers[i + 2]);
            i += 2;
        }
        else if (numbers[i])
            AddNumber(numbers[i]);
    }

    for (auto& itTerminal : grammar.GetLexer().m_oTerminalNames)
        if (itTerminal.second.m_eType == Definition::TerminalType::Symbol)
            AddSymbol(itTerminal.second.m_strValue);

    return true;
}
