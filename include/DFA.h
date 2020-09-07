#pragma once

#include <vector>
#include <set>
#include <map>
//
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


struct DFATransition
{
    DFATransition(char cValue, int iToStateID);
    DFATransition(bool (*fn)(char), int iToStateID);

    bool (*condition)(char); //a bool-returning and char-taking function pointer which is used to test whether this transition is to be taken.
    char m_cValue;
    int m_iToStateID;
};

struct DFAState
{
    DFAState(bool bHasAction, std::string strActionName, int iDefaultToStateID);

    int m_iID; //unique ID associated with every state
    bool m_bHasAction; //indicates whether any action should be taken when DFA is in this state
    int m_iNumberOfTransitions; //number of outgoing transitions excluding the default (other) transition
    std::string m_strActionName; //the string based on which action is taken
    std::vector<DFATransition> m_oTransitions; //list of outgoing transitions
    int m_iDefaultToStateID; //the default (other) transition. This is taken when no other transition is possible

    DFAState(bool bHasAction, std::string strActionName);
};

class DFA
{
    int m_iStartStateID;
    int m_iNumberOfStates;
    std::vector<DFAState> m_oStates;

public:
    DFA();


    int m_iCurrentStateID;
    int m_iPreviousStateID;


    void Reset(); //makes the dfa ready for consumption. i.e. sets the current state to start state.
    bool MakeNextTransition(char c);
    void AddState(DFAState oNewState);
    void AddTransition(int iFromStateID, bool (*condition)(char), int iToStateID);

    void AddTransition(int iFromStateID, char value, int iToStateID);

    int GetStateID(const std::string& strActionName);

    void SetStartState(const std::string &strActionName);

    bool StateChanged();

    std::string GetStateName(int iId);

    bool StateExist(const std::string &strActionName);
};

#endif