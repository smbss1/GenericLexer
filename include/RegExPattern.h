#ifndef _REGEX_PATTERN_H_
#define _REGEX_PATTERN_H_

#include "RegexState.h"

class RegexPattern {
public:
    RegexPattern() : m_pState(NULL), m_iStartIndex(-1) {};

    RegexPattern(const RegexPattern &other)
    {
		*this = other; 
	}

	virtual ~RegexPattern() {};

    RegexPattern& operator=(const RegexPattern& other){
		m_pState		= other.m_pState;
        m_iStartIndex	= other.m_iStartIndex;
		return *this;
	}
	RegexState* m_pState;
	int m_iStartIndex;
private:
protected:
};

#endif