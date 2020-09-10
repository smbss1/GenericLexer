#pragma once

// #include <set>
// #include <map>
// //
//template<typename T>
//class DFA
//{
//public:
//	explicit DFA(int initial, bool isfinal);
//	~DFA();
//	void AddState(int s, bool isfinal);
//	void AddTransition(int src, T input, int dest);
//	bool IsAccepting();
//    bool IsAccepting(T inp);
//    void Reset();
//	int Input(T inp);
//	int State();
//
//
//private:
//	int m_iInitialState;
//	int m_iState;
//    std::set<int> m_iStates;
//    std::set<int> m_iFinalStates;
//    std::map<std::pair<int, T>, int> m_iTransitions;
//};




#ifndef _DFA_H_
#define _DFA_H_

#define MAX_TRANSITIONS 50
#define MAX_STATES 100

#include <string>
#include <vector>
#include "NFA.h"

//
//struct DFATransition
//{
//    DFATransition(char cValue, int iToStateID);
//    DFATransition(bool (*fn)(char), int iToStateID);
//
//    bool (*condition)(char); //a bool-returning and char-taking function pointer which is used to test whether this transition is to be taken.
//    char m_cValue;
//    int m_iToStateID;
//
//    bool operator==(const DFATransition& oTransition) const
//    {
//        return  oTransition.condition == condition &&
//                oTransition.m_cValue == m_cValue &&
//                oTransition.m_iToStateID == m_iToStateID;
//    }
//};
//
//struct DFAState
//{
//    int m_iID; //unique ID associated with every state
//    bool m_bHasAction; //indicates whether any action should be taken when DFA is in this state
//    bool m_bIsFinal;
//    int m_iNumberOfTransitions; //number of outgoing transitions excluding the default (other) transition
//    std::string m_strActionName; //the string based on which action is taken
//    std::vector<DFATransition> m_oTransitions; //list of outgoing transitions
//    int m_iDefaultToStateID; //the default (other) transition. This is taken when no other transition is possible
//
//    DFAState(bool bHasAction, std::string strActionName, bool bIsFinal = false);
//
//public:
//    bool operator==(const DFAState & oState) const
//    {
//        return  oState.m_iID == m_iID &&
//                oState.m_bHasAction == m_bHasAction &&
//                oState.m_bIsFinal == m_bIsFinal &&
//                oState.m_iNumberOfTransitions == m_iNumberOfTransitions &&
//                oState.m_strActionName == m_strActionName &&
//                oState.m_oTransitions == m_oTransitions &&
//                oState.m_iDefaultToStateID == m_iDefaultToStateID;
//    }
//};
//
//class DFA
//{
//    int m_iStartStateID;
//    int m_iNumberOfStates;
//    std::vector<DFAState> m_oStates;
//    std::vector<DFAState> m_oFinalStates;
//
//public:
//    DFA();
//
//
//    int m_iCurrentStateID;
//    int m_iPreviousStateID;
//
//
//    void Reset(); //makes the dfa ready for consumption. i.e. sets the current state to start state.
//    bool MakeNextTransition(char c);
//    int AddState(DFAState oNewState);
//    void AddTransition(int iFromStateID, bool (*condition)(char), int iToStateID);
//
//    void AddTransition(int iFromStateID, char value, int iToStateID);
//
//    int GetStateID(const std::string& strActionName);
//
//    void SetStartState(const std::string &strActionName);
//
//    bool StateChanged();
//
//    std::string GetStateName(int iId);
//
//    bool StateExist(const std::string &strActionName);
//
//    bool IsAccepting();
//
//    bool IsAccepting(char inp);
//
//    void PrintStates();
//
//    void PrintAllTransitions();
//
//    void PrintTransition(DFAState &oState);
//
//    DFATransition &GetTransition(int iFromId, char c, int iToId);
//
//    void ReplaceTransition(int iFromStateID, char cValue, int iPrevToStateId, int iNewToStateID);
//};

std::string Join(std::vector<int> v, std::string delim);

class DFA {
public:

    std::vector<trans>        transitions;
    std::vector<std::vector<int>> m_vEntries;
    std::vector<bool>         vMarked;
    std::vector<int>          vFinalStates;

    /**
     * Add newly_created entry into DFA
     */
    int AddEntry(std::vector<int> vEntry) {
        m_vEntries.push_back(vEntry);
        vMarked.push_back(false);
        return m_vEntries.size() - 1;
    }

    /**
     * Return the array position of the next unmarked entry
     */
    int NextUnmarkedEntryIndex() {
        for (int i = 0; i < vMarked.size(); i++) {
            if (!vMarked.at(i)) {
                return i;
            }
        }
        return -1;
    }

    /**
     * mark the entry specified by index as marked (marked = true)
     */
    void MarkEntry(int idx) {
        vMarked.at(idx) = true;
    }

    std::vector<int> EntryAt(int i) {
        return m_vEntries.at(i);
    }

    int FindEntry(std::vector<int> entry) {
        for (int i = 0; i < m_vEntries.size(); i++) {
            std::vector<int> vIt = m_vEntries.at(i);
            if (vIt == entry) {
                return i;
            }
        }

        return -1;
    }

    void SetFinalState(int iNfaFS) {
        for (int i = 0; i < m_vEntries.size(); i++) {
            std::vector<int> entry = m_vEntries.at(i);

            for (int j = 0; j < entry.size(); j++) {
                int vertex = entry.at(j);
                if (vertex == iNfaFS) {
                    vFinalStates.push_back(i);
                }
            }
        }

    }

    std::string GetFinalState() {
        return Join(vFinalStates, ",");
    }

    void SetTransition(int vertex_from, int vertex_to, char trans_symbol) {
        trans oNewTransition;
        oNewTransition.vertex_from = vertex_from;
        oNewTransition.vertex_to = vertex_to;
        oNewTransition.trans_symbol = trans_symbol;
        transitions.push_back(oNewTransition);
    }


    void Display();
    bool Match(std::string strText);
    void GetTransition(int iFrom, char cSymbol, std::vector<trans>& vOutTransition);
};

#endif