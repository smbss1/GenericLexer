#ifndef _REGEX_PATTERN_H_
#define _REGEX_PATTERN_H_

#include "RegexState.h"

class RegexPattern {
public:
    RegexPattern() : m_pState(NULL), m_nStartIndex(-1) {};
    RegexPattern(const RegexPattern &other){
		*this = other; 
	}
	virtual ~RegexPattern() {};
    RegexPattern& operator=(const RegexPattern& other){
		m_pState		= other.m_pState;
		m_nStartIndex	= other.m_nStartIndex;
		return *this;
	}
	RegexState* m_pState;
	int m_nStartIndex;
private:
protected:
};

#endif