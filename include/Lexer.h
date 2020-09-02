
#ifndef oLexerH_INCLUDED
#define oLexerH_INCLUDED

#include <string>
#include <set>
#include <vector>

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
public:
    std::string m_strText;
    TokenType m_eType;
    char *m_cstring;
    int m_iLength;
    int m_iLinesTraversed;

    Token();
    Token(TokenType type);
    Token(TokenType type, const char* beg, std::size_t len);
    Token(TokenType type, const char* beg, const char* end);
    ~Token();

    std::string GetText();
    char* GetCString();
    void SetText(std::string& strText);

    TokenType GetType();
    std::string TypeToString();
    void SetType(TokenType type);
    bool IsType(TokenType type);
    bool IsOneTypeOf(TokenType k1, TokenType k2);
    int GetBinaryOpTypeFromToken(Lexer& oLexer);

    inline bool IsError() const
    {
        return (m_eType == TokenType::TokenUnknown);
    }
};

class Lexer
{
    // typedef std::vector<Token> oTokenListt;
    // typedef std::vector<Token>::iterator oTokenListitr_t;

public:
    std::vector<Token> oTokenList;
    std::vector<Token>::iterator oTokenIterator;
    std::vector<Token>::iterator oStoreTokenIterator;
    Token oEofToken;
    const char* strBaseIterator;
    // const char* strIterator;
    const char* strEnd;
    std::vector<std::string> m_oSymbols;

private:
    int IsSymbol(int c);
    Token ParseString();
    Token ParseAlpha();
    Token ParseNumber();
    Token ParseTag();
    bool TagCheck(char **cCurrent, char t, Token& token);
    Token ParseSymbol();
    bool IsWhitespace(char c);
    int SkipComment(bool long_comment);
    int SkipWhitespace();

public:
    // const char* m_strBegin = nullptr;
    char* m_strFilename;
    const char* m_strCurrent;
    int m_iLines;
    int m_iError;

public:
	Lexer();
    // Lexer(char* strText);
	~Lexer();
    void Begin();
    bool IsEnd(const char* strItr);
    void ScanToken();
    Token GetToken();
    Token PeekToken();
    Token NextToken();
    bool Process(const std::string& strText);
    bool RequireToken(char* strText, Token* pToken);
    bool RequireTokenType(TokenType eType, Token* pToken);
    bool TokenMatch(Token oToken, char* string);
    bool TokenMatch(Token oToken, const std::string& strString);
    bool IsToken(const TokenType eType, bool bAdvance = true);
    bool IsToken(const TokenType eType, const std::string& strValue, bool bAdvance = true);
    bool IsTokenThenAssign(const TokenType eType, std::string& strToken, bool bAdvance = true);
    template <typename Allocator, template <typename, typename> class Container>
    bool IsTokenThenAssign(const TokenType eType, Container<std::string, Allocator>& oTokenList, bool bAdvance = true);
    void AddSymbol(std::string& oSymbol);
    void AddSymbol(const char* oSymbol);
    void AddSymbols(std::vector<std::string> oSymbols);
    void Store();
    void Restore();
    Token& PeekNextToken();
    bool Empty() const;
    std::size_t Size() const;
    void Clear();
    bool Finished() const;

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
};

class HelperInterface
{
public:
    virtual void Init()                     {              }
    virtual void Reset()                    {              }
    virtual bool Result()                   { return true; }
    virtual std::size_t Process(Lexer&) { return 0;    }
    virtual ~HelperInterface()             {              }
};

class TokenScanner : public HelperInterface
{
public:

    virtual ~TokenScanner()
    {}

    explicit TokenScanner(const std::size_t& stride) : m_iStride(stride)
    {
        if (stride > 4)
        {
            throw std::invalid_argument("TokenScanner() - Invalid stride value");
        }
    }

