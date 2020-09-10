/*
** EPITECH PROJECT, 2019
** directory_test
** File description:
** Created by besseausamuel
*/

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdarg.h>
#include <stack>
#include "Lexer.h"
#include "DFA.h"
#include "NFA.h"
//
//void Regex(DFA& oDfa, const std::string& strId, std::string strValue)
//{
//    // m_oAllDefines.insert(make_pair(strId, strValue));
//    if (!oDfa.StateExist(strId))
//        oDfa.AddState(DFAState(false, strId));
//
//    int iState = oDfa.GetStateID(strId);
//    int nextId;
//    int prevId = 0;
//
//    for (int i = 0; i < strValue.size(); i++) {
//        if (strValue[i] == '[')
//        {
//            while (strValue[i] != ']' && strValue[i])
//            {
//                char first = strValue[i];
//                if (strValue[i + 1] == '-')
//                {
//                    char last = strValue[i + 2];
//                    for (char c = first; c <= last; c++)
//                        oDfa.AddTransition(prevId, c, iState);
////                    if (bStar) {
////                        bStar = false;
////                        for (char c = first; c <= last; c++)
////                            oDfa.AddTransition(iState, c, iState);
////                    }
//                    i += 2;
//                }
//                else if (first != '[' && first != ']') {
//                    oDfa.AddTransition(prevId, first, iState);
//                }
//                i++;
//            }
//        }
//        else
//        {
//            if (i > 0 && i < strValue.size() - 1) {
//                nextId = oDfa.AddState(DFAState(false, string(1, strValue[i])));
//            } else if (i < strValue.size()) {
//                nextId = oDfa.AddState(DFAState(false, string(1, strValue[i]), true));
//            } else {
//                nextId = iState;
//            }
//            oDfa.AddTransition(prevId, strValue[i], nextId);
//            prevId = nextId;
//        }
//    }
//}
//
//class NFA;
//
//class NFAState
//{
//    /*
//     * WARNING:
//     *
//     * The maximum integer character code we'll match is 255, which
//     * is sufficient for the ASCII character set.
//     *
//     * If we were to use this on the Unicode character set, we'd get
//     * an array index out-of-bounds exception.
//     *
//     * A ``proper'' implementation of this would not use arrays but
//     * rather a dynamic data structure like Vector.
//     */
//    int MAX_CHAR = 255;
//    std::vector<std::vector<NFAState>> onChar = std::vector<std::vector<NFAState>>(MAX_CHAR) ;
//    std::vector<NFAState> onEmpty;
//
//    bool matches(std::string s, std::vector<NFAState*> visited) {
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
//
//        if (std::find(visited.begin(), visited.end(), this) != visited.end())
//            /* We've found a path back to ourself through empty edges;
//             * stop or we'll go into an infinite loop. */
//            return false ;
//
//        /* In case we make an empty transition, we need to add this
//         * state to the visited list. */
//        visited.push_back(this) ;
//
//        if (s.size() == 0) {
//            /* The string is empty, so we match this string only if
//             * this state is a final state, or we can reach a final
//             * state without consuming any input. */
//            if (isFinal)
//                return true ;
//
//            /* Since this state is not final, we'll ask if any
//             * neighboring states that we can reach on empty edges can
//             * match the empty string. */
//            for (NFAState next : onEmpty) {
//                if (next.matches("",visited))
//                    return true ;
//            }
//            return false ;
//        } else {
//            /* In this case, the string is not empty, so we'll pull
//             * the first character off and check to see if our
//             * neighbors for that character can match the remainder of
//             * the string. */
//
//            int c = (int)s[0] ;
//
//            for (NFAState next : onChar[c]) {
//                if (next.matches(s.substr(1)))
//                    return true ;
//            }
//
//            /* It looks like we weren't able to match the string by
//             * consuming a character, so we'll ask our
//             * empty-transition neighbors if they can match the entire
//             * string. */
//            for (NFAState next : onEmpty) {
//                if (next.matches(s,visited))
//                    return true ;
//            }
//            return false ;
//        }
//    }
//
//public:
//    bool matches(std::string s) {
//        return matches(s, std::vector<NFAState*>()) ;
//    }
//
//    NFAState () {
//        for (auto & i : onChar)
//            i = std::vector<NFAState>() ;
//    }
//
//    bool isFinal = false;
//
///*
// * Add a transition edge from this state to next which consumes
// * the character c.
// */
//void addCharEdge(char c, NFAState next) {
//    onChar[(int) c].push_back(next) ;
//}
//
///*
// * Add a transition edge from this state to next that does not
// * consume a character.
// */
//void addEmptyEdge(NFAState next) {
//    onEmpty.push_back(next) ;
//}
//};
//
//template<typename Base, typename T>
//inline bool instanceof(const T*) {
//    return std::is_base_of<Base, T>::value;
//}
//
///*
//     * e() : Creates an NFA which matches the empty string.
//     */
//NFA e() {
//    NFAState entry  = NFAState() ;
//    NFAState exit = NFAState() ;
//    entry.addEmptyEdge(exit) ;
//    exit.isFinal = true ;
//    return NFA(entry, exit) ;
//}
//
///* Syntactic sugar. */
//template<typename T>
//NFA re(const T* o) {
//    if (instanceof<NFA>(o))
//        return *((NFA*) o);
//    else if (instanceof<char>(o))
//        return c(*((char*) o)) ;
//    else if (instanceof<std::string>(o))
//        return fromString(*((std::string*) o)) ;
//    else {
//        std::cout << "bad regex" << std::endl;
//    }
//}
//
///*
// * Here, an NFA is represented by an entry state and an exit state.
// *
// * Any NFA can be represented by an NFA with a single exit state by
// * creating a special exit state, and then adding empty transitions
// * from all final states to the special one.
// *
// */
//class NFA
//{
//    NFAState m_entry ;
//    NFAState m_exit ;
//
//    /*
//      * c() : Creates an NFA which just matches the character `c'.
//      */
//    NFA c(char c) {
//        NFAState entry = NFAState() ;
//        NFAState exit = NFAState() ;
//        exit.isFinal = true ;
//        entry.addCharEdge(c,exit) ;
//        return NFA(entry, exit) ;
//    }
//
//    /*
//     * s() : Creates an NFA that matches a sequence of the two
//     * provided NFAs.
//     */
//    NFA s(NFA first, NFA second) {
//        first.m_exit.isFinal = false ;
//        second.m_exit.isFinal = true ;
//        first.m_exit.addEmptyEdge(second.m_entry) ;
//        return NFA(first.m_entry,second.m_exit) ;
//    }
//
//    /*
//     * or() : Creates an NFA that matches either provided NFA.
//     */
//    NFA orr(NFA choice1, NFA choice2) {
//        choice1.m_exit.isFinal = false ;
//        choice2.m_exit.isFinal = false ;
//        NFAState entry = NFAState() ;
//        NFAState exit  = NFAState() ;
//        exit.isFinal = true ;
//        entry.addEmptyEdge(choice1.m_entry) ;
//        entry.addEmptyEdge(choice2.m_entry) ;
//        choice1.m_exit.addEmptyEdge(exit) ;
//        choice2.m_exit.addEmptyEdge(exit) ;
//        return NFA(entry,exit) ;
//    }
//
//    NFA fromString(std::string str) {
//        if (str.size() == 0)
//            return e() ;
//        else
//            return s(re(&str[0]), fromString(str.substr(1))) ;
//    }
//
//public:
//    bool matches(std::string str) {
//        return m_entry.matches(str);
//    }
//
//    NFA orr(std::vector<NFA> states) {
//        va_list args;
//        NFA exp = states[0];
//        for (int i = 1; i < states.size(); i++) {
//            exp = orr(exp, re(&states[i])) ;
//        }
//        va_end(args);
//        return exp ;
//    }
//
///*
// * rep() : Creates an NFA which matches zero or more repetitions
// * of the given NFA.
// */
//    NFA rep(NFA nfa) {
//        nfa.m_exit.addEmptyEdge(nfa.m_entry) ;
//        nfa.m_entry.addEmptyEdge(nfa.m_exit) ;
//        return nfa ;
//    }
//
//    NFA(NFAState entry, NFAState exit) {
//        m_entry = std::move(entry) ;
//        m_exit = std::move(exit);
//    }
//};
//
//NFA s(std::vector<NFA> states) {
//    NFA exp = e() ;
//    for (int i = 0; i < states.size(); i++) {
//        exp = s(exp, re(&states[i])) ;
//    }
//    return exp ;
//}

