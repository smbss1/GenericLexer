//
// Created by besseausamuel on 08/09/2020.
//

#include <map>
#include <vector>
#include <algorithm>
#include "NFA.h"

NFA character(const std::string& strSymbol)
{
    State inState = State();
    State outState = State();
    outState.m_bAccept = true;

    inState.AddTransitionForSymbol(strSymbol, outState);

    return NFA(inState, outState);
}

NFA epsilon(const std::string& strSymbol)
{

    return character(EPSILON);
}

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


NFA ConcatPair(NFA oFirst, NFA oSecond)
{
    oFirst.m_oOutState.m_bAccept = false;
    oSecond.m_oOutState.m_bAccept = true;
    oFirst.m_oOutState.AddTransitionForSymbol(EPSILON, oSecond.m_oInState);
    return NFA(oFirst.m_oInState, oSecond.m_oOutState);
}

NFA Concat(NFA oFirst, std::vector<NFA> oSecond)
{
    for (auto& fragment : oSecond) {
        oFirst = ConcatPair(oFirst, fragment);
    }
    return oFirst;
}

/**
 * @brief Union: a|b
 * @param oFirst
 * @param oSecond
 * @return
 */

NFA OrPair(NFA oFirst, NFA oSecond)
{
    State start = State();
    start.AddTransitionForSymbol(EPSILON, oFirst.m_oInState);
    start.AddTransitionForSymbol(EPSILON, oSecond.m_oInState);

    State end = State();
    oFirst.m_oOutState.AddTransitionForSymbol(EPSILON, end);
    oFirst.m_oOutState.m_bAccept = false;
    oSecond.m_oOutState.AddTransitionForSymbol(EPSILON, end);
    oSecond.m_oOutState.m_bAccept = false;
    return NFA(start, end);
}

NFA OrArray(NFA oFirst, const std::vector<NFA>& oSecond)
{
    for (auto& fragment : oSecond) {
        oFirst = OrPair(oFirst, fragment);
    }
    return oFirst;
}

/**
 * Kleene star/closure.
 *
 * a*
 */
NFA RepExplicit(NFA oFragment) {
    State inState = State();
    State outState = State();
    outState.m_bAccept = true;

    // 0 or more.
    inState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
    inState.AddTransitionForSymbol(EPSILON, outState);

    oFragment.m_oOutState.m_bAccept = false;
    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, outState);
    outState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);

    return NFA(inState, outState);
}

NFA NFA::Rep(NFA oFragment)
{
    State start = State();
    State end = State();

    start.AddTransitionForSymbol(EPSILON, end);
    start.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, end);
    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);

    return NFA(start, end);
}

/**
 * Optimized Plus: just adds ε-transitions from
 * the output to the input.
 */
NFA PlusRep(NFA oFragment) {
    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
    return oFragment;
}

/**
 * Optimized ? repetition: just adds ε-transitions from
 * the input to the output.
 */
NFA QuestionRep(NFA oFragment) {
    oFragment.m_oInState.AddTransitionForSymbol(EPSILON, oFragment.m_oOutState);
    return oFragment;
}

NFA FromEpsilon() {
    State start = State();
    State end = State();
    start.AddTransitionForSymbol(EPSILON, end);

    return NFA(start, end);
}

NFA FromSymbol(const std::string& symbol) {
    State start = State();
    State end = State();
    start.AddTransitionForSymbol(symbol, end);

    return NFA(start, end);
}

NFA NFA::toNFA(const std::string& postfixExp) {
    if(postfixExp.empty()) {
        return FromEpsilon();
    }

    std::vector<NFA> vStack;

    for (auto& token : postfixExp) {
        if(token == '*') {
            auto val = vStack.back();
            vStack.pop_back();
            vStack.push_back(Rep(val));
        } else if (token == '|') {
            NFA right = vStack.back();
            vStack.pop_back();
            NFA left = vStack.back();
            vStack.pop_back();
            vStack.push_back(OrPair(left, right));
        } else if (token == '.') {
            NFA right = vStack.back();
            vStack.pop_back();
            NFA left = vStack.back();
            vStack.pop_back();
            vStack.push_back(ConcatPair(left, right));
        } else {
            vStack.push_back(FromSymbol(std::string(1, token)));
        }
    }
    NFA val = vStack.back();
    vStack.pop_back();
    return val;
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