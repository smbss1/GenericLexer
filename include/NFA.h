////
//// Created by besseausamuel on 08/09/2020.
////

#pragma once

#ifndef DIRECTORY_TEST_NFA_H
#define DIRECTORY_TEST_NFA_H
//
#include <map>
#include <utility>
#include <vector>
#include <iostream>
//
//const std::string EPSILON = "ε";
//
//class State
//{
//
//public:
//    State(bool bAccept = false);
//
//// bool isFinal = false;
//    void AddTransitionForSymbol(const std::string& strSymbol, const State& oState);
//    std::vector<State>& GetTransitionForSymbol(const std::string& strSymbol);
//    std::string m_strName;
//
//    State(std::string m_strName, bool bAccept = false);
//
//
//    bool test(const std::string &basicString);
//
//    bool m_bAccept;
//
//    bool matches(std::string s) {
//        return matches(s, std::vector<State*>()) ;
//    }
//
//    bool matches(std::string s, std::vector<State*> visited) {
//        /*
//         * When matching, we work character by character.
//         *
//         * If we're out of characters in the string, we'll check to
//         * see if this state if final, or if we can get to a final
//         * state from here through empty edges.
//         *
//         * If we're not out of characters, we'll try to consume a
//         * character and then match what's left of the string.
//         *
//         * If that fails, we'll ask if empty-edge neighbors can match
//         * the entire string.
//         *
//         * If that fails, the match fails.
//         *
//         * Note: Because we could have a circular loop of empty
//         * transitions, we'll have to keep track of the states we
//         * visited through empty transitions so we don't end up
//         * looping forever.
//         */
//        std::cout << "Is Visited?" << std::endl;
//        if (std::find(visited.begin(), visited.end(), this) != visited.end())
//            /* We've found a path back to ourself through empty edges;
//             * stop or we'll go into an infinite loop. */
//            return false ;
//        /* In case we make an empty transition, we need to add this
//         * state to the visited list. */
//        visited.push_back(this);
//
//        if (s.empty()) {
//            std::cout << "String Is Empty" << std::endl;
//            /* The string is empty, so we match this string only if
//             * this state is a final state, or we can reach a final
//             * state without consuming any input. */
//            std::cout << "Accept?" << std::endl;
//            if (m_bAccept)
//                return true;
//
//            /* Since this state is not final, we'll ask if any
//             * neighboring states that we can reach on empty edges can
//             * match the empty string. */
//            std::cout << "Next State EPSILON" << std::endl;
//            for (State next : GetTransitionForSymbol(EPSILON)) {
//                if (next.matches("", visited))
//                    return true;
//            }
//            std::cout << "Next State EPSILON Non Matches" << std::endl;
//            return false;
//        }
//        /* In this case, the string is not empty, so we'll pull
//         * the first character off and check to see if our
//         * neighbors for that character can match the remainder of
//         * the string. */
//
//        char c = (char)s.at(0);
//        std::cout << "Char: " << c << std::endl;
//        std::cout << "Next State" << std::endl;
//
//        for (State next : GetTransitionForSymbol(std::string(1, c))) {
//            std::cout << "Next State: " << next.m_strName << std::endl;
//            if (next.matches(s.substr(1)))
//                return true;
//        }
//
//        /* It looks like we weren't able to match the string by
//         * consuming a character, so we'll ask our
//         * empty-transition neighbors if they can match the entire
//         * string. */
//        std::cout << "Next State EPSILON" << std::endl;
//
//        for (State next : GetTransitionForSymbol(EPSILON)) {
//            std::cout << "Next State: EPS" << std::endl;
//            if (next.matches(s, visited))
//                return true;
//        }
//        std::cout << "Next State EPSILON Non Matches" << std::endl;
//        return false;
//    }
//
//    void Print()
//    {
//        std::cout << "- - - - - - - - - - -" << std::endl;
//        for (auto& it : m_oTransitionMap) {
//            std::cout << "First: " << it.first << std::endl;
//            for (State next : it.second) {
//                std::cout << "Next State: " << next.m_bAccept << std::endl;
//                next.Print();
//            }
//        }
//        std::cout << "* * * *  * * * * * *  *" << std::endl;
//    }
//
//    std::map<std::string, std::vector<State>> m_oTransitionMap;
//};
//
//class NFA {
//
//    bool test(const std::string& strString)
//    {
//        return m_oInState.test(strString);
//    }
//
//public:
//    NFA(State inState, State outState);
//
//    State m_oOutState;
//    State m_oInState;
//
//    bool matches(std::string& str) {
//        return m_oInState.matches(str);
//    }
//
//    void Print()
//    {
//        m_oInState.Print();
//        m_oOutState.Print();
//    }
//};
//


struct trans {
    int vertex_from;
    int vertex_to;
    char trans_symbol;
};


class NFA {
public:
    std::vector<int> vertex;
    std::vector<trans> transitions;
    int final_state;

    NFA() {

    }

    int get_vertex_count() {
        return vertex.size();
    }

    void set_vertex(int no_vertex) {
        for(int i = 0; i < no_vertex; i++) {
            vertex.push_back(i);
        }
    }

    void set_transition(int vertex_from, int vertex_to, char trans_symbol) {
        trans new_trans;
        new_trans.vertex_from = vertex_from;
        new_trans.vertex_to = vertex_to;
        new_trans.trans_symbol = trans_symbol;
        transitions.push_back(new_trans);
    }

    void set_final_state(int fs) {
        final_state = fs;
    }

