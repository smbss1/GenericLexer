

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include "Lexer.h"

class Timer
{
public:
    Timer(const char* name) : m_Name(name), m_Stopped(false)
    {
        m_StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!m_Stopped)
            Stop();
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
        auto end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();
        m_Stopped = true;

        auto duration = end - start;
        double ms = duration * 0.001;
        std::cout << m_Name << ": " << duration << "us (" << ms << "ms)\n";
    }
private:
    const char* m_Name;
    bool m_Stopped;
    std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
};

bool ReadFile(const std::string& strFile, std::string& strOutReadContent)
{
    std::ifstream oFile(strFile); //open the input file
    if (!oFile.is_open())
        return false;

    std::stringstream strStream;
    strStream << oFile.rdbuf(); //read the file
    strOutReadContent = strStream.str();

    return true;
}

int main()
{
    Lexer oLexer;
    oLexer.m_bEnableError = false;
    oLexer.define("TOKEN_NEW_LINE","\n", true);
    oLexer.define("TOKEN_WS","[ \t\r\b]+", true);
    oLexer.define("TOKEN_NUMBER","([0-9]+)|([0-9]+.[0-9]+)");
    // oLexer.define(TOKEN_INT,"[0-9]+");
    oLexer.define_area("TOKEN_STRING",'"', '"');
	oLexer.define("TOKEN_PLUS", "\\+");
    oLexer.define("TOKEN_PLUS_PLUS", "\\+\\+");
	oLexer.define("TOKEN_MINUS", "-");
    oLexer.define("TOKEN_MINUS_MINUS", "--");
    oLexer.define("TOKEN_DOUBLE_COLON","::");
    oLexer.define("TOKEN_COLON",":");
    oLexer.define("TOKEN_LEFT_PAREN","\\(");
    oLexer.define("TOKEN_RIGHT_PAREN","\\)");
    oLexer.define("TOKEN_LEFT_BRACE","\\{");
    oLexer.define("TOKEN_RIGHT_BRACE","\\}");
    oLexer.define("TOKEN_LEFT_BRACKET","\\[");
    oLexer.define("TOKEN_RIGHT_BRACKET","\\]");
	oLexer.define("TOKEN_DOT", "\\.");
    oLexer.define("TOKEN_SEMICOLON",";");
    oLexer.define("TOKEN_SLASH","/");
    oLexer.define("TOKEN_COMMA",",");
    oLexer.define("TOKEN_STAR","\\*");
    oLexer.define("TOKEN_GREATER",">");
    oLexer.define("TOKEN_GREATER_EQUAL",">=");
    oLexer.define("TOKEN_LESS", "<");
    oLexer.define("TOKEN_LESS_EQUAL", "<=");
    oLexer.define("TOKEN_NIL", "nil");
    oLexer.define("TOKEN_OR", "\\|\\|");
    oLexer.define("TOKEN_AND", "&&");
    oLexer.define("TOKEN_PRINT", "print");
    oLexer.define("TOKEN_SUPER", "super");
    oLexer.define("TOKEN_THIS", "this");
    oLexer.define("TOKEN_TRUE", "true");
    oLexer.define("TOKEN_FALSE", "false");
    oLexer.define("TOKEN_FOR", "for");
    oLexer.define("TOKEN_FUN", "func");
    oLexer.define("TOKEN_IF", "if");
    oLexer.define("TOKEN_IMPORT", "import");
    oLexer.define("TOKEN_RETURN", "return");
    oLexer.define("TOKEN_VAR", "var");
    oLexer.define("TOKEN_DOUBLE_DOT_EQUAL", ":=");
    oLexer.define("TOKEN_WHILE", "while");
    oLexer.define("TOKEN_SWITCH", "switch");
    oLexer.define("TOKEN_BANG", "!");
    oLexer.define("TOKEN_BANG_EQUAL", "!=");
    oLexer.define("TOKEN_CLASS", "class");
    oLexer.define("TOKEN_ELSE", "else");
    oLexer.define("TOKEN_IS", "is");
    oLexer.define("TOKEN_EQUAL", "=");
    oLexer.define("TOKEN_EQUAL_EQUAL", "==");
    oLexer.define("TOKEN_SHEBANG","#[^\n\r]*", true);
    oLexer.define("TOKEN_IDENTIFIER","[A-Za-z_]+[0-9]*");
    oLexer.define("Token Operator", "operator");

    oLexer.define("TOKEN_SINGLE_COMMENT","//[^\n\r]*", true);
    oLexer.define("Token_Start_MultiComment","/\\*");
    oLexer.define("Token_End_MultiComment","\\*/");
    oLexer.define("TOKEN_EOF","\\0");

    std::string file;
    if (ReadFile("test.fox", file)) {
        Timer t("Process");
        oLexer.process(file);
    }
    if (ReadFile("test.fox", file)) {
        Timer t("Process Async");
        oLexer.process_async(file);
        // helper::Dump(oLexer);
    }
}