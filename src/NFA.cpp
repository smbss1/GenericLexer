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

#define DEBUG 1

using namespace std;

string join(vector<int> v, string delim)
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
    result.set_vertex(a.get_vertex_count() + b.get_vertex_count());
    int i;
    trans new_trans;

    for(i = 0; i < a.transitions.size(); i++) {
        new_trans = a.transitions.at(i);
        result.set_transition(new_trans.vertex_from, new_trans.vertex_to, new_trans.trans_symbol);
    }

    result.set_transition(a.get_final_state(), a.get_vertex_count(), '^');

    for(i = 0; i < b.transitions.size(); i++) {
        new_trans = b.transitions.at(i);
        result.set_transition(new_trans.vertex_from + a.get_vertex_count(), new_trans.vertex_to + a.get_vertex_count(), new_trans.trans_symbol);
    }

    result.set_final_state(a.get_vertex_count() + b.get_vertex_count() - 1);

    return result;
}


NFA kleene(NFA a)
{
    NFA result;
    int i;
    trans new_trans;

    result.set_vertex(a.get_vertex_count() + 2);

    result.set_transition(0, 1, '^');

    for(i = 0; i < a.transitions.size(); i++) {
        new_trans = a.transitions.at(i);
        result.set_transition(new_trans.vertex_from + 1, new_trans.vertex_to + 1, new_trans.trans_symbol);
    }

    result.set_transition(a.get_vertex_count(), a.get_vertex_count() + 1, '^');
    result.set_transition(a.get_vertex_count(), 1, '^');
    result.set_transition(0, a.get_vertex_count() + 1, '^');

    result.set_final_state(a.get_vertex_count() + 1);

    return result;
}


NFA or_selection(vector<NFA> selections, int no_of_selections)
{
    NFA result;
    int vertex_count = 2;
    int i, j;
    NFA med;
    trans new_trans;

    for(i = 0; i < no_of_selections; i++) {
        vertex_count += selections.at(i).get_vertex_count();
    }

    result.set_vertex(vertex_count);

    int adder_track = 1;

    for(i = 0; i < no_of_selections; i++) {
        result.set_transition(0, adder_track, '^');
        med = selections.at(i);
        for(j = 0; j < med.transitions.size(); j++) {
            new_trans = med.transitions.at(j);
            result.set_transition(new_trans.vertex_from + adder_track, new_trans.vertex_to + adder_track, new_trans.trans_symbol);
        }
        adder_track += med.get_vertex_count();

        result.set_transition(adder_track - 1, vertex_count - 1, '^');
    }

    result.set_final_state(vertex_count - 1);

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
            new_sym->set_vertex(2);
            new_sym->set_transition(0, 1, cur_sym);
            new_sym->set_final_state(1);
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
    const vector<int> s0 = nfa.eclosure(start);

    int vertex_from = dfa.add_entry(s0);

    while (vertex_from != -1) {
        const vector<int> T = dfa.entry_at(vertex_from);
        dfa.mark_entry(vertex_from);

        const vector<char> symbols = nfa.find_possible_input_symbols(T);
        for (int i = 0; i < symbols.size(); i++) {
            char a = symbols.at(i);

            //TODO: add a eclosure cache : { state => eclosure }
            const vector<int> U = nfa.eclosure(nfa.move(T, a));

            int vertex_to = dfa.find_entry(U);
            if (vertex_to == -1) { // U not already in S'
                vertex_to = dfa.add_entry(U);
            }

            dfa.set_transition(vertex_from, vertex_to, a);
        }

        vertex_from = dfa.next_unmarked_entry_idx();
    }

    // The finish states of the DFA are those which contain any
    // of the finish states of the NFA.
    dfa.set_final_state(nfa.get_final_state());

    return dfa;
}