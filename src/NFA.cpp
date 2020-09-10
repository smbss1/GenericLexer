////
//// Created by besseausamuel on 08/09/2020.
////
//
//#include <map>
//#include <utility>
//#include <vector>
//#include <algorithm>
//#include "NFA.h"
//
//// -------------------------------------------------
//
//NFA::NFA(State inState, State outState)
//{
//    m_oInState = std::move(inState);
//    m_oOutState = std::move(outState);
//}
//
//State::State(bool bAccept)
//{
//    m_bAccept = bAccept;
//}
//
//State::State(std::string strName, bool bAccept)
//{
//    m_strName = std::move(strName);
//    m_bAccept = bAccept;
//}
//
//void State::AddTransitionForSymbol(const std::string& strSymbol, const State& oState)
//{
//    // m_oTransitionMap.insert(strSymbol, oState);
//    GetTransitionForSymbol(strSymbol).push_back(oState);
////    std::vector<State>& vStates = m_oTransitionMap[strSymbol];
////    vStates.push_back(oState);
////    m_oTransitionMap.insert(std::make_pair(strSymbol, m_oTransitionMap[strSymbol]))
//}
//
//std::vector<State>& State::GetTransitionForSymbol(const std::string& strSymbol)
//{
//    std::vector<State>& vStates = m_oTransitionMap[strSymbol];
//    if (vStates.empty())
//        m_oTransitionMap.insert(std::make_pair(strSymbol, vStates));
//    return vStates;
//}
//
//bool State::test(const std::string &basicString)
//{
//    return false;
//}
////
////void NFA::PrintTransition(std::vector<State>& vStates)
////{
////    for (auto& oState : vStates)
////    {
////        std::cout << oState.m_strName << " --> ";
////    }
////}
////
////void NFA::PrintAllTransitions()
////{
////    for (auto& oState : m_oStates)
////    {
////        PrintTransition(oState);
////    }
////}
//
////void AddNextState(State oState, std::vector<State>& oNextStates, std::vector<State>& vVisited) {
////    if (!oState.GetTransitionForSymbol(EPSILON).empty()) {
////        for (auto st : oState.GetTransitionForSymbol(EPSILON)) {
////            if (std::find(vVisited.begin(), vVisited.end(), st) == vVisited.end()) {
////                vVisited.push_back(st);
////                AddNextState(st, oNextStates, vVisited);
////            }
////        }
////    } else {
////        oNextStates.push_back(oState);
////    }
////}
///*
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
//    for (auto& cur : currentStates) {
//        if (cur.m_bAccept)
//            return true;
//    }
//    return false;
//
//    // return currentStates.find(s => s.isEnd) ? true : false;
//}*/



#include<iostream>
#include<sstream>
#include<vector>
#include<string>
#include<stack>
#include <algorithm>
#include "NFA.h"
#include "DFA.h"

#define DEBUG 1

using namespace std;

string Join(vector<int> v, string delim)
{
    stringstream ss;
    for(int i = 0; i < v.size(); ++i) {
        if(i != 0)
            ss << ",";
        ss << v[i];
    }
    return ss.str();
}


NFA concat(NFA a, NFA b)
{
    NFA result;
    result.SetVertex(a.GetVertexCount() + b.GetVertexCount());
    int i;
    trans oNewTransition;

    for(i = 0; i < a.transitions.size(); i++) {
        oNewTransition = a.transitions.at(i);
        result.SetTransition(oNewTransition.vertex_from, oNewTransition.vertex_to, oNewTransition.trans_symbol);
    }

    result.SetTransition(a.GetFinalState(), a.GetVertexCount(), '^');

    for(i = 0; i < b.transitions.size(); i++) {
        oNewTransition = b.transitions.at(i);
        result.SetTransition(oNewTransition.vertex_from + a.GetVertexCount(), oNewTransition.vertex_to + a.GetVertexCount(), oNewTransition.trans_symbol);
    }

    result.SetFinalState(a.GetVertexCount() + b.GetVertexCount() - 1);

    return result;
}


NFA kleene(NFA a)
{
    NFA result;
    int i;
    trans oNewTransition;

    result.SetVertex(a.GetVertexCount() + 2);

    result.SetTransition(0, 1, '^');

    for(i = 0; i < a.transitions.size(); i++) {
        oNewTransition = a.transitions.at(i);
        result.SetTransition(oNewTransition.vertex_from + 1, oNewTransition.vertex_to + 1, oNewTransition.trans_symbol);
    }

    result.SetTransition(a.GetVertexCount(), a.GetVertexCount() + 1, '^');
    result.SetTransition(a.GetVertexCount(), 1, '^');
    result.SetTransition(0, a.GetVertexCount() + 1, '^');

    result.SetFinalState(a.GetVertexCount() + 1);

    return result;
}


