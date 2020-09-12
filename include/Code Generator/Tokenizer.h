
#ifndef TOKENIZER_H_INCLUDED
#define TOKENIZER_H_INCLUDED

#include <string>
#include <string_view>

class Tokenizer;

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
    // std::string_view m_strvText;
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
    void SetText(std::string lexeme);

    TokenType GetType();
    void SetType(TokenType type);
    bool IsType(TokenType type);
    bool IsOneTypeOf(TokenType k1, TokenType k2);
    int GetBinaryOpTypeFromToken(Tokenizer& oTokenizer);
};

class Tokenizer
{
private:
    int IsSymbol(int c);
    Token ParseString();
    Token ParseAlpha();
    Token ParseNumber();
    Token ParseTag();
    bool TagCheck(char **cCurrent, char t, Token& token);
    Token ParseSymbol();

public:
    const char* m_strBegin = nullptr;
    char* m_strFilename;
    char* m_strCurrent = nullptr;
    int m_iLines;
    int m_iError;

public:
	Tokenizer();
    Tokenizer(char* strFileContent, char* strFilename);
	~Tokenizer();
    Token GetToken();
    Token PeekToken();
    Token NextToken();
    bool RequireToken(char* strText, Token* pToken);
    bool RequireTokenType(TokenType type, Token* pToken);
    bool TokenMatch(Token oToken, char* string);
};

#endif