    int get_final_state() {
        return final_state;
    }

    void display() {
        trans new_trans;
        std::cout<<"\n";
        for(int i = 0; i < transitions.size(); i++) {
            new_trans = transitions.at(i);
            std::cout<<"q"<<new_trans.vertex_from<<" -> q"<<new_trans.vertex_to<<" : Symbol - "<<new_trans.trans_symbol<<std::endl;
        }
        std::cout<<"\nThe final state is q"<<get_final_state()<<std::endl;
    }

    /**
     * Get the set of reachable state from each specified vertex.
     */
    std::vector<char> find_possible_input_symbols(const std::vector<int>& vertexs) {
        std::vector<char> result;

        for (int i = 0; i < vertexs.size(); i++) {
            int vertex_from = vertexs.at(i);
            for (int j = 0; j < transitions.size(); j++) {
                trans it = transitions.at(j);
                if (it.vertex_from == vertex_from && it.trans_symbol != '^') {
                    result.push_back(it.trans_symbol);
                }
            }
        }

        return result;
    }

    std::vector<int> eclosure(const std::vector<int>& X) {
        std::vector<int> result;
        std::vector<bool> visited (get_vertex_count(), false);

        for (int i = 0; i < X.size(); i++) {
            eclosure(X.at(i), result, visited);
        }

        sort(result.begin(), result.end());
        unique(result.begin(), result.end());
#ifdef DEBUG
        cout << "eclosure{" << join(X, ",") << "}  ->  "<< join(result, ",") << endl;
#endif
        return result;
    }

    void eclosure(int x, std::vector<int>& result, std::vector<bool>& visited) {
        result.push_back(x);

        for (int i = 0; i < transitions.size(); i++) {
            trans it = transitions.at(i);
            if (it.vertex_from == x && it.trans_symbol == '^') {
                int y = it.vertex_to;
                if (!visited.at(y)) {
                    visited.at(y) = true;
                    eclosure(y, result, visited);
                }
            }
        }
    }

    std::vector<int> move(const std::vector<int>& T, const char trans_symbol) {
        std::vector<int> result;

        for (int j = 0; j < T.size(); j++) {
            int t = T.at(j);

            for (int i = 0; i < transitions.size(); i++) {
                trans it = transitions.at(i);
                if (it.vertex_from == t && it.trans_symbol == trans_symbol) {
                    result.push_back(it.vertex_to);
                }
            }
        }

        std::sort(result.begin(), result.end());

#ifdef DEBUG
        int l1 = result.size();
        std::unique(result.begin(), result.end());
        int l2 = result.size();
        if (l2 < l1) {
            cerr << "move(T, a) return non-unique vector" << endl;
            exit(1);
        }
#endif

        return result;
    }

};

std::string join(std::vector<int> v, std::string delim);

class DFA {
public:

    std::vector<trans>        transitions;
    std::vector<std::vector<int> > entries;
    std::vector<bool>         marked;
    std::vector<int>          final_states;

    /**
     * Add newly_created entry into DFA
     */
    int add_entry(std::vector<int> entry) {
        entries.push_back(entry);
        marked.push_back(false);
        return entries.size() - 1;
    }

    /**
     * Return the array position of the next unmarked entry
     */
    int next_unmarked_entry_idx() {
        for (int i = 0; i < marked.size(); i++) {
            if (!marked.at(i)) {
                return i;
            }
        }

        return -1;
    }

    /**
     * mark the entry specified by index as marked (marked = true)
     */
    void mark_entry(int idx) {
        marked.at(idx) = true;
    }

    std::vector<int> entry_at(int i) {
        return entries.at(i);
    }

    int find_entry(std::vector<int> entry) {
        for (int i = 0; i < entries.size(); i++) {
            std::vector<int> it = entries.at(i);
            if (it == entry) {
                return i;
            }
        }

        return -1;
    }

    void set_final_state(int nfa_fs) {
        for (int i = 0; i < entries.size(); i++) {
            std::vector<int> entry = entries.at(i);

            for (int j = 0; j < entry.size(); j++) {
                int vertex = entry.at(j);
                if (vertex == nfa_fs) {
                    final_states.push_back(i);
                }
            }
        }

    }

    std::string get_final_state() {
        return join(final_states, ",");
    }

    void set_transition(int vertex_from, int vertex_to, char trans_symbol) {
        trans new_trans;
        new_trans.vertex_from = vertex_from;
        new_trans.vertex_to = vertex_to;
        new_trans.trans_symbol = trans_symbol;
        transitions.push_back(new_trans);
    }


    void display() {
        trans new_trans;
        std::cout<<"\n";
        for(int i = 0; i < transitions.size(); i++) {
            new_trans = transitions.at(i);
            std::cout<<"q"<<new_trans.vertex_from<<" {"<<join(entries.at(new_trans.vertex_from), ",")
                <<"} -> q"<<new_trans.vertex_to<<" {"<<join(entries.at(new_trans.vertex_to), ",")
                <<"} : Symbol - "<<new_trans.trans_symbol<<std::endl;
        }
        std::cout<<"\nThe final state is q : "<<join(final_states, ",")<<std::endl;
    }
};

NFA concat(NFA a, NFA b);
NFA kleene(NFA a);
NFA or_selection(std::vector<NFA> selections, int no_of_selections);
NFA re_to_nfa(std::string re);
DFA nfa_to_dfa(NFA nfa);

#endif //DIRECTORY_TEST_NFA_H