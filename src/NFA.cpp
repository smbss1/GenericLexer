//
// Created by besseausamuel on 08/09/2020.
//

#include <map>
#include <vector>
#include <algorithm>
#include "NFA.h"

// -------------------------------------------------

NFA::NFA(State inState, State outState)
{
    m_oInState = std::move(inState);
    m_oOutState = std::move(outState);
}

NFA::NFA() {

}

State::State(bool bAccept)
{
    m_bAccept = bAccept;
}

void State::AddTransitionForSymbol(const std::string& strSymbol, const State& oState)
{
    // m_oTransitionMap.insert(strSymbol, oState);
    GetTransitionForSymbol(strSymbol).push_back(oState);
}

std::vector<State> State::GetTransitionForSymbol(const std::string& strSymbol)
{
    std::vector<State> vStates = m_oTransitionMap[strSymbol];
    if (vStates.empty())
        m_oTransitionMap.insert(std::make_pair(strSymbol, vStates));
    return vStates;
}

bool State::test(const std::string &basicString)
{
    return false;
}

//void AddNextState(State oState, std::vector<State>& oNextStates, std::vector<State>& vVisited) {
//    if (!oState.GetTransitionForSymbol(EPSILON).empty()) {
//        for (auto st : oState.GetTransitionForSymbol(EPSILON)) {
//            if (std::find(vVisited.begin(), vVisited.end(), st) == vVisited.end()) {
//                vVisited.push_back(st);
//                AddNextState(st, oNextStates, vVisited);
//            }
//        }
//    } else {
//        oNextStates.push_back(oState);
//    }
//}
//
//bool search(NFA nfa, std::string word) {
//    std::vector<State> currentStates;
//    std::vector<State> vVisited;
//    AddNextState(nfa.m_oInState, currentStates, vVisited);
//
//    for (auto symbol : word) {
//        std::vector<State> nextStates;
//
//        for (auto state : currentStates) {
//            State nextState = state.GetTransitionForSymbol(std::string(1, symbol));
//            if (nextState) {
//                std::vector<State> vVisited;
//                AddNextState(nextState, nextStates, vVisited);
//            }
//        }
//        currentStates = nextStates;
//    }
//
//    return currentStates.find(s => s.isEnd) ? true : false;
//}