/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#include <string>
#include <cstring>
#include <iostream>

#include "TreeNode.h"

//char *no_space(char *str);
//int sum(char **str);
//
//
//int my_add(int a, int b)
//{
//    return (a + b);
//}
//
//int my_sub(int a, int b)
//{
//    return (a - b);
//}
//
//int my_mul(int a, int b)
//{
//    return (a * b);
//}
//
//int my_div(int a, int b)
//{
//    return (a / b);
//}
//
//int my_mod(int a, int b)
//{
//    return (a % b);
//}
//
//int number(char **str)
//{
//    int nbr = 0;
//    int neg = 1;
//
//    while (*str[0] == '+' || *str[0] == '-') {
//        if (*str[0] == '-')
//            neg *= -1;
//        *str += 1;
//    }
//    if (*str[0] == '(') {
//        *str += 1;
//        nbr = sum(str);
//        if (*str[0] == ')')
//            *str += 1;
//        return (nbr * neg);
//    }
//    while (isdigit(*str[0])) {
//        nbr *= 10;
//        nbr += *str[0] - '0';
//        *str += 1;
//    }
//    return(nbr * neg);
//}
//
//int my_strtol(char *str, char **endptr)
//{
//    int num = number(&str);
//    char *p = str;
//
//    for (int i = 0; *p != '\0'; i++) {
//        if (!isdigit(*p))
//            break;
//        *p++;
//    }
//    *endptr = p;
//    return (num);
//}
//
//static int verif_error(int i, char operation[], int num2)
//{
//    if (operation[i] == '/' && num2 == 0) {
//        std::cout << "Stop: division by zero" << std::endl;
//        return (84);
//    } else if (operation[i] == '%' && num2 == 0) {
//        std::cout << "Stop: modulo by zero" << std::endl;
//        return (84);
//    }
//    return (0);
//}
//
//int do_op(int num1, int num2, char op)
//{
//    int result = 0;
//    int (*operation[])(int, int) = {my_add, my_sub, my_mul, my_div, my_mod};
//    char operators[] = {'+', '-', '*', '/', '%'};
//
//    for (int i = 0; i < 5; i++)
//        if (operators[i] == op) {
//        int code = verif_error(i, operators, num2);
//        if(code == 84)
//            return (code);
//        result = operation[i](num1, num2);
//        return (result);
//    }
//    return (0);
//}
//
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
//int sum(char **str)
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
//
//char *no_space(char *str)
//{
//    char *str2 = (char *) malloc(sizeof(char) * strlen(str));
//    int j = 0;
//
//    for (int i = 0; str[i] != '\0'; i++) {
//        if (str[i] != ' ') {
//            str2[j] = str[i];
//            j++;
//        }
//    }
//    str2[j] = '\0';
//    return (str2);
//}
//
//int eval_expr(char const *str)
//{
//    str = no_space((char*) str);
//    if (str[0] == '\0')
//        return (0);
//    return (sum((char**) &str));
//}
//
//int main(int ac, char** av)
//{
//    if (ac == 2) {
//        std::cout << eval_expr(av[1]) << std::endl;
//        return (0);
//    }
//    return (84);
//}

char *no_space(char *str);
shared_ptr<TreeNode<int>> sum(char **str);


int my_add(int a, int b)
{
    return (a + b);
}

int my_sub(int a, int b)
{
    return (a - b);
}

int my_mul(int a, int b)
{
    return (a * b);
}

int my_div(int a, int b)
{
    return (a / b);
}

int my_mod(int a, int b)
{
    return (a % b);
}

int number(char **str)
{
    int nbr = 0;
    int neg = 1;

    while (*str[0] == '+' || *str[0] == '-') {
        if (*str[0] == '-')
            neg *= -1;
        *str += 1;
    }
    if (*str[0] == '(') {
        *str += 1;
        nbr = sum(str)->Get();
        if (*str[0] == ')')
            *str += 1;
        return (nbr * neg);
    }
    while (isdigit(*str[0])) {
        nbr *= 10;
        nbr += *str[0] - '0';
        *str += 1;
    }
    return(nbr * neg);
}

int my_strtol(char *str, char **endptr)
{
    int num = number(&str);
    char *p = str;

    for (int i = 0; *p != '\0'; i++) {
        if (!isdigit(*p))
            break;
        *p++;
    }
    *endptr = p;
    return (num);
}

static int verif_error(int i, const char operation[], int num2)
{
    if (operation[i] == '/' && num2 == 0) {
        std::cout << "Stop: division by zero" << std::endl;
        return (84);
    } else if (operation[i] == '%' && num2 == 0) {
        std::cout << "Stop: modulo by zero" << std::endl;
        return (84);
    }
    return (0);
}

int do_op(int num1, int num2, char op)
{
    int result = 0;
    int (*operation[])(int, int) = {my_add, my_sub, my_mul, my_div, my_mod};
    char operators[] = {'+', '-', '*', '/', '%'};

    for (int i = 0; i < 5; i++)
        if (operators[i] == op) {
            int code = verif_error(i, operators, num2);
            if(code == 84)
                return (code);
            result = operation[i](num1, num2);
            return (result);
        }
    return (0);
}

shared_ptr<TreeNode<int>> factor(char **str)
{
    shared_ptr<TreeNode<int>> nbr1(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> nbr2(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> oper(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> result(new TreeNode<int>(0));
    char op;

    nbr1->Set(my_strtol(*str, str));
    while (*str[0] == '*' || *str[0] == '/' || *str[0] == '%') {
        op = *str[0];
        oper->Set(op);
        *str += 1;
        nbr2->Set(my_strtol(*str, str));
        result->Set(do_op(nbr1->Get(), nbr2->Get(), op));
        oper->AddChild(nbr1);
        oper->AddChild(nbr2);
    }
    return (result);
}

shared_ptr<TreeNode<int>> sum(char **str)
{
    shared_ptr<TreeNode<int>> nbr1(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> nbr2(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> oper(new TreeNode<int>(0));
    shared_ptr<TreeNode<int>> result(new TreeNode<int>(0));
    char op;

    nbr1->Set(my_strtol(*str, str));
    while (*str[0] != '\0' && *str[0] != ')') {
        op = *str[0];
        oper->Set(op);
        *str += 1;
        if (op == '+' || op == '-')
            nbr2 = factor(str);
        else
            nbr2->Set(my_strtol(*str, str));
        result->Set(do_op(nbr1->Get(), nbr2->Get(), op));
        oper->AddChild(nbr1);
        oper->AddChild(nbr2);
    }
    return (result);
}

char *no_space(char *str)
{
    char *str2 = (char *) malloc(sizeof(char) * strlen(str));
    int j = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ') {
            str2[j] = str[i];
            j++;
        }
    }
    str2[j] = '\0';
    return (str2);
}

shared_ptr<TreeNode<int>> eval_expr(char const *str)
{
    str = no_space((char*) str);
    if (str[0] == '\0')
        return (0);
    shared_ptr<TreeNode<int>> root(sum((char**) &str));
    return (root);
}

int main(int ac, char** av)
{
    if (ac == 2) {
        shared_ptr<TreeNode<int>> root = eval_expr(av[1]);
        return (0);
    }
    return (84);
}