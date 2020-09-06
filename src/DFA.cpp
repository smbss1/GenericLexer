#include "DFA.h"

//template<typename T>
//DFA<T>::DFA(int iInitialState, bool bIsfinal)
//{
//	this->m_iInitialState = this->m_iState = iInitialState;
//    AddState(iInitialState, bIsfinal);
//    AddState(-1, false); // invalid dead state
//}
//
//template<typename T>
//DFA<T>::~DFA()
//{
//}
//
//template<typename T>
//void DFA<T>::AddState(int s, bool bIsfinal)
//{
//	m_iStates.insert(s);
//	if(bIsfinal)
//        m_iFinalStates.insert(s);
//}
//
//template<typename T>
//void DFA<T>::AddTransition(int src, T input, int dest)
//{
//	m_iTransitions.insert(std::pair<std::pair<int,T>, int>(std::pair<int,T>(src, input), dest));
//}
//
//template<typename T>
//void DFA<T>::Reset()
//{
//	m_iState = m_iInitialState;
//}
//
//template<typename T>
//int DFA<T>::Input(T inp)
//{
//	auto tr = std::make_pair(m_iState, inp);
//	if(m_iTransitions.count(tr) > 0)
//	{
//		auto it = m_iTransitions.find(tr);
//		return m_iState = it->second;
//	}
//	else
//		return m_iState = -1;
//}
//
//template<typename T>
//bool DFA<T>::IsAccepting()
//{
//	return m_iFinalStates.count(m_iState) != 0;
//}
//
//template<typename T>
//bool DFA<T>::IsAccepting(T inp)
//{
//    auto tr = std::make_pair(m_iState, inp);
//    if(m_iTransitions.count(tr) > 0)
//    {
//        auto it = m_iTransitions.find(tr);
//        return m_iFinalStates.count(it->second) != 0;
//    }
//    else
//        return false;
//}
//
//template<typename T>
//int DFA<T>::State()
//{
//	return m_iState;
//}
//
//
//template class DFA<char>;






#include <utility>
#include <iostream>

//void dfa_addState(DFA* pDFA, DFAState* newState)
//{
//    newState->ID = pDFA->numberOfStates;
//    pDFA->states[pDFA->numberOfStates] = newState;
//    pDFA->numberOfStates++;
//}

//void dfa_addTransition(DFA* dfa, int fromStateID, int(*condition)(char), int toStateID)
//{
//    DFAState* state = dfa->states[fromStateID];
//    state->transitions[state->numberOfTransitions].condition = condition;
//    state->transitions[state->numberOfTransitions].toStateID = toStateID;
//    state->numberOfTransitions++;
//
//}

//DFAState* dfa_createState(int hasAction, char* actionName)
//{
//    DFAState* newState = (DFAState*)malloc(sizeof(DFAState));
//    strcpy(newState->actionName, actionName);
//    newState->defaultToStateID = -1;
//    newState->hasAction = hasAction;
//    newState->ID = -1;
//    newState->numberOfTransitions = 0;
//
//    return newState;
//
//}

//void dfa_reset(DFA* dfa)
//{
//    dfa->currentStateID = dfa->startStateID;
//}

DFAState::DFAState(bool bHasAction, std::string strActionName)
{
    m_strActionName = std::move(strActionName);
    m_iDefaultToStateID = -1;
    m_bHasAction = bHasAction;
    m_iID = -1;
    m_iNumberOfTransitions = 0;
}

DFAState::DFAState(bool bHasAction, std::string strActionName, int iDefaultToStateID)
{
    m_strActionName = std::move(strActionName);
    m_iDefaultToStateID = iDefaultToStateID;
    m_bHasAction = bHasAction;
    m_iID = -1;
    m_iNumberOfTransitions = 0;
}

DFATransition::DFATransition(char cValue, int iToStateID)
{
    condition = nullptr;
    m_cValue = cValue;
    m_iToStateID = iToStateID;
}

DFATransition::DFATransition(bool (*fn)(char), int iToStateID)
{
    condition = fn;
    m_cValue = 0;
    m_iToStateID = iToStateID;
}

DFA::DFA()
{
    m_iNumberOfStates = 0;
    m_iStartStateID = 0;
    m_iCurrentStateID = -1;
    m_iPreviousStateID = -1;
    AddState(DFAState(false, "START"));
}

void DFA::SetStartState(const std::string& strActionName)
{
    for (auto& itState : m_oStates) {
        if (itState.m_strActionName == strActionName) {
            m_iStartStateID = itState.m_iID;
            break;
        }
    }
}

void DFA::Reset()
{
    m_iPreviousStateID = m_iCurrentStateID;
    m_iCurrentStateID = m_iStartStateID;
}

bool DFA::StateChanged()
{
    return m_iPreviousStateID != m_iCurrentStateID;
}

bool DFA::MakeNextTransition(char c)
{
    DFAState& oCurrentState = m_oStates[m_iCurrentStateID];
    m_iPreviousStateID = m_iCurrentStateID;
    for (int i = 0; i < oCurrentState.m_iNumberOfTransitions; i++)
    {
        if (oCurrentState.m_oTransitions[i].m_cValue == c)
        {
            m_iCurrentStateID = oCurrentState.m_oTransitions[i].m_iToStateID;
            return true;
        }
    }

    //take the default transition
    m_iCurrentStateID = oCurrentState.m_iDefaultToStateID;
    return false;
}

void DFA::AddState(DFAState oNewState)
{
    oNewState.m_iID = m_iNumberOfStates;
    m_oStates.push_back(oNewState);
    m_iNumberOfStates++;
}

int DFA::GetStateID(const std::string& strActionName)
{
    for (auto& itState : m_oStates) {
        if (itState.m_strActionName == strActionName) {
            return itState.m_iID;
        }
    }
}

bool DFA::StateExist(const std::string& strActionName)
{
    for (auto& itState : m_oStates) {
        if (itState.m_strActionName == strActionName)
            return true;
    }
    return false;
}

std::string DFA::GetStateName(int iId)
{
    if (iId > -1 && iId < m_oStates.size())
        return m_oStates[iId].m_strActionName;
    else
        return "";
}

void DFA::AddTransition(int iFromStateID, bool (*condition)(char), int iToStateID)
{
    DFAState& state = m_oStates[iFromStateID];
    state.m_oTransitions.emplace_back(condition, iToStateID);
    state.m_iNumberOfTransitions++;
}

void DFA::AddTransition(int iFromStateID, char cValue, int iToStateID)
{
    DFAState& state = m_oStates[iFromStateID];
    state.m_oTransitions.emplace_back(cValue, iToStateID);
    state.m_iNumberOfTransitions++;
}