//NFA character(const std::string& strSymbol)
//{
//    State inState = State(strSymbol);
//    State outState = State(strSymbol);
//    outState.m_bAccept = true;
//
//    inState.AddTransitionForSymbol(strSymbol, outState);
//
//    return NFA(inState, outState);
//}
//
//NFA epsilon()
//{
//    return character(EPSILON);
//}
//
//NFA ConcatPair(NFA& oFirst, NFA& oSecond)
//{
//    oFirst.m_oOutState.AddTransitionForSymbol(EPSILON, oSecond.m_oInState);
//
//    oFirst.m_oOutState.m_bAccept = false;
//    // oSecond.m_oOutState.m_bAccept = true;
//
//    State outState = State();
//    outState.m_bAccept = true;
//    oSecond.m_oOutState.m_bAccept = false;
//
//    oSecond.m_oOutState.AddTransitionForSymbol(EPSILON, outState);
//
//    return NFA(oFirst.m_oInState, outState);
//}
//
//NFA Concat(NFA oFirst, std::vector<NFA> oSecond)
//{
//    for (auto& fragment : oSecond) {
//        oFirst = ConcatPair(oFirst, fragment);
//    }
//    return oFirst;
//}
//
///**
// * @brief Union: a|b
// * @param oFirst
// * @param oSecond
// * @return
// */
//
//NFA OrPair(NFA oFirst, NFA oSecond)
//{
//    State inState = State();
//    State outState = State();
//
//    inState.AddTransitionForSymbol(EPSILON, oFirst.m_oInState);
//    inState.AddTransitionForSymbol(EPSILON, oSecond.m_oInState);
//    outState.m_bAccept = true;
//    oFirst.m_oOutState.m_bAccept = false;
//    oSecond.m_oOutState.m_bAccept = false;
//
//    oFirst.m_oOutState.AddTransitionForSymbol(EPSILON, outState);
//    oSecond.m_oOutState.AddTransitionForSymbol(EPSILON, outState);
//    return NFA(inState, outState);
//}
//
//NFA OrArray(NFA oFirst, const std::vector<NFA>& oSecond)
//{
//    for (auto& fragment : oSecond) {
//        oFirst = OrPair(oFirst, fragment);
//    }
//    return oFirst;
//}
//
///**
// * Kleene star/closure.
// *
// * a*
// */
//NFA RepExplicit(NFA oFragment) {
//    State inState = State();
//    State outState = State();
//    outState.m_bAccept = true;
//
//    // 0 or more.
//    inState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
//    inState.AddTransitionForSymbol(EPSILON, outState);
//
//    oFragment.m_oOutState.m_bAccept = false;
//    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, outState);
//    outState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
//
//    return NFA(inState, outState);
//}
//
//NFA Rep(NFA oFragment)
//{
//    oFragment.m_oInState.AddTransitionForSymbol(EPSILON, oFragment.m_oOutState);
//    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
//    return oFragment;
//}
//
///**
// * Optimized Plus: just adds ε-transitions from
// * the output to the input.
// */
//NFA PlusRep(NFA oFragment) {
//    oFragment.m_oOutState.AddTransitionForSymbol(EPSILON, oFragment.m_oInState);
//    return oFragment;
//}
//
///**
// * Optimized ? repetition: just adds ε-transitions from
// * the input to the output.
// */
//NFA QuestionRep(NFA oFragment) {
//    oFragment.m_oInState.AddTransitionForSymbol(EPSILON, oFragment.m_oOutState);
//    return oFragment;
//}
//
//NFA FromEpsilon() {
//    State start = State();
//    State end = State();
//    start.AddTransitionForSymbol(EPSILON, end);
//
//    return NFA(start, end);
//}

