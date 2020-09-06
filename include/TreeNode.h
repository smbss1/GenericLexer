/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/
#ifndef DIRECTORY_TEST_TREENODE_H
#define DIRECTORY_TEST_TREENODE_H

#include <iostream>
#include <string>
#include <memory>

using namespace std;

template<typename T>
class TreeNode {

public:
    TreeNode(const T& val) : value(val), m_pParent(NULL), m_pLeft(NULL), m_pRight(NULL) {}
    TreeNode(const T& val, TreeNode<T>* pLeft, TreeNode<T>* pRight) : value(val), m_pParent(NULL), m_pLeft(pLeft), m_pRight(pRight) {}
    TreeNode(const T& val, TreeNode<T>* pParent, TreeNode<T>* pLeft, TreeNode<T>* pRight) : value(val), m_pParent(pParent), m_pLeft(pLeft), m_pRight(pRight) {}
    TreeNode(const T& val, TreeNode<T>* pParent) : value(val), m_pParent(pParent) {}

    ~TreeNode()
    {
//        if (m_pLeft)
//            delete m_pLeft;
//        if (m_pRight)
//            delete m_pRight;
    }

    const T& Get() const
    {
        return(value);
    }

    void Set(const T& val)
    {
        value = val;
    }

    void AddChild(shared_ptr<TreeNode<T>> pChild)
    {
        const T& other = pChild->Get();
        if (other > value)
            if (m_pRight)
                m_pRight->AddChild(pChild);
            else
                m_pRight = pChild;
        else
        if (m_pLeft)
            m_pLeft->AddChild(pChild);
        else
            m_pLeft = pChild;
    }

    const TreeNode<T>* GetLeft()
    {
        return(m_pLeft);
    }

    const TreeNode<T>* GetRight()
    {
        return(m_pRight);
    }

    void Print()
    {
        Print(this);
    }

    void Draw()
    {
        std::string buffer;
        Draw(this, buffer, "", "");
        std::cout << buffer << std::endl;
    }

    void Draw(TreeNode<T>* pRoot, std::string& buffer, const std::string& prefix, const std::string& childrenPrefix)
    {
        if(pRoot != nullptr) {
            buffer.append(prefix);
            buffer.append(pRoot->value);
            buffer.append("\n");
            Draw(pRoot->m_pRight.get(), buffer, childrenPrefix + "├── ", childrenPrefix + "│   ");
            Draw(pRoot->m_pLeft.get(), buffer, childrenPrefix + "└── ", childrenPrefix + "    ");
        }

    }

private:
    void Print(shared_ptr<TreeNode<T>> pRoot)
    {
        if(pRoot != nullptr) {
            std::cout << pRoot->value << std::endl;
            std::cout << "/";
            Print(pRoot->m_pLeft);
            std::cout << "\\";
            Print(pRoot->m_pRight);
        }
    }

//    void Draw(std::string buffer, std::string prefix, std::string childrenPrefix)
//    {
//        buffer.append(prefix);
//        buffer.append(value);
//        buffer.append("\n");
//
//        if (m_pRight) {
//            m_pRight->Draw(buffer, childrenPrefix + "├── ", childrenPrefix + "│   ");
//        } else {
//            m_pLeft->Draw(buffer, childrenPrefix + "└── ", childrenPrefix + "    ");
//        }
//    }

private:
    T value;
    shared_ptr<TreeNode<T>> m_pLeft;
    shared_ptr<TreeNode<T>> m_pRight;
    TreeNode<T>* m_pParent;
};



enum ASTNodeType
{
    Undefined,
    OperatorPlus = '+',
    OperatorMinus = '-',
    OperatorMul = '*',
    OperatorDiv = '/',
    UnaryMinus = 47,
    NumberValue
};

class ASTNode
{
public:
    ASTNodeType Type;
    double      Value;
    ASTNode*    Left;
    ASTNode*    Right;

    ASTNode()
    {
        Type = Undefined;
        Value = 0;
        Left = NULL;
        Right = NULL;
    }

    ~ASTNode()
    {
        delete Left;
        delete Right;
    }

    void Draw()
    {
        std::string buffer;
        Draw(this, buffer, "", "");
        std::cout << buffer << std::endl;
    }

    void Draw(ASTNode* pRoot, std::string& buffer, const std::string& prefix, const std::string& childrenPrefix)
    {
        if(pRoot != nullptr) {
            buffer.append(prefix);
            buffer.append(std::string(1, pRoot->Type));
            buffer.append(to_string(pRoot->Value));
            buffer.append("\n");
            Draw(pRoot->Right, buffer, childrenPrefix + "├── ", childrenPrefix + "│   ");
            Draw(pRoot->Left, buffer, childrenPrefix + "└── ", childrenPrefix + "    ");
        }
    }
};

#endif //DIRECTORY_TEST_TREENODE_H
