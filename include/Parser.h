//
// Created by besseausamuel on 05/09/2020.
//

#ifndef DIRECTORY_TEST_PARSER_H
#define DIRECTORY_TEST_PARSER_H

#include <cstring>
#include "TreeNode.h"
#include "ParserHelper.h"

class Parser : public ParserHelper
{

    ASTNode* Expression()
    {
        ASTNode* tnode = Term();
        ASTNode* e1node = Expression1();

        return CreateNode(OperatorPlus, tnode, e1node);
    }

    ASTNode* Expression1()
    {
        ASTNode* tnode;
        ASTNode* e1node;

        if (IsToken(TokenType::TokenSymbol, "+"))
        {
                tnode = Term();
                e1node = Expression1();

                return CreateNode(OperatorPlus, e1node, tnode);
        }
        else if (IsToken(TokenType::TokenSymbol, "-"))
        {
            tnode = Term();
            e1node = Expression1();

            return CreateNode(OperatorMinus, e1node, tnode);
        }

        return CreateNodeNumber(2);
    }

    ASTNode* Term()
    {
        ASTNode* fnode = Factor();
        ASTNode* t1node = Term1();

        return CreateNode(OperatorMul, fnode, t1node);
    }

    ASTNode* Term1()
    {
        ASTNode* fnode;
        ASTNode* t1node;

        if (IsToken(TokenType::TokenSymbol, "*"))
        {
            fnode = Factor();
            t1node = Term1();
            return CreateNode(OperatorMul, t1node, fnode);
        }
        else if(IsToken(TokenType::TokenSymbol, "/"))
        {
            fnode = Factor();
            t1node = Term1();
            return CreateNode(OperatorDiv, t1node, fnode);
        }
        return CreateNodeNumber(1);
    }

    ASTNode* Factor()
    {
        ASTNode* node;
        std::string number;
        if (IsToken(TokenType::TokenSymbol, "("))
        {
            node = Expression();
            IsToken(TokenType::TokenSymbol, ")");
            return node;
        }
        else if(IsToken(TokenType::TokenSymbol, "-"))
        {
            node = Factor();
            return CreateUnaryNode(node);
        }
        else if(IsTokenThenAssign(TokenType::TokenNumber, number))
        {
            double value = atof(number.c_str());
            NextToken();
            return CreateNodeNumber(value);
        }
        else
        {
//            std::stringstream sstr;
//            sstr << "Unexpected token '" << m_crtToken.Symbol << "' at position " << m_Index;
//            throw ParserException(sstr.str(), m_Index);
        }
    }

    ASTNode* CreateNode(ASTNodeType type, ASTNode* left, ASTNode* right)
    {
        ASTNode* node = new ASTNode;
        node->Type = type;
        node->Left = left;
        node->Right = right;

        return node;
    }

    ASTNode* CreateUnaryNode(ASTNode* left)
    {
        ASTNode* node = new ASTNode;
        node->Type = UnaryMinus;
        node->Left = left;
        node->Right = NULL;

        return node;
    }

    ASTNode* CreateNodeNumber(double value)
    {
        ASTNode* node = new ASTNode;
        node->Type = NumberValue;
        node->Value = value;

        return node;
    }

public:
    ASTNode* Parse(const std::string& strText)
    {
        std::string code = "<Whitespace>: [ \t\n\r\b];"
                           "<Number>: [0-9];"
                           "<Mul>: '*';"
                           "<Div>: '/';"
                           "<Plus>: '+';"
                           "<Minus>: '-';"
                            "<Dot>: '.';";
        if (!GetLexer().LoadGrammar(code))
            return NULL;
        if (!Init(strText))
            return NULL;
        return Expression();
    }
};


#endif //DIRECTORY_TEST_PARSER_H