string insert_concat(string regexp){
   string ret = "(";
   char c, c2;
   for(unsigned int i = 0; i < regexp.size(); i++){
       c = regexp[i];
       if(i + 1 < regexp.size()){
           c2 = regexp[i + 1];
           ret += c;
           if(c != '(' && c2 != ')' && c != '+' && c2 != '+' && c2 != '*' && c != '|' && c2 != '|'){
               ret += '.';
           }
       }
   }
   ret += regexp[regexp.size() - 1];
   ret += ')';
   return ret;
}

// int priority(char c){
//    switch(c){
//        case '*': return 3;
//        case '.': return 2;
//        case '+': return 1;
//        default: return 0;
//    }
// }

// std::string regexp_to_postfix(string regexp)
// {
//    std::string postfix = "";
//    std::stack<char> op;
//    char c;
//    for(unsigned int i = 0; i < regexp.size(); i++)
//    {
//        if (isalpha(regexp[i]))
//        {
//            postfix += regexp[i];

//        } else {
//            switch (regexp[i]) {
//                case '(':
//                    op.push(regexp[i]);
//                    break;
//                case ')':
//                    while (op.top() != '(') {
//                        postfix += op.top();
//                        op.pop();
//                    }
//                    op.pop();
//                    break;
//                default:
//                    while (!op.empty()) {
//                        c = op.top();
//                        if (priority(c) >= priority(regexp[i])) {
//                            postfix += op.top();
//                            op.pop();
//                        } else break;
//                    }
//                    op.push(regexp[i]);
//            }
//        }
//        //cout<<regexp[i]<<' '<<postfix<<endl;
//    }
//    while(!op.empty())
//    {
//        postfix += op.top();
//        op.pop();
//    }
//    return postfix;
// }
//
//NFA toNFA(const std::string& postfixExp) {
//    if(postfixExp.empty()) {
//        std::cout << "Empty" << std::endl;
//        return FromEpsilon();
//    }
//
//    std::stack<NFA> vStack;
//
//    for (auto& token : postfixExp) {
//        if(token == '*') {
//            std::cout << "Rep" << std::endl;
//            auto val = vStack.top();
//            vStack.pop();
//            vStack.push(Rep(val));
//        } else if (token == '|') {
//            std::cout << "Or" << std::endl;
//            NFA right = vStack.top();
//            vStack.pop();
//            NFA left = vStack.top();
//            vStack.pop();
//            vStack.push(OrPair(left, right));
//        } else if (token == '.') {
//            std::cout << "Concat" << std::endl;
//            NFA right = vStack.top();
//            vStack.pop();
//            NFA left = vStack.top();
//            vStack.pop();
//            vStack.push(ConcatPair(left, right));
//        } else {
//            std::cout << "Character" << std::endl;
//            vStack.push(character(std::string(1, token)));
//        }
//    }
//    std::cout << vStack.size() << std::endl;
//    NFA val = vStack.top();
//    vStack.pop();
//    return val;
//}
//
//bool isMatch(string text, string pattern) {
//    if (pattern.empty()) return text.empty();
//    bool first_match = (!text.empty() &&
//                        (pattern[0] == text[0] || pattern[0] == '.'));
//
//    if (pattern.size() >= 2 && pattern[1] == '*'){
//        return (isMatch(text, pattern.substr(2)) ||
//                (first_match && isMatch(text.substr(1), pattern)));
//    } else {
//        return first_match && isMatch(text.substr(1), pattern.substr(1));
//    }
//}

