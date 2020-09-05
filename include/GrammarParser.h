//
// Created by besseausamuel on 04/09/2020.
//

#ifndef DIRECTORY_TEST_GRAMMARPARSER_H
#define DIRECTORY_TEST_GRAMMARPARSER_H

#include "ParserHelper.h"

class GrammarParser : public ParserHelper
{
public:
    bool Process(const std::string& strText);
};


#endif //DIRECTORY_TEST_GRAMMARPARSER_H
