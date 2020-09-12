#ifndef _RegexState_H_
#define _RegexState_H_

#include <map>
#include <set>
#include <vector>
#include <string>

class RegexState
{
private:	
	std::set<RegexState*> m_NFAStates;	
public:
	typedef std::vector<RegexState*> Table;
	typedef std::set<RegexState*>::iterator StateIterator;
	std::multimap<char, RegexState*> m_Transition;	
	int m_nStateID;
	bool m_bAcceptingState;
	bool m_Marked;
	int  m_GroupID;

	RegexState() : m_nStateID(-1), m_bAcceptingState(false) {};
	RegexState(int nID) : m_nStateID(nID), m_bAcceptingState(false), m_GroupID(0) {};
	RegexState(std::set<RegexState*> NFAState, int nID);
	RegexState(const RegexState &other);

	virtual ~RegexState();

	void AddTransition(char inputCh, RegexState *pState);
	void RemoveTransition(RegexState* pState);
	void GetTransition(char inputCh, Table &States);
	std::set<RegexState*>& GetNFAState();

	bool IsDeadEnd();
	RegexState& operator=(const RegexState& other);
	bool operator==(const RegexState& other);

	std::string getStringID();
protected:
};
#endif
