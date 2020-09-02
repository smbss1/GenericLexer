
#ifndef oLexerH_INCLUDED
#define oLexerH_INCLUDED

#include <string>
#include <deque>

class Lexer;

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
private:
    std::string m_strText;
    TokenType m_eType;
    char *m_cstring;

public:
    int m_iLength;
    int m_iLinesTraversed;

    Token();
    Token(TokenType type);
    Token(TokenType type, const char* beg, std::size_t len);
    Token(TokenType type, const char* beg, const char* end);
    ~Token();

    std::string GetText();
    char* GetCString();
    void SetText(std::string strText);

    TokenType GetType();
    void SetType(TokenType type);
    bool IsType(TokenType type);
    bool IsOneTypeOf(TokenType k1, TokenType k2);
    int GetBinaryOpTypeFromToken(Lexer& oLexer);
};

class Lexer
{
    typedef std::deque<Token> token_list_t;
    typedef std::deque<Token>::iterator token_list_itr_t;

public:
    std::deque<Token> token_list_;
    token_list_itr_t token_itr_;
    token_list_itr_t store_token_itr_;
    Token eof_token_;
    const char* base_itr_;
    const char* s_itr_;
    const char* s_end_;

private:
    int IsSymbol(int c);
    Token ParseString();
    Token ParseAlpha();
    Token ParseNumber();
    Token ParseTag();
    bool TagCheck(char **cCurrent, char t, Token& token);
    Token ParseSymbol();
    bool IsWhitespace(char c);
    int SkipComment(char** c, bool long_comment);
    int SkipWhitespace(char** c);

public:
    const char* m_strBegin = nullptr;
    char* m_strFilename;
    char* m_strCurrent = nullptr;
    int m_iLines;
    int m_iError;

public:
	Lexer();
    Lexer(char* strText);
	~Lexer();
    void Begin();
    bool IsEnd(const char* strItr);
    Token GetToken();
    Token PeekToken();
    Token NextToken();
    bool Process(const std::string strText);
    bool RequireToken(char* strText, Token* pToken);
    bool RequireTokenType(TokenType eType, Token* pToken);
    bool TokenMatch(Token oToken, char* string);
    bool TokenMatch(Token oToken, std::string strString);
    bool IsToken(const TokenType eType, bool bAdvance = true);
    bool IsToken(const TokenType eType, const std::string strValue, bool bAdvance = true);
    bool IsTokenThenAssign(const TokenType eType, std::string& strToken, bool bAdvance = true);
    template <typename Allocator, template <typename, typename> class Container>
    bool IsTokenThenAssign(const TokenType eType, Container<std::string, Allocator>& oTokenList, bool bAdvance = true);
};

class ParserHelper
{
private:

    Lexer oLexer;
    Token oCurrentToken;

public:

    inline bool Init(const std::string str)
    {
        if (!oLexer.Process(str))
        {
            return false;
        }

        oLexer.Begin();

        NextToken();

        return true;
    }

    inline Lexer& GetLexer()
    {
        return oLexer;
    }

    inline const Lexer& GetLexer() const
    {
        return oLexer;
    }

    inline void NextToken()
    {
        oCurrentToken = oLexer.NextToken();
    }

    inline const Token& CurrentToken() const
    {
        return oCurrentToken;
    }

    enum token_advance_mode
    {
        e_hold    = 0,
        e_advance = 1
    };

    inline void AdvanceToken(const token_advance_mode mode)
    {
        if (e_advance == mode)
        {
            NextToken();
        }
    }

    bool IsToken(const TokenType eType, bool bAdvance)
    {
        if (oCurrentToken.IsType(eType) && bAdvance) {
            NextToken();
            return (true);
        }
        return (false);
    }

    bool IsToken(const TokenType eType, const string strValue, bool bAdvance)
    {
        if (oCurrentToken.IsType(eType) && oLexer.TokenMatch(oCurrentToken, strValue) && bAdvance)
        {
            NextToken();
            return true;
        }
        return false;
    }

    bool IsTokenThenAssign(const TokenType eType, string& strToken, bool bAdvance)
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
    bool IsTokenThenAssign(const TokenType eType, Container<string,Allocator>& oTokenList, bool bAdvance)
    {
        if (oCurrentToken.IsType(eType) && bAdvance)
        {
            oTokenList.push_back(oCurrentToken.GetText());
            NextToken();
            return true;
        }
        return false;
    }

    inline bool peek_token_is(const TokenType eType)
    {
        return (oLexer.PeekToken().IsType(eType));
    }

    inline bool peek_token_is(const std::string& s)
    {
        return (oLexer.TokenMatch(oLexer.PeekToken(), s));
    }
};

#endif