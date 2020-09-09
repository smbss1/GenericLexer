//
// Created by besseausamuel on 08/09/2020.
//

#pragma once

#ifndef DIRECTORY_TEST_NFA_H
#define DIRECTORY_TEST_NFA_H

#include <map>
#include <utility>
#include <vector>
#include <iostream>

const std::string EPSILON = "ε";

class State
{

public:
    // bool isFinal = false;
    void AddTransitionForSymbol(const std::string& strSymbol, const State& oState);
    std::vector<State> GetTransitionForSymbol(const std::string& strSymbol);

    State(bool bAccept = false);

    bool test(const std::string &basicString);

    bool m_bAccept;

    bool matches(std::string s) {
        return matches(s, std::vector<State*>()) ;
    }

    bool matches(std::string s, std::vector<State*> visited) {
        /*
         * When matching, we work character by character.
         *
         * If we're out of characters in the string, we'll check to
         * see if this state if final, or if we can get to a final
         * state from here through empty edges.
         *
         * If we're not out of characters, we'll try to consume a
         * character and then match what's left of the string.
         *
         * If that fails, we'll ask if empty-edge neighbors can match
         * the entire string.
         *
         * If that fails, the match fails.
         *
         * Note: Because we could have a circular loop of empty
         * transitions, we'll have to keep track of the states we
         * visited through empty transitions so we don't end up
         * looping forever.
         */

        if (std::find(visited.begin(), visited.end(), this) != visited.end())
            /* We've found a path back to ourself through empty edges;
             * stop or we'll go into an infinite loop. */
            return false ;
        /* In case we make an empty transition, we need to add this
         * state to the visited list. */
        visited.push_back(this);

        if (s.empty()) {
            /* The string is empty, so we match this string only if
             * this state is a final state, or we can reach a final
             * state without consuming any input. */
            if (m_bAccept)
                return true;

            /* Since this state is not final, we'll ask if any
             * neighboring states that we can reach on empty edges can
             * match the empty string. */
            for (State next : GetTransitionForSymbol(EPSILON)) {
                if (next.matches("", visited))
                    return true;
            }
            return false;
        }
        /* In this case, the string is not empty, so we'll pull
         * the first character off and check to see if our
         * neighbors for that character can match the remainder of
         * the string. */

        int c = (int)s[0];

        for (State next : GetTransitionForSymbol(std::string(1, c))) {
            if (next.matches(s.substr(0,1)))
                return true ;
        }

        /* It looks like we weren't able to match the string by
         * consuming a character, so we'll ask our
         * empty-transition neighbors if they can match the entire
         * string. */
        for (State next : GetTransitionForSymbol(EPSILON)) {
            if (next.matches(s, visited))
                return true ;
        }
        return false;
    }

    std::map<std::string, std::vector<State>> m_oTransitionMap;
};

class NFA {

    bool test(const std::string& strString)
    {
        return m_oInState.test(strString);
    }

public:
    NFA();
    NFA(State inState, State outState);

    State m_oOutState;
    State m_oInState;

    bool matches(std::string& str) {
        return m_oInState.matches(str);
    }
};

#endif //DIRECTORY_TEST_NFA_H