    inline std::size_t process(Lexer& oLexer)
    {
        if (!oLexer.oTokenList.empty())
        {
            for (std::size_t i = 0; i < (oLexer.oTokenList.size() - m_iStride + 1); ++i)
            {
                // Token t;
                switch (m_iStride)
                {
                    case 1 :
                            {
                                const Token& ooT0 = oLexer.oTokenList[i];

                                if (!operator()(ooT0)) return i;
                            }
                            break;

                    case 2 :
                            {
                                const Token& ooT0 = oLexer.oTokenList[i    ];
                                const Token& ooT1 = oLexer.oTokenList[i + 1];

                                if (!operator()(ooT0, ooT1)) return i;
                            }
                            break;

                    case 3 :
                            {
                                const Token& ooT0 = oLexer.oTokenList[i    ];
                                const Token& ooT1 = oLexer.oTokenList[i + 1];
                                const Token& oT2 = oLexer.oTokenList[i + 2];

                                if (!operator()(ooT0, ooT1, oT2)) return i;
                            }
                            break;

                    case 4 :
                            {
                                const Token& ooT0 = oLexer.oTokenList[i    ];
                                const Token& ooT1 = oLexer.oTokenList[i + 1];
                                const Token& oT2 = oLexer.oTokenList[i + 2];
                                const Token& oT3 = oLexer.oTokenList[i + 3];

                                if (!operator()(ooT0, ooT1, oT2, oT3)) return i;
                            }
                            break;
                }
            }
        }

        return (oLexer.oTokenList.size() - m_iStride + 1);
    }

    virtual bool operator()(const Token&)
    {
        return false;
    }

    virtual bool operator()(const Token&, const Token&)
    {
        return false;
    }

    virtual bool operator()(const Token&, const Token&, const Token&)
    {
        return false;
    }

    virtual bool operator()(const Token&, const Token&, const Token&, const Token&)
    {
        return false;
    }

private:

    std::size_t m_iStride;
};

class TokenModifier : public HelperInterface
{
public:

    inline std::size_t Process(Lexer& oLexer)
    {
        std::size_t lChanges = 0;

        for (std::size_t i = 0; i < oLexer.oTokenList.size(); ++i)
        {
            if (Modify(oLexer.oTokenList[i]))
                lChanges++;
        }

        return lChanges;
    }

    virtual bool Modify(Token& t) = 0;
};

class TokenInserter : public HelperInterface
{
public:

    explicit TokenInserter(const std::size_t& stride) : m_iStride(stride)
    {
        if (stride > 5)
        {
            throw std::invalid_argument("token_inserter() - Invalid stride value");
        }
    }

    inline std::size_t Process(Lexer& oLexer)
    {
        if (oLexer.oTokenList.empty())
            return 0;

        std::size_t lChanges = 0;

        for (std::size_t i = 0; i < (oLexer.oTokenList.size() - m_iStride + 1); ++i)
        {
            Token oToken;
            int insert_index = -1;

            switch (m_iStride)
            {
                case 1 : insert_index = Insert(oLexer.oTokenList[i], oToken);
                        break;

                case 2 : insert_index = Insert(oLexer.oTokenList[i],oLexer.oTokenList[i + 1], oToken);
                        break;

                case 3 : insert_index = Insert(oLexer.oTokenList[i],oLexer.oTokenList[i + 1],oLexer.oTokenList[i + 2], oToken);
                        break;

                case 4 : insert_index = Insert(oLexer.oTokenList[i],oLexer.oTokenList[i + 1],oLexer.oTokenList[i + 2],oLexer.oTokenList[i + 3], oToken);
                        break;

                case 5 : insert_index = Insert(oLexer.oTokenList[i],oLexer.oTokenList[i + 1],oLexer.oTokenList[i + 2],oLexer.oTokenList[i + 3],oLexer.oTokenList[i + 4], oToken);
                        break;
            }

            if ((insert_index >= 0) && (insert_index <= (static_cast<int>(m_iStride) + 1)))
            {
                oLexer.oTokenList.insert(oLexer.oTokenList.begin() + (i + insert_index), oToken);
                lChanges++;
            }
        }

        return lChanges;
    }

