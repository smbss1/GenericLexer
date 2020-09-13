#include <iostream>
#include <utility>
#include <vector>
#include <cstring>
#include <algorithm>
#include "Lexer.h"
#include "Regex.h"

Lexer::Lexer()
{
    m_iLines = 0;
    m_iError = 0;
    m_strFilename = nullptr;
    m_strCurrent = nullptr;
}

Lexer::~Lexer() { }

// ---------------------------------------------------------------

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

bool Lexer::Process(const string& strText)
{
    m_strText    = strText;
    oTokenList.clear();

    if (strText.empty())
        return false;

    Regex re;
    bool bError = false;
    m_iLines = 1;

    while (!m_strText.empty() && bError != true) {
        int iLen = 0;
        const char* pText;
        bool bFound = false;
        std::vector<std::string>::iterator itTrash;

        for (auto& define : m_oAllDefines) {
            re.Compile(define.second.c_str());
            pText = re.Search(m_strText.c_str(), &iLen);
            if (iLen > 0 && m_strText.find(pText) == 0) {
                itTrash = std::find(m_oTrashDefines.begin(), m_oTrashDefines.end(), define.first);
                if (itTrash == m_oTrashDefines.end())
                    oTokenList.emplace_back(define.first, pText, iLen, m_iLines);
                m_strText.erase(0, iLen);
                bFound = true;

                // std::cout << define.first << " - " << m_iLines << std::endl;

                if (define.first == "New Line")
                    m_iLines++;

                break;
            }
        }
        if (iLen == 1 && !bFound) {
            std::cout << "'" << m_strText[0] << "'" << " didn't Define." << std::endl;
            bError = true;
            oTokenList.clear();
            break;
        }
    }
    m_iLines++;
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

void Lexer::AddArea(std::pair<char, char> cRange)
{
    // m_oAreas.emplace_back(cRange);
}

void Lexer::Define(const std::string& strId, const std::string& strRegex, bool bAddInTrash)
{
    m_oAllDefines.insert(std::make_pair(strId, strRegex));

    if (bAddInTrash)
        m_oTrashDefines.push_back(strId);
}

void Lexer::DefineArea(const std::string strId, char cStart, char cEnd)
{
//    // m_oAreas.insert(make_pair(strId, std::string(cStart, cEnd)));
//    if (!oDfa.StateExist(strId))
//        oDfa.AddState(DFAState(false, strId));
//    int iState = oDfa.GetStateID(strId);
//    oDfa.AddTransition(0, cStart, iState);
//    oDfa.AddTransition(iState, cStart, iState);
//    oDfa.AddTransition(iState, cEnd, iState);
//    for (int j = 32; j <= 127; j++) {
//        if (j != cStart && j != cEnd)
//            oDfa.AddTransition(iState, (char) j, iState);
//    }
}