NFA or_selection(vector<NFA> selections, int no_of_selections)
{
    NFA result;
    int vertex_count = 2;
    int i, j;
    NFA med;
    trans oNewTransition;

    for(i = 0; i < no_of_selections; i++) {
        vertex_count += selections.at(i).GetVertexCount();
    }

    result.SetVertex(vertex_count);

    int adder_track = 1;

    for(i = 0; i < no_of_selections; i++) {
        result.SetTransition(0, adder_track, '^');
        med = selections.at(i);
        for(j = 0; j < med.transitions.size(); j++) {
            oNewTransition = med.transitions.at(j);
            result.SetTransition(oNewTransition.vertex_from + adder_track, oNewTransition.vertex_to + adder_track, oNewTransition.trans_symbol);
        }
        adder_track += med.GetVertexCount();

        result.SetTransition(adder_track - 1, vertex_count - 1, '^');
    }

    result.SetFinalState(vertex_count - 1);

    return result;
}


NFA re_to_nfa(string re)
{
    stack<char> operators;
    stack<NFA> operands;
    char op_sym;
    int op_count;
    char cur_sym;
    NFA *new_sym;

    for(string::iterator it = re.begin(); it != re.end(); ++it) {
        cur_sym = *it;
        if(cur_sym != '(' && cur_sym != ')' && cur_sym != '*' && cur_sym != '|' && cur_sym != '.') {
            new_sym = new NFA();
            new_sym->SetVertex(2);
            new_sym->SetTransition(0, 1, cur_sym);
            new_sym->SetFinalState(1);
            operands.push(*new_sym);
            delete new_sym;
        } else {
            if(cur_sym == '*') {
                NFA star_sym = operands.top();
                operands.pop();
                operands.push(kleene(star_sym));
            } else if(cur_sym == '.') {
                operators.push(cur_sym);
            } else if(cur_sym == '|') {
                operators.push(cur_sym);
            } else if(cur_sym == '(') {
                operators.push(cur_sym);
            } else {
                op_count = 0;
                char c;
                op_sym = operators.top();
                if(op_sym == '(') continue;
                do {
                    operators.pop();
                    op_count++;
                } while(operators.top() != '(');
                operators.pop();
                NFA op1;
                NFA op2;
                vector<NFA> selections;
                if(op_sym == '.') {
                    for(int i = 0; i < op_count; i++) {
                        op2 = operands.top();
                        operands.pop();
                        op1 = operands.top();
                        operands.pop();
                        operands.push(concat(op1, op2));
                    }
                } else if(op_sym == '|'){
                    selections.assign(op_count + 1, NFA());
                    int tracker = op_count;
                    for(int i = 0; i < op_count + 1; i++) {
                        selections.at(tracker) = operands.top();
                        tracker--;
                        operands.pop();
                    }
                    operands.push(or_selection(selections, op_count+1));
                } else {

                }
            }
        }
    }

    return operands.top();
}


DFA nfa_to_dfa(NFA nfa)
{
    DFA dfa;

    const vector<int> start(1, 0);
    const vector<int> s0 = nfa.Eclosure(start);

    int vertex_from = dfa.AddEntry(s0);

    while (vertex_from != -1) {
        const vector<int> T = dfa.EntryAt(vertex_from);
        dfa.MarkEntry(vertex_from);

        const vector<char> symbols = nfa.FindPossibleInputSymbols(T);
        for (int i = 0; i < symbols.size(); i++) {
            char a = symbols.at(i);

            //TODO: add a Eclosure cache : { state => Eclosure }
            const vector<int> U = nfa.Eclosure(nfa.Move(T, a));

            int vertex_to = dfa.FindEntry(U);
            if (vertex_to == -1) { // U not already in S'
                vertex_to = dfa.AddEntry(U);
            }

            dfa.SetTransition(vertex_from, vertex_to, a);
        }

        vertex_from = dfa.NextUnmarkedEntryIndex();
    }

    // The finish states of the DFA are those which contain any
    // of the finish states of the NFA.
    dfa.SetFinalState(nfa.GetFinalState());

    return dfa;
}