    virtual inline int Insert(const Token&, Token& )
    {
        return -1;
    }

    virtual inline int Insert(const Token&, const Token&, Token&)
    {
        return -1;
    }

    virtual inline int Insert(const Token&, const Token&, const Token&, Token&)
    {
        return -1;
    }

    virtual inline int Insert(const Token&, const Token&, const Token&, const Token&, Token&)
    {
        return -1;
    }

    virtual inline int Insert(const Token&, const Token&, const Token&, const Token&, const Token&, Token&)
    {
        return -1;
    }

private:

    std::size_t m_iStride;
};

class TokenJoiner : public HelperInterface
{
public:

    inline std::size_t Process(Lexer& oLexer)
    {
        if (oLexer.oTokenList.empty())
            return 0;

        std::size_t lChanges = 0;

        for (std::size_t i = 0; i < oLexer.oTokenList.size() - 1; ++i)
        {
            Token oToken;

            if (Join(oLexer.oTokenList[i],oLexer.oTokenList[i + 1], oToken))
            {
                oLexer.oTokenList[i] = oToken;
                oLexer.oTokenList.erase(oLexer.oTokenList.begin() + (i + 1));

                ++lChanges;
            }
        }

        return lChanges;
    }

    virtual bool Join(const Token&, const Token&, Token&) = 0;
};

namespace helper
{

    inline void Dump(Lexer& oLexer)
    {
        for (std::size_t i = 0; i < oLexer.Size(); ++i)
        {
            Token oToken = oLexer[i];
            std::cout << "Token[" << i << "] " << oToken.TypeToString() << " --> '" << oToken.GetText() << "'" << std::endl;
        }
    }

    // class CommutativeInserter : public TokenInserter
    // {
    // public:

    //     CommutativeInserter() : TokenInserter(2)
    //     {}

    //     inline void IgnoreSymbol(const std::string& symbol)
    //     {
    //         oIgnoreSet.insert(symbol);
    //     }

    //     inline int Insert(const Token& oT0, const Token& oT1, Token& oNewToken)
    //     {
    //         oNewToken.m_eType     = token::e_mul;
    //         oNewToken.value    = "*";
    //         oNewToken.position = oT1.position;
    //         bool match         = false;

    //         if (oT0.type == token::e_symbol)
    //         {
    //             if (oIgnoreSet.end() != oIgnoreSet.find(oT0.value))
    //             {
    //                 return -1;
    //             }
    //             else if (!oT0.value.empty() && ('$' == oT0.value[0]))
    //             {
    //                 return -1;
    //             }
    //         }

    //         if (oT1.type == token::e_symbol)
    //         {
    //             if (oIgnoreSet.end() != oIgnoreSet.find(oT1.value))
    //             {
    //                 return -1;
    //             }
    //         }

    //                 if ((oT0.type == token::e_number     ) && (oT1.type == token::e_symbol     )) match = true;
    //         else if ((oT0.type == token::e_number     ) && (oT1.type == token::e_lbracket   )) match = true;
    //         else if ((oT0.type == token::e_number     ) && (oT1.type == token::e_lcrlbracket)) match = true;
    //         else if ((oT0.type == token::e_number     ) && (oT1.type == token::e_lsqrbracket)) match = true;
    //         else if ((oT0.type == token::e_symbol     ) && (oT1.type == token::e_number     )) match = true;
    //         else if ((oT0.type == token::e_rbracket   ) && (oT1.type == token::e_number     )) match = true;
    //         else if ((oT0.type == token::e_rcrlbracket) && (oT1.type == token::e_number     )) match = true;
    //         else if ((oT0.type == token::e_rsqrbracket) && (oT1.type == token::e_number     )) match = true;
    //         else if ((oT0.type == token::e_rbracket   ) && (oT1.type == token::e_symbol     )) match = true;
    //         else if ((oT0.type == token::e_rcrlbracket) && (oT1.type == token::e_symbol     )) match = true;
    //         else if ((oT0.type == token::e_rsqrbracket) && (oT1.type == token::e_symbol     )) match = true;

