//
// Created by besseausamuel on 04/09/2020.
//

#include "GrammarParser.h"


/*struct parse_function_definition_impl : public lexertk::parser_helper
{
    *//*
       Structure: function <name> (v0,v1,...,vn) { expression }
    *//*
    bool process(std::string& func_def, function_definition& fd)
    {
        if (!init(func_def))
            return false;

        if (!token_is(token_t::e_symbol,"function"))
            return false;

        if (!token_is_then_assign(token_t::e_symbol,fd.name))
            return false;

        if (!token_is(token_t::e_lbracket))
            return false;

        if (!token_is(token_t::e_rbracket))
        {
            std::vector<std::string> var_list;

            for ( ; ; )
            {
                // (x,y,z,....w)
                if (!token_is_then_assign(token_t::e_symbol,var_list))
                    return false;

                if (token_is(token_t::e_rbracket))
                    break;

                if (!token_is(token_t::e_comma))
                    return false;
            }

            var_list.swap(fd.var_list);
        }

        std::size_t body_begin = current_token().position;
        std::size_t body_end   = current_token().position;

        int bracket_stack = 0;

        if (!token_is(token_t::e_lcrlbracket,e_hold))
            return false;

        for ( ; ; )
        {
            body_end = current_token().position;

            if (token_is(token_t::e_lcrlbracket))
                bracket_stack++;
            else if (token_is(token_t::e_rcrlbracket))
            {
                if (0 == --bracket_stack)
                    break;
            }
            else
            {
                if (lexer().finished())
                    return false;

                next_token();
            }
        }

        std::size_t size = body_end - body_begin + 1;

        fd.body = func_def.substr(body_begin,size);

        const std::size_t index = body_begin + size;

        if (index < func_def.size())
            func_def = func_def.substr(index,func_def.size() - index);
        else
            func_def = "";

        return true;
    }
};*/

/*bool parse_function_definition(std::string& func_def, function_definition& fd)
{
    parse_function_definition_impl parser;
    return parser.process(func_def,fd);
}*/

bool GrammarParser::Process(const std::string& strText) {
    if (!Init(strText))
        return false;
    std::string name;
    std::string content;
    if (IsToken(TokenType::TokenAlphaNum,"terminal") && IsToken(TokenType::TokenSymbol, ":")) {
        while (GetLexer().oTokenIterator != GetLexer().oTokenList.end())
        {
            if (!IsToken(TokenType::TokenSymbol, "<"))
                return false;
            if (!IsTokenThenAssign(TokenType::TokenAlphaNum, name))
                return false;
            if (!IsToken(TokenType::TokenSymbol,">"))
                return false;
            if (!IsToken(TokenType::TokenSymbol,":"))
                return false;
            if (IsTokenThenAssign(TokenType::TokenString, content))
                if (content[0] == '[')
                    GetLexer().m_oTerminalNames.insert(std::make_pair(name, Definition(Definition::TerminalType::Terminal, string(content.c_str() + 1, content.size() - 2))));
                else if (content[0] == '\'')
                    GetLexer().m_oTerminalNames.insert(std::make_pair(name, Definition(Definition::TerminalType::Symbol, string(content.c_str() + 1, content.size() - 2))));
        }
    }

    return true;
}
