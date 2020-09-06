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

//int factor(char **str)
//{
//    int nbr1;
//    int nbr2 = 0;
//    char op;
//
//    nbr1 = my_strtol(*str, str);
//    while (*str[0] == '*' || *str[0] == '/' || *str[0] == '%') {
//        op = *str[0];
//        *str += 1;
//        nbr2 = my_strtol(*str, str);
//        nbr1 = do_op(nbr1, nbr2, op);
//    }
//    return (nbr1);
//}
//
//int summand(char **str)
//{
//    int nbr1;
//    int nbr2 = 0;
//    char op;
//
//    nbr1 = my_strtol(*str, str);
//    while (*str[0] != '\0' && *str[0] != ')') {
//        op = *str[0];
//        *str += 1;
//        if (op == '+' || op == '-')
//            nbr2 = factor(str);
//        else
//            nbr2 = my_strtol(*str, str);
//        nbr1 = do_op(nbr1, nbr2, op);
//    }
//    return (nbr1);
//}

bool GrammarParser::Process(const std::string& strText) {
    if (!Init(strText))
        return false;

    while (GetLexer().oTokenIterator != GetLexer().oTokenList.end())
    {
        std::string strName;
        std::string strNameRef;
        std::string strContent;
        std::vector<Definition> vDefinitions;


        if (!IsToken(TokenType::TokenSymbol, "<"))
            return false;
        if (!IsTokenThenAssign(TokenType::TokenAlphaNum, strName))
            return false;
        if (!IsToken(TokenType::TokenSymbol,">"))
            return false;
        if (!IsToken(TokenType::TokenSymbol,":"))
            return false;
        while (!IsToken(TokenType::TokenSymbol, ";"))
        {
            if (IsToken(TokenType::TokenSymbol, "<")) {
                if (IsTokenThenAssign(TokenType::TokenAlphaNum, strNameRef))
                    if (IsToken(TokenType::TokenSymbol, ">"))
                        vDefinitions.emplace_back(Definition::TerminalType::TerminalRef, strNameRef);
            }
            else if (IsTokenThenAssign(TokenType::TokenString, strContent)) {
                if (!IsToken(TokenType::TokenSymbol, ";")) {
                    if (strContent[0] == '[')
                        vDefinitions.emplace_back(Definition::TerminalType::Terminal,
                                                  string(strContent.c_str() + 1, strContent.size() - 2));
                    else if (strContent[0] == '\'')
                        vDefinitions.emplace_back(Definition::TerminalType::Symbol,
                                                  string(strContent.c_str() + 1, strContent.size() - 2));
                } else {
                    if (strContent[0] == '[') {
                        GetLexer().m_oTerminalNames.insert(std::make_pair(strName, Definition(Definition::TerminalType::Terminal, string(strContent.c_str() + 1, strContent.size() - 2))));
                        break;
                    }
                    else if (strContent[0] == '\'') {
                        GetLexer().m_oTerminalNames.insert(std::make_pair(strName, Definition(Definition::TerminalType::Symbol, string(strContent.c_str() + 1, strContent.size() - 2))));
                        break;
                    }
                }
            }
        }

        if (!vDefinitions.empty())
            GetLexer().m_oNonTerminalNames.insert(std::make_pair(strName, vDefinitions));
    }

    return true;
}