    //         return (match) ? 1 : -1;
    //     }

    // private:

    //     std::set<std::string,details::ilesscompare> oIgnoreSet;
    // };

    // class OperatorJoiner : public TokenJoiner
    // {
    // public:

    //     inline bool Join(const Token& oT0, const Token& oT1, Token& oToken)
    //     {
    //         //': =' --> ':='
    //         if ((oT0.m_eType == token::e_colon) && (oT1.type == token::e_eq))
    //         {
    //             t.type     = token::e_assign;
    //             t.value    = ":=";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         //'> =' --> '>='
    //         else if ((oT0.type == token::e_gt) && (oT1.type == token::e_eq))
    //         {
    //             t.type     = token::e_gte;
    //             t.value    = ">=";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         //'< =' --> '<='
    //         else if ((oT0.type == token::e_lt) && (oT1.type == token::e_eq))
    //         {
    //             t.type     = token::e_lte;
    //             t.value    = "<=";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         //'= =' --> '=='
    //         else if ((oT0.type == token::e_eq) && (oT1.type == token::e_eq))
    //         {
    //             t.type     = token::e_eq;
    //             t.value    = "==";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         //'! =' --> '!='
    //         else if ((static_cast<char>(oT0.type) == '!') && (oT1.type == token::e_eq))
    //         {
    //             t.type     = token::e_ne;
    //             t.value    = "!=";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         //'< >' --> '<>'
    //         else if ((oT0.type == token::e_lt) && (oT1.type == token::e_gt))
    //         {
    //             t.type     = token::e_ne;
    //             t.value    = "<>";
    //             t.position = oT0.position;

    //             return true;
    //         }
    //         else
    //             return false;
    //         }
    // };

    // class BracketChecker : public TokenScanner
    // {
    // public:

    //     BracketChecker() : TokenScanner(1), state_(true)
    //     {}

    //     bool result()
    //     {
    //         return state_ && stack_.empty();
    //     }

    //     token error_token()
    //     {
    //         return error_token_;
    //     }

    //     void reset()
    //     {
    //         //why? because msvc doesn't support swap properly.
    //         stack_ = std::stack<char>();
    //         state_ = true;
    //         error_token_.clear();
    //     }

    //     bool operator()(const Token& t)
    //     {
    //         if (
    //                 !t.value.empty()                         &&
    //                 (token::e_string != t.type)     &&
    //                 (token::e_symbol != t.type)     &&
    //                 details::is_bracket(t.value[0])
    //             )
    //         {
    //             char c = t.value[0];

    //                 if (t.type == token::e_lbracket)    stack_.push(')');
    //             else if (t.type == token::e_lcrlbracket) stack_.push('}');
    //             else if (t.type == token::e_lsqrbracket) stack_.push(']');
    //             else if (details::is_right_bracket(c))
    //             {
    //                 if (stack_.empty())
    //                 {
    //                     state_ = false;
    //                     error_token_ = t;

    //                     return false;
    //                 }
    //                 else if (c != stack_.top())
    //                 {
    //                     state_ = false;
    //                     error_token_ = t;

    //                     return false;
    //                 }
    //                 else
    //                     stack_.pop();
    //             }
    //         }

    //         return true;
    //     }

    // private:

    //     bool state_;
    //     std::stack<char> stack_;
    //     token error_token_;
    // };

    // class SymbolReplacer : public TokenModifier
    // {
    // private:

    //     typedef std::map<std::string,std::pair<std::string,token::Tokenype>,details::ilesscompare> replace_map_t;

    // public:

    //     bool remove(const std::string& target_symbol)
    //     {
    //     replace_map_t::iterator itr = replace_map_.find(target_symbol);

    //     if (replace_map_.end() == itr)
    //         return false;

    //     replace_map_.erase(itr);

    //     return true;
    //     }

    //     bool add_replace(const std::string& target_symbol,
    //                     const std::string& replace_symbol,
    //                     const token::Tokenype Tokenype = token::e_symbol)
    //     {
    //         replace_map_t::iterator itr = replace_map_.find(target_symbol);

    //         if (replace_map_.end() != itr)
    //         {
    //             return false;
    //         }

    //         replace_map_[target_symbol] = std::make_pair(replace_symbol,Tokenype);

    //         return true;
    //     }

    //     void clear()
    //     {
    //         replace_map_.clear();
    //     }

    // private:

    //     bool Modify(Token& t)
    //     {
    //         if (token::e_symbol == t.type)
    //         {
    //             if (replace_map_.empty())
    //                 return false;

    //             replace_map_t::iterator itr = replace_map_.find(t.value);

    //             if (replace_map_.end() != itr)
    //             {
    //                 t.value = itr->second.first;
    //                 t.type  = itr->second.second;

    //                 return true;
    //             }
    //         }

    //         return false;
    //     }

    //     replace_map_t replace_map_;
    // };

    // class SequenceValidator : public TokenScanner
    // {
    // private:

    //     typedef std::pair<token::Tokenype,token::Tokenype> token_pair_t;
    //     typedef std::set<token_pair_t> set_t;

    // public:

    //     SequenceValidator() : TokenScanner(2)
    //     {
    //     add_invalid(token::e_number,token::e_number);
    //     add_invalid(token::e_string,token::e_string);
    //     add_invalid(token::e_number,token::e_string);
    //     add_invalid(token::e_string,token::e_number);
    //     add_invalid(token::e_string,token::e_colon);
    //     add_invalid(token::e_colon,token::e_string);
    //     add_invalid_seoT1(token::e_assign);
    //     add_invalid_seoT1(token::e_shr   );
    //     add_invalid_seoT1(token::e_shl   );
    //     add_invalid_seoT1(token::e_lte   );
    //     add_invalid_seoT1(token::e_ne    );
    //     add_invalid_seoT1(token::e_gte   );
    //     add_invalid_seoT1(token::e_lt    );
    //     add_invalid_seoT1(token::e_gt    );
    //     add_invalid_seoT1(token::e_eq    );
    //     add_invalid_seoT1(token::e_comma );
    //     add_invalid_seoT1(token::e_add   );
    //     add_invalid_seoT1(token::e_sub   );
    //     add_invalid_seoT1(token::e_div   );
    //     add_invalid_seoT1(token::e_mul   );
    //     add_invalid_seoT1(token::e_mod   );
    //     add_invalid_seoT1(token::e_pow   );
    //     add_invalid_seoT1(token::e_colon );
    //     }

    //     bool result()
    //     {
    //     return error_list_.empty();
    //     }

    //     bool operator()(const Token& oT0, const Token& oT1)
    //     {
    //     set_t::value_type p = std::make_pair(oT0.type,oT1.type);

    //     if (invalid_bracket_check(oT0.type,oT1.type))
    //     {
    //         error_list_.push_back(std::make_pair(oT0,oT1));
    //     }
    //     else if (invalid_comb_.find(p) != invalid_comb_.end())
    //         error_list_.push_back(std::make_pair(oT0,oT1));

    //     return true;
    //     }

    //     std::size_t error_count()
    //     {
    //     return error_list_.size();
    //     }

    //     std::pair<token,token> error(const std::size_t index)
    //     {
    //     if (index < error_list_.size())
    //     {
    //         return error_list_[index];
    //     }
    //     else
    //     {
    //         static const token error_token;
    //         return std::make_pair(error_token,error_token);
    //     }
    //     }

