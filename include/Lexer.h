

#ifndef FOX_LEXER_H_
#define FOX_LEXER_H_

#include <vector>
#include <map>
#include <iostream>
#include <string>
#include <iterator>
#include <regex>
#include "Token.h"


struct LessCompare
{
    inline bool operator()(const std::string& s1, const std::string& s2) const
    {
        const std::size_t length = std::min(s1.size(),s2.size());

        for (std::size_t i = 0; i < length;  ++i)
        {
            if (std::tolower(s1[i]) > std::tolower(s2[i]))
                return false;
            else if (std::tolower(s1[i]) < std::tolower(s2[i]))
                return true;
        }

        return s1.size() < s2.size();
    }
};

struct Area
{
	explicit Area()
	{
		m_cStart = 0;
		m_cEnd = 0;
	}

	explicit Area(char start, char end)
	{
		m_cStart = start;
		m_cEnd = end;
	}
	char m_cStart;
	char m_cEnd;
};

class Lexer
{
public:
    std::vector<Token> oTokenList;
    std::vector<std::string> m_strList; // List of token string value
    std::vector<Token>::iterator oTokenIterator;
    std::vector<Token>::iterator oStoreTokenIterator;
    Token oEofToken;
    const char* strEnd;
    std::vector<std::pair<StringID, std::regex>> m_oAllDefines;
    std::vector<std::pair<StringID, Area>> m_oAreaDefines;
    std::vector<StringID> m_oTrashDefines;

public:
    char* m_strFilename;
    const char* m_strCurrent;
    std::string m_strText;
    int m_iLines;
    int m_iError;
    bool m_bEnableError;

public:
    Lexer();
    ~Lexer();
    void begin();
    Token next_token();
    static bool TokenMatch(Token oToken, const char* string);
    static bool TokenMatch(Token oToken, const std::string& strString);
    void store();
    void restore();
    Token& peek_next_token();
    bool empty() const;
    std::size_t size() const;
    void clear();
    bool is_end() const;
    bool process(const std::string& strText);
    bool process_async(const std::string& strText);

    inline Token& operator[](const std::size_t& lIndex)
    {
        if (lIndex < oTokenList.size())
            return oTokenList[lIndex];
        else
            return oEofToken;
    }

    inline Token operator[](const std::size_t& lIndex) const
    {
        if (lIndex < oTokenList.size())
            return oTokenList[lIndex];
        else
            return oEofToken;
    }

    void define(const std::string& strId, const std::string& strValue, bool bAddInTrash = false);
	void define(const int id, const std::string& strRegex, bool bAddInTrash = false);
    void define_area(const std::string& strId, char cStart, char cEnd);
    void define_area(const int id, char cStart, char cEnd);

private:
    int SkipComment(bool long_comment);
};

class HelperInterface
{
public:
    virtual void Init()                     {              }
    virtual void Reset()                    {              }
    virtual bool Result()                   { return true; }
    virtual std::size_t process(Lexer&) { return 0;    }
    virtual ~HelperInterface()             = default;
};

namespace helper
{
    inline void replaceAll(std::string& source, const std::string& from, const std::string& to)
    {
        std::string newString;
        newString.reserve(source.length());  // avoids a few memory allocations

        std::string::size_type lastPos = 0;
        std::string::size_type findPos;

        while(std::string::npos != (findPos = source.find(from, lastPos)))
        {
            newString.append(source, lastPos, findPos - lastPos);
            newString += to;
            lastPos = findPos + from.length();
        }

        // Care for the rest after last occurrence
        newString += source.substr(lastPos);

        source.swap(newString);
    }

    inline void Dump(Lexer& oLexer)
    {
        for (std::size_t i = 0; i < oLexer.size(); ++i)
        {
            Token oToken = oLexer[i];
            std::cout << "Token[" << i << "] " << oToken.m_oType << " --> '" << oToken.GetText() << "'" << std::endl;
        }
    }

    inline bool TokenIs(Token oToken, const std::string& eType, const char* strValue)
    {
        return oToken.m_oType == eType && Lexer::TokenMatch(oToken, strValue);
    }
}

#endif