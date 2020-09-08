//
// Created by besseausamuel on 08/09/2020.
//

#ifndef DIRECTORY_TEST_NFA_H
#define DIRECTORY_TEST_NFA_H

#include <map>
#include <utility>
#include <vector>

const std::string EPSILON = "ε";

class State
{
    std::map<std::string, std::vector<State>> m_oTransitionMap;

public:

    void AddTransitionForSymbol(const std::string& strSymbol, const State& oState);
    std::vector<State> GetTransitionForSymbol(const std::string& strSymbol);

    State(bool bAccept = false);

    bool test(const std::string &basicString);

    bool m_bAccept;

};

class NFA {

    bool test(const std::string& strString)
    {
        return m_oInState.test(strString);
    }

public:
    NFA();
    NFA(State inState, State outState);
    NFA toNFA(const std::string& postfixExp);
    NFA Rep(NFA oFragment);

    State m_oOutState;
    State m_oInState;
};

NFA Rep(NFA oFragment);

#endif //DIRECTORY_TEST_NFA_H
