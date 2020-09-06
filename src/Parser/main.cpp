
#include <vector>
#include <string>
#include "Lexer.h"
#include "CommutativeInserter.h"
#include "TreeNode.h"
#include "Parser.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <cctype>
#include <iterator>

#include <fstream>
#include <iostream>
#include <string>

#include "Unger.h"

//using namespace std;
//
//class Exp {
//public:
////  Exp(){}
//    virtual void print(){}
//    virtual void release(){}
//    virtual void Draw(Exp* pRoot, std::string& buffer, const std::string& prefix, const std::string& childrenPrefix) {}
//    void Draw()
//    {
//        std::string buffer;
//        Draw(this, buffer, "", "");
//        std::cout << buffer << std::endl;
//        std::cout << "buffer" << std::endl;
//    }
//};
//class Term: public Exp {
//    string val;
//public:
//    Term(string v) : val(v){}
//    void print(){
//        cout << ' ' << val << ' ';
//    }
//    void release(){}
//
//    void Draw(Exp* pRoot, std::string& buffer, const std::string& prefix, const std::string& childrenPrefix) override
//    {
//        if(pRoot != nullptr) {
//            buffer.append(prefix);
//            buffer.append(val);
//            buffer.append("\n");
//        }
//    }
//};
//
//class Node: public Exp {
//    Exp *l_exp;
//    Exp *r_exp;
//    char op; // +, -, *, /
//public:
//    Node(char op, Exp* left, Exp* right):op(op),l_exp(left), r_exp(right){}
//    ~Node(){
//    }
//    void print(){
//        cout << '(' << op << ' ';
//        l_exp->print();
//        r_exp->print();
//        cout  << ')';
//    }
//    void release(){
//        l_exp->release();
//        r_exp->release();
//        delete l_exp;
//        delete r_exp;
//    }
//
//    void Draw(Exp* pRoot, std::string& buffer, const std::string& prefix, const std::string& childrenPrefix) override
//    {
//        if(pRoot != nullptr) {
//            buffer.append(prefix);
//            buffer.push_back(op);
//            buffer.append("\n");
//            r_exp->Draw(r_exp, buffer, childrenPrefix + "├── ", childrenPrefix + "│   ");
//            l_exp->Draw(l_exp, buffer, childrenPrefix + "└── ", childrenPrefix + "    ");
//        }
//    }
//};
//
//Exp* strToExp(string &str){
//    int level = 0;//inside parentheses check
//    //case + or -
//    //most right '+' or '-' (but not inside '()') search and split
//    for(int i=str.size()-1;i>=0;--i){
//        char c = str[i];
//        if(c == ')'){
//            ++level;
//            continue;
//        }
//        if(c == '('){
//            --level;
//            continue;
//        }
//        if(level>0) continue;
//        if((c == '+' || c == '-') && i!=0 ){//if i==0 then s[0] is sign
//            string left(str.substr(0,i));
//            string right(str.substr(i+1));
//            return new Node(c, strToExp(left), strToExp(right));
//        }
//    }
//    //case * or /
//    //most right '*' or '/' (but not inside '()') search and split
//    for(int i=str.size()-1;i>=0;--i){
//        char c = str[i];
//        if(c == ')'){
//            ++level;
//            continue;
//        }
//        if(c == '('){
//            --level;
//            continue;
//        }
//        if(level>0) continue;
//        if(c == '*' || c == '/'){
//            string left(str.substr(0,i));
//            string right(str.substr(i+1));
//            return new Node(c, strToExp(left), strToExp(right));
//        }
//    }
//    if(str[0]=='('){
//        //case ()
//        //pull out inside and to strToExp
//        for(int i=0;i<str.size();++i){
//            if(str[i]=='('){
//                ++level;
//                continue;
//            }
//            if(str[i]==')'){
//                --level;
//                if(level==0){
//                    string exp(str.substr(1, i-1));
//                    return strToExp(exp);
//                }
//                continue;
//            }
//        }
//    } else
//        //case value
//        return new Term(str);
//    cerr << "Error:never execute point" << endl;
//    return NULL;//never
//}
//






