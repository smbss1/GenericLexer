
#include <sstream>
#include "RegexState.h"

RegexState::~RegexState() {
	m_NFAStates.clear();
	m_Transition.clear();
}

RegexState::RegexState(std::set<RegexState*> NFAState, int nID) {
	m_NFAStates = NFAState;
	m_nStateID = nID;
	m_bAcceptingState = false;
	m_GroupID = 0;
	StateIterator iter;
	for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
		if((*iter)->m_bAcceptingState) {
			m_bAcceptingState = true;
		}
	}
}
RegexState::RegexState(const RegexState &other){ 
	*this = other;
}

void RegexState::AddTransition(char inputCh, RegexState *pState){
	m_Transition.insert(std::make_pair(inputCh, pState));
}

void RegexState::RemoveTransition(RegexState* pState) {
	std::multimap<char, RegexState*>::iterator iter;
	for(iter = m_Transition.begin(); iter != m_Transition.end();) {
		RegexState *toState = iter->second;
		if(toState == pState)
			m_Transition.erase(iter++);
		else 
			++iter;
	}
}

void RegexState::GetTransition(char inputCh, Table &States) {
	States.clear();
	std::multimap<char, RegexState*>::iterator iter;
	for(iter = m_Transition.lower_bound(inputCh); iter != m_Transition.upper_bound(inputCh); ++iter) {
		RegexState *pState = iter->second;
		States.push_back(pState);
	}
}

std::set<RegexState*>& RegexState::GetNFAState() { 
	return m_NFAStates; 
}

bool RegexState::IsDeadEnd() {
	if(m_bAcceptingState)
		return false;
	if(m_Transition.empty())
		return true;
	std::multimap<char, RegexState*>::iterator iter;
	for(iter=m_Transition.begin(); iter!=m_Transition.end(); ++iter){
		RegexState *toState = iter->second;
		if(toState != this)
			return false;
	}
	return true;
}	

RegexState& RegexState::operator=(const RegexState& other) { 
	this->m_Transition	= other.m_Transition; 
	this->m_nStateID	= other.m_nStateID;
	this->m_NFAStates	= other.m_NFAStates;
	return *this;
}

bool RegexState::operator==(const RegexState& other) {
	if(m_NFAStates.empty())
		return(m_nStateID == other.m_nStateID);
	else 
		return(m_NFAStates == other.m_NFAStates);
}
std::string RegexState::getStringID(){
	std::string result;
	std::stringstream out;
	//if(m_bAcceptingState){
	//	out << "{" << m_nStateID << "}";
	//} else {
		out << m_nStateID;
	//}
	return out.str();
}