int main(int argc, char *argv[])
{
    // std::cout << "\n\nThe Thompson's Construction Algorithm takes a regular expression as an input " << "and returns its corresponding Non-Deterministic Finite Automaton \n\n";
    // std::cout << "\n\nThe basic building blocks for constructing the NFA are : \n";

    // NFA a, b;

    // std::cout << "\nFor the regular expression segment : (a)";
    // a.SetVertex(2);
    // a.SetTransition(0, 1, 'a');
    // a.SetFinalState(1);
    // a.Display();
    // //	getch();

    // std::cout << "\nFor the regular expression segment : (b)";
    // b.SetVertex(2);
    // b.SetTransition(0, 1, 'b');
    // b.SetFinalState(1);
    // b.Display();
    // //	getch();

    // std::cout << "\nFor the regular expression segment [Concatenation] : (a.b)";
    // NFA ab = concat(a, b);
    // ab.Display();
    // //	getch();

    // std::cout << "\nFor the regular expression segment [Kleene Closure] : (a*)";
    // NFA a_star = kleene(a);
    // a_star.Display();
    // //	getch();

    // std::cout << "\nFor the regular expression segment [Or] : (a|b)";
    // int no_of_selections;
    // no_of_selections = 2;
    // std::vector<NFA> selections(no_of_selections, NFA());
    // selections.at(0) = a;
    // selections.at(1) = b;
    // NFA a_or_b = or_selection(selections, no_of_selections);
    // a_or_b.Display();
    // //	getch();


    string regex;

    std::cout << "\n*****\t*****\t*****\n";
    std::cout << "\nFORMAT : \n"
        << "> Explicitly mention concatenation with a '.' operator \n"
        << "> Enclose every concatenation and or section by parantheses \n"
        << "> Enclose the entire regular expression with parantheses \n\n";

    std::cout << "For example : \nFor the regular expression (a.(b|c))  -- \n";
    NFA example_nfa = re_to_nfa("(a.(b|c))");
    example_nfa.Display();

    std::cout << "\n\nEnter the regular expression in the above mentioned format - \n\n";
    std::cin >> regex;
    std::string conc = insert_concat(regex);

    std::cout << "\n\nThe required NFA has the transitions : \n\n";

    NFA required_nfa;
    required_nfa = re_to_nfa(conc);
    required_nfa.Display();

    std::cout << "\n\n==> DFA : \n\n";

    DFA required_dfa = nfa_to_dfa(required_nfa);
    required_dfa.Display();

    return 0;
}