//int main() {
//    string exp(" ( a + b ) * c - ( d - e ) * f / g");
//    //remove space character
//    exp.erase(remove_if(exp.begin(), exp.end(), ::isspace), exp.end());
//    Exp *tree = strToExp(exp);
//    std::string buffer;
//    tree->Draw(tree, buffer, "", "");
//    std::cout << buffer << std::endl;
//    tree->print();
//    tree->release();
//    delete tree;
//}
//
//int main(int argc, char *argv[])
//{
//    Lexer lexer;
////	std::vector<std::string> oSymbols = {
////		"(", ")", "[", "]",
////		"{", "}", ";",
////		"::", "->", ":",
////		"*", "+", "-",
////		"/", "<<", ">>", ",","!", ">"
////	};
////	lexer.AddSymbols(oSymbols);
////	lexer.LoadGrammar(
////	    "terminal:"
////                   "<Whitespace>:[ ]"
////                   "<Word>: [a-zA-Z0-9]"
////                   "<Number>: [0-9]"
////                   "<Comma>: ','"
////                   "<RBraces>: '('"
////                   "<LBraces>: ')'"
////                   "<Div>: '/'"
////                   "<Plus>: '+'"
////                   "<Expression>: <Number> + <Number>");
////
////	lexer.Process("1 (2variable1 / 3variable2)4 + variable3");
////	helper::Dump(lexer);
//    TreeNode<string> node1("2");
//    shared_ptr<TreeNode<string>> node2(new TreeNode<string>("3"));
//    shared_ptr<TreeNode<string>> result(new TreeNode<string>("5"));
//    shared_ptr<TreeNode<string>> op(new TreeNode<string>("+"));
//
//    node1.AddChild(node2);
//    node1.AddChild(result);
//    node1.AddChild(op);
//
//    // node1.Print();
//    node1.Draw();
//}
//
//void Test(const char* text)
//{
//    Parser parser;
//
//    try
//    {
//        ASTNode* ast = parser.Parse(text);
//        try
//        {
//            ast->Draw();
////            Evaluator eval;
////            double val = eval.Evaluate(ast);
////
////            std::cout << text << " = " << val << std::endl;
//        }
//        catch(exception& ex)
//        {
//            std::cout << text << " t " << ex.what() << std::endl;
//        }
//
//        delete ast;
//    }
//    catch(exception& ex)
//    {
//        std::cout << text << " t " << ex.what() << std::endl;
//    }
//}
//
//int main()
//{
//    std::string filename = "../unger.dat";
//    std::ifstream ifs(filename);
//    if (ifs) {
//        MB::Grammar grammar(ifs);
//        std::cout << grammar << std::endl;
////        std::string sentence[] = {"0", "+", "0"};
////        const size_t len = sizeof(sentence) / sizeof(sentence[0]);
////        bool success = MB::unger_parser(grammar).parse(sentence, sentence + len, std::cout);
////        std::cout << "Success: " << std::boolalpha << success << '\n';
//    }
//    else {
//        std::cerr << "Couldn't open " << filename << " for reading\n";
//    }


//     Test("1 + 2 + 3 + 4");
//    Test("1*2*3*4");
//    Test("1-2-3-4");
//    Test("1/2/3/4");
//    Test("1*2+3*4");
//    Test("1+2*3+4");
//    Test("(1+2)*(3+4)");
//    Test("1+(2*3)*(4+5)");
//    Test("1+(2*3)/4+5");
//    Test("5/(4+3)/2");
//    Test("1 + 2.5");
//    Test("125");
//    Test("-1");
//    Test("-1+(-2)");
//    Test("-1+(-2.0)");
//
//    Test("   1*2,5");
//    Test("   1*2.5e2");
//    Test("M1 + 2.5");
//    Test("1 + 2&5");
//    Test("1 * 2.5.6");
//    Test("1 ** 2.5");
//    Test("*1 / 2.5");

//    return 0;
//}