    //     void clear_errors()
    //     {
    //     error_list_.clear();
    //     }

    // private:

    //     void add_invalid(token::Tokenype base, token::Tokenype t)
    //     {
    //     invalid_comb_.insert(std::make_pair(base,t));
    //     }

    //     void add_invalid_seoT1(token::Tokenype t)
    //     {
    //     add_invalid(t,token::e_assign);
    //     add_invalid(t,token::e_shr   );
    //     add_invalid(t,token::e_shl   );
    //     add_invalid(t,token::e_lte   );
    //     add_invalid(t,token::e_ne    );
    //     add_invalid(t,token::e_gte   );
    //     add_invalid(t,token::e_lt    );
    //     add_invalid(t,token::e_gt    );
    //     add_invalid(t,token::e_eq    );
    //     add_invalid(t,token::e_comma );
    //     add_invalid(t,token::e_div   );
    //     add_invalid(t,token::e_mul   );
    //     add_invalid(t,token::e_mod   );
    //     add_invalid(t,token::e_pow   );
    //     add_invalid(t,token::e_colon );
    //     }

    //     bool invalid_bracket_check(token::Tokenype base, token::Tokenype t)
    //     {
    //     if (details::is_right_bracket(static_cast<char>(base)))
    //     {
    //         switch (t)
    //         {
    //             case token::e_string : return true;
    //             case token::e_assign : return true;
    //             default                       : return false;
    //         }
    //     }
    //     else if (details::is_left_bracket(static_cast<char>(base)))
    //     {
    //         if (details::is_right_bracket(static_cast<char>(t)))
    //             return false;
    //         else if (details::is_left_bracket(static_cast<char>(t)))
    //             return false;
    //         else
    //         {
    //             switch (t)
    //             {
    //                 case token::e_number : return false;
    //                 case token::e_symbol : return false;
    //                 case token::e_string : return false;
    //                 case token::e_add    : return false;
    //                 case token::e_sub    : return false;
    //                 case token::e_colon  : return false;
    //                 default                       : return true;
    //             }
    //         }
    //     }
    //     else if (details::is_right_bracket(static_cast<char>(t)))
    //     {
    //         switch (base)
    //         {
    //             case token::e_number : return false;
    //             case token::e_symbol : return false;
    //             case token::e_string : return false;
    //             case token::e_eof    : return false;
    //             case token::e_colon  : return false;
    //             default                       : return true;
    //         }
    //     }
    //     else if (details::is_left_bracket(static_cast<char>(t)))
    //     {
    //         switch (base)
    //         {
    //             case token::e_rbracket    : return true;
    //             case token::e_rsqrbracket : return true;
    //             case token::e_rcrlbracket : return true;
    //             default                            : return false;
    //         }
    //     }

    //     return false;
    //     }

    //     set_t invalid_comb_;
    //     std::deque<std::pair<token,token> > error_list_;

    // };

    // struct helper_assembly
    // {
    //     inline bool register_scanner(TokenScanner* scanner)
    //     {
    //     if (TokenScanner_list.end() != std::find(TokenScanner_list.begin(),
    //                                                 TokenScanner_list.end(),
    //                                                 scanner))
    //     {
    //         return false;
    //     }

    //     TokenScanner_list.push_back(scanner);

    //     return true;
    //     }

    //     inline bool register_modifier(TokenM
    // odifier* modifier)
    //     {
    //     if (TokenM
    // odifier_list.end() != std::find(TokenM
    // odifier_list.begin(),
    //                                                 TokenM
    //                                             odifier_list.end(),
    //                                                 modifier))
    //     {
    //         return false;
    //     }

    //     TokenM
    // odifier_list.push_back(modifier);

    //     return true;
    //     }

    //     inline bool register_joiner(token_joiner* joiner)
    //     {
    //     if (token_joiner_list.end() != std::find(token_joiner_list.begin(),
    //                                                 token_joiner_list.end(),
    //                                                 joiner))
    //     {
    //         return false;
    //     }

    //     token_joiner_list.push_back(joiner);

    //     return true;
    //     }

    //     inline bool register_inserter(token_inserter* inserter)
    //     {
    //     if (token_inserter_list.end() != std::find(token_inserter_list.begin(),
    //                                                 token_inserter_list.end(),
    //                                                 inserter))
    //     {
    //         return false;
    //     }

    //     token_inserter_list.push_back(inserter);

    //     return true;
    //     }

    //     inline bool run_modifiers(Lexer& g)
    //     {
    //     error_TokenM
    // odifier = reinterpret_cast<TokenM
    // odifier*>(0);

    //     for (std::size_t i = 0; i < TokenM
    // odifier_list.size(); ++i)
    //     {
    //         TokenM
    //     odifier& modifier = (*TokenM
    //     odifier_list[i]);

    //         modifier.reset();
    //         modifier.process(g);

    //         if (!modifier.result())
    //         {
    //             error_TokenM
    //         odifier = TokenM
    //         odifier_list[i];

    //             return false;
    //         }
    //     }

    //     return true;
    //     }

    //     inline bool run_joiners(Lexer& g)
    //     {
    //     error_token_joiner = reinterpret_cast<token_joiner*>(0);

    //     for (std::size_t i = 0; i < token_joiner_list.size(); ++i)
    //     {
    //         token_joiner& joiner = (*token_joiner_list[i]);

    //         joiner.reset();
    //         joiner.process(g);

    //         if (!joiner.result())
    //         {
    //             error_token_joiner = token_joiner_list[i];

    //             return false;
    //         }
    //     }

    //     return true;
    //     }

    //     inline bool run_inserters(Lexer& g)
    //     {
    //     error_token_inserter = reinterpret_cast<token_inserter*>(0);

    //     for (std::size_t i = 0; i < token_inserter_list.size(); ++i)
    //     {
    //         token_inserter& inserter = (*token_inserter_list[i]);

    //         inserter.reset();
    //         inserter.process(g);

    //         if (!inserter.result())
    //         {
    //             error_token_inserter = token_inserter_list[i];

    //             return false;
    //         }
    //     }

    //     return true;
    //     }

    //     inline bool run_scanners(Lexer& g)
    //     {
    //     error_TokenScanner = reinterpret_cast<TokenScanner*>(0);

    //     for (std::size_t i = 0; i < TokenScanner_list.size(); ++i)
    //     {
    //         TokenScanner& scanner = (*TokenScanner_list[i]);

    //         scanner.reset();
    //         scanner.process(g);

    //         if (!scanner.result())
    //         {
    //             error_TokenScanner = TokenScanner_list[i];

    //             return false;
    //         }
    //     }

    //     return true;
    //     }

    //     std::deque<TokenScanner*>  TokenScanner_list;
    //     std::deque<TokenM
    // odifier*> TokenM
    // odifier_list;
    //     std::deque<token_joiner*>   token_joiner_list;
    //     std::deque<token_inserter*> token_inserter_list;

    //     TokenScanner*  error_TokenScanner;
    //     TokenM
    // odifier* error_TokenM
    // odifier;
    //     token_joiner*   error_token_joiner;
    //     token_inserter* error_token_inserter;
    // };
}

// --------------------------------------

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

    bool IsToken(const TokenType eType, const std::string strValue, bool bAdvance)
    {
        if (oCurrentToken.IsType(eType) && oLexer.TokenMatch(oCurrentToken, strValue) && bAdvance)
        {
            NextToken();
            return true;
        }
        return false;
    }

    bool IsTokenThenAssign(const TokenType eType, std::string& strToken, bool bAdvance)
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
    bool IsTokenThenAssign(const TokenType eType, Container<std::string,Allocator>& oTokenList, bool bAdvance)
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