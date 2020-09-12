#include <algorithm>
#include <sstream>
#include <iostream>
#include <string.h>
#include "Regex.h"


#define EPSILON       -1 
#define CLOSURE       '*'
#define PLUS_CLOSURE  '+'
#define OPTIONAL      '?'
#define OPEN_PAREN    '('
#define CLOSE_PAREN   ')'
#define OR            '|'
#define FALSE_CONCAT  '&'


RegEx::RegEx() {
}
RegEx::~RegEx(){
    CleanUp();
}
bool RegEx::IsMetaChar( char inputCh ) {
    return(( inputCh == CLOSURE )      ||
           ( inputCh == OR )           ||
           ( inputCh == OPEN_PAREN )   ||
           ( inputCh == CLOSE_PAREN )  ||
           ( inputCh == PLUS_CLOSURE ) ||
           ( inputCh == FALSE_CONCAT ) ||
           ( inputCh == OPTIONAL ));
}
bool RegEx::IsInput(char inputCh)      { return(!IsMetaChar(inputCh));   }

void RegEx::CleanUp() {
    for(int i = 0; i < (int)m_oNFATable.size(); ++i) {
        delete m_oNFATable[i];
    }
    m_oNFATable.clear();

    for(int i = 0; i < (int)m_oDFATable.size(); ++i)
        delete m_oDFATable[i];
    m_oDFATable.clear();

    m_iNextStateID = 0;

    while(!m_CharacterStack.empty())
        m_CharacterStack.pop();

    while(!m_ExpressionStack.empty())
        m_ExpressionStack.pop();

    m_InputSet.clear();

}

bool RegEx::PopTable(Table  &NFATable) {
    if(m_CharacterStack.size()>0) {
        NFATable = m_CharacterStack.top();
        m_CharacterStack.pop();
        return true;
    }
    return false;
}

bool RegEx::Concatenate() {

    Table  LeftTable, RightTable;
    if(!PopTable(RightTable) || !PopTable(LeftTable))
        return false;

    (*(LeftTable.rbegin()))->AddTransition(EPSILON, (*(RightTable.begin())));
    LeftTable.insert(LeftTable.end(), RightTable.begin(), RightTable.end());
    m_CharacterStack.push(LeftTable);

    return true;
}

void RegEx::PushOnCharacterStack(char chInput) {

    RegexState *s0 = new RegexState(m_iNextStateID++);
    RegexState *s1 = new RegexState(m_iNextStateID++);

    s0->AddTransition(chInput, s1);

    Table NewSubTable;
    NewSubTable.push_back(s0);
    NewSubTable.push_back(s1);
    m_CharacterStack.push(NewSubTable);

    m_InputSet.insert(chInput);
}

bool RegEx::ClosureOptional() {

    Table PrevTable;
    if(!PopTable(PrevTable))
        return false;

    RegexState *LeftTable  = new RegexState(m_iNextStateID++);
    RegexState *RightTable = new RegexState(m_iNextStateID++);

    LeftTable->AddTransition(EPSILON, RightTable);
    LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
    ((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);

    PrevTable.insert( PrevTable.begin(), LeftTable );
    PrevTable.push_back ( RightTable);

    m_CharacterStack.push(PrevTable);

    return true;
}

bool RegEx::ClosurePlus() {
    Table PrevTable;
    if(!PopTable(PrevTable))
        return false;

    RegexState *LeftTable  = new RegexState(m_iNextStateID++);
    RegexState *RightTable = new RegexState(m_iNextStateID++);

    LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
    ((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);
    ((*(PrevTable.rbegin())))->AddTransition(EPSILON, ((*(PrevTable.begin()))));

    PrevTable.insert( PrevTable.begin(), LeftTable );
    PrevTable.push_back ( RightTable);

    m_CharacterStack.push(PrevTable);

    return true;
}
bool RegEx::Closure() {

    Table PrevTable;
    if(!PopTable(PrevTable))
        return false;

    RegexState *LeftTable  = new RegexState(m_iNextStateID++);
    RegexState *RightTable = new RegexState(m_iNextStateID++);

    LeftTable->AddTransition(EPSILON, RightTable);
    LeftTable->AddTransition(EPSILON, ((*(PrevTable.begin()))));
    ((*(PrevTable.rbegin())))->AddTransition(EPSILON, RightTable);
    ((*(PrevTable.rbegin())))->AddTransition(EPSILON, ((*(PrevTable.begin()))));

    PrevTable.insert( PrevTable.begin(), LeftTable );
    PrevTable.push_back ( RightTable);

    m_CharacterStack.push(PrevTable);

    return true;
}

bool RegEx::Or() {

    Table  UpperTable, LowerTable;

    if(!PopTable(LowerTable) || !PopTable(UpperTable))
        return false;

    RegexState *LeftTable	= new RegexState(m_iNextStateID++);
    RegexState *RightTable	= new RegexState(m_iNextStateID++);

    LeftTable->AddTransition(EPSILON, ((*(UpperTable.begin()))));
    LeftTable->AddTransition(EPSILON, ((*(LowerTable.begin()))));
    ((*(UpperTable.rbegin())))->AddTransition(EPSILON, RightTable);
    ((*(LowerTable.rbegin())))->AddTransition(EPSILON, RightTable);

    LowerTable.push_back(RightTable);
    UpperTable.insert(UpperTable.begin(),LeftTable);
    UpperTable.insert(UpperTable.end(), LowerTable.begin(), LowerTable.end());

    m_CharacterStack.push(UpperTable);
    return true;
}
void RegEx::EpsilonClosure(std::set<RegexState*> startSet, std::set<RegexState*> &result) {
    std::stack<RegexState*> UnVisitedStates;
    result.clear();
    result = startSet;
    StateIterator iter;
    for(iter = startSet.begin(); iter != startSet.end(); ++iter) {
        UnVisitedStates.push(*iter);
    }

    while(!UnVisitedStates.empty()) {
        RegexState* curState = UnVisitedStates.top();
        UnVisitedStates.pop();

        Table epsilonStates;
        curState->GetTransition(EPSILON, epsilonStates);

        TableIterator epsilonItr;

        for ( epsilonItr = epsilonStates.begin(); epsilonItr != epsilonStates.end(); ++epsilonItr ){
            if(result.find((*epsilonItr)) == result.end()) {
                result.insert((*epsilonItr));
                UnVisitedStates.push((*epsilonItr));
            }
        }
    }
}

void RegEx::Move(char chInput, std::set<RegexState*> NFAState, std::set<RegexState*> &Result) {

    Result.clear();
    StateIterator iter;
    for(iter = NFAState.begin(); iter != NFAState.end(); ++iter) {
        Table States;
        (*iter)->GetTransition(chInput, States);
        for(int index = 0; index < (int)States.size(); ++index){
            Result.insert(States[index]);
        }
    }
}

void RegEx::ConvertNFAtoDFA() {
    for(int i = 0; i < (int)m_oDFATable.size(); ++i)
        delete m_oDFATable[i];
    m_oDFATable.clear();

    if(m_oNFATable.size() == 0)
        return;

    std::set<RegexState*> NFAStartStateSet;
    NFAStartStateSet.insert(m_oNFATable[0]);

    std::set<RegexState*> DFAStartStateSet;
    EpsilonClosure(NFAStartStateSet, DFAStartStateSet);

    m_iNextStateID = 0;
    RegexState *DFAStartState = new RegexState(DFAStartStateSet, m_iNextStateID++);

    m_oDFATable.push_back(DFAStartState);

    Table UnVisitedStates;
    UnVisitedStates.push_back(DFAStartState);
    while(!UnVisitedStates.empty()) {

        RegexState* CurDFAState = UnVisitedStates[UnVisitedStates.size()-1];
        UnVisitedStates.pop_back();

        std::set<char>::iterator iter;
        for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
            std::set<RegexState*> MoveRes, EpsilonClosureRes;

            Move(*iter, CurDFAState->GetNFAState(), MoveRes);
            EpsilonClosure(MoveRes, EpsilonClosureRes);

            StateIterator MoveResItr;
            StateIterator EpsilonClosureResItr;

            bool bFound = false;
            RegexState *s   = NULL;
            for(int i = 0; i < (int)m_oDFATable.size(); ++i) {
                s = m_oDFATable[i];
                if(s->GetNFAState() == EpsilonClosureRes) {
                    bFound = true;
                    break;
                }
            }
            if(!bFound) {
                RegexState* U = new RegexState(EpsilonClosureRes, m_iNextStateID++);
                UnVisitedStates.push_back(U);
                m_oDFATable.push_back(U);
                CurDFAState->AddTransition(*iter, U);
            } else {
                CurDFAState->AddTransition(*iter, s);
            }
        }
    }
    ReduceDFA();
}
void RegEx::ReduceDFA() {
    std::set<RegexState*> DeadEndSet;
    for(int i = 0; i < (int)m_oDFATable.size(); ++i) {
        if(m_oDFATable[i]->IsDeadEnd()) {
            DeadEndSet.insert(m_oDFATable[i]);
        }
    }
    if(DeadEndSet.empty())
        return;
    StateIterator iter;
    for(iter = DeadEndSet.begin(); iter != DeadEndSet.end(); ++iter) {
        for(int i = 0; i < (int)m_oDFATable.size(); ++i)
            m_oDFATable[i]->RemoveTransition(*iter);

        TableIterator pos;
        for(pos = m_oDFATable.begin(); pos != m_oDFATable.end(); ++pos) {
            if(*pos == *iter) {
                break;
            }
        }
        m_oDFATable.erase(pos);
        delete *iter;
    }
}

void RegEx::MinimizeDFA () {
}

std::string RegEx::PreProcessBracket( std::string strRegEx) {
    std::string::size_type startPos, endPos, separatorPos;
    std::string ReplacedStrRegEx;

    startPos = strRegEx.find_first_of("[");
    endPos   = strRegEx.find_first_of("]");
    separatorPos = strRegEx.find_first_of("-");

    if ( startPos == std::string::npos || endPos == std::string::npos )
        return strRegEx;

    ReplacedStrRegEx += strRegEx.substr( 0, startPos );
    ReplacedStrRegEx.push_back('(');
    std::string result = strRegEx.substr( startPos + 1, endPos - startPos - 1);
    char first = result[0];
    char last  = result[result.size() - 1 ];

    if ( separatorPos != std::string::npos ) {
        while ( first != last ) {
            ReplacedStrRegEx.push_back(first);
            ReplacedStrRegEx += "|";
            first++;
        }
        ReplacedStrRegEx.push_back(first);
    } else {
        startPos++;
        while ( startPos != endPos - 1) {
            ReplacedStrRegEx.push_back(strRegEx[startPos]);
            ReplacedStrRegEx += "|";
            startPos++;
        }
        ReplacedStrRegEx.push_back(strRegEx[endPos - 1]);
    }
    ReplacedStrRegEx += strRegEx.substr( endPos + 1, strRegEx.size() - endPos );
    ReplacedStrRegEx += ")";

    return ReplacedStrRegEx;
}

bool RegEx::Compile(std::string strRegEx) {

    while ( strRegEx.find( "[" ) != std::string::npos ) {
        strRegEx = PreProcessBracket(strRegEx);
    }

    m_strInfixRegEx = const_cast<char*>(strRegEx.c_str());
    CleanUp();

    if(!ConstructThompsonNFA())
        return false;

    PopTable( m_oNFATable);
    m_oNFATable[m_oNFATable.size() - 1 ]->m_bAcceptingState = true;

    ConvertNFAtoDFA();
    MinimizeDFA();
    return true;
}

bool RegEx::Match(std::string strText) {
    m_strText = strText;
    RegexState *pState = m_oDFATable[0];
    std::vector<RegexState*>  Transition;
    for(int i = 0; i < (int)m_strText.size(); ++i) {
        char CurrChar = m_strText[i];
        pState->GetTransition(CurrChar, Transition);
        if (Transition.empty())
            return false;
        pState = Transition[0];
        std::cout << strText.substr(i) << std::endl;
        if (pState->m_bAcceptingState && !Match(strText.substr(i, 3)))
            return true;

    }
    return false;
}

bool RegEx::Match(RegexState **pState, char cSymbol) {
    std::vector<RegexState*>  Transition;
    (*pState)->GetTransition(cSymbol, Transition);
    if (!Transition.empty()) {
        *pState = Transition[0];
    }
    return (*pState)->m_bAcceptingState;
}


int RegEx::PreProcessLiterals() {
    if (m_cCurPreProcChar == '\0')
        return -1;
    if (m_cCurPreProcChar == '\\') {
        m_cCurPreProcChar = *(m_strInfixRegEx++);
        if (m_cCurPreProcChar == '\0')
            return -1;
        m_strPostRegEx += m_cCurPreProcChar;
        m_cCurPreProcChar = *(m_strInfixRegEx++);
    } else if (strchr("()|*+?", m_cCurPreProcChar) == NULL) {
        if (m_cCurPreProcChar == '&')
            m_strPostRegEx += "\\&";
        else
            m_strPostRegEx += m_cCurPreProcChar;

        m_cCurPreProcChar = *(m_strInfixRegEx++);
    } else if (m_cCurPreProcChar == '(') {
        m_cCurPreProcChar = *(m_strInfixRegEx++);
        PreProcessOr();
        if (m_cCurPreProcChar != ')')
            return -1;
        m_cCurPreProcChar = *(m_strInfixRegEx++);
    } else
        return -1;
    return 0;
}
int RegEx::PreProcessClosure() {
    PreProcessLiterals();
    while (m_cCurPreProcChar != '\0' && strchr("*+?",m_cCurPreProcChar) != NULL) {
        m_strPostRegEx += m_cCurPreProcChar;
        m_cCurPreProcChar = *(m_strInfixRegEx++);
    }
    return 0;
}
int RegEx::PrePreprocessConcatenation(void) {
    PreProcessClosure();
    while (strchr(")|*+?", m_cCurPreProcChar) == NULL) {
        PreProcessClosure();
        m_strPostRegEx += "&";
    }
    return 0;
}
int RegEx::PreProcessOr() {
    PrePreprocessConcatenation();
    while (m_cCurPreProcChar == '|') {
        m_cCurPreProcChar = *(m_strInfixRegEx++);
        PrePreprocessConcatenation();
        m_strPostRegEx += "|";
    }
    return 0;
}

int RegEx::CovertToPostfix() {
    m_cCurPreProcChar = *(m_strInfixRegEx++);
    PreProcessOr();
    return 0;
}
bool RegEx::ConstructThompsonNFA() {
    CovertToPostfix();
    for(int i = 0; i < (int)m_strPostRegEx.size(); ++i) {
        char curChar = m_strPostRegEx[i];
        if ( IsInput( curChar )) {
            PushOnCharacterStack( curChar );
        } else if ( IsMetaChar( curChar ) ) {
            switch ( curChar ) {
                case CLOSURE:
                    Closure();
                    break;
                case PLUS_CLOSURE:
                    ClosurePlus();
                    break;
                case OPTIONAL:
                    ClosureOptional();
                    break;
                case OR:
                    Or();
                    break;
                case FALSE_CONCAT:
                    Concatenate();
                    break;
            }
        }
    }
    return true;
}


void RegEx::PrintTable(Table &table) {
    std::string TableString;
    for(int i = 0; i < (int)table.size(); ++i) {
        RegexState *pState = table[i];
        if(pState->m_bAcceptingState) {
            TableString += "\t" + pState->getStringID() + "\t\n";
        }
    }
    TableString += "\n";
    for( int i = 0; i < (int)table.size(); ++i) {
        RegexState *pState = table[i];
        std::vector<RegexState*> State;
        pState->GetTransition(EPSILON, State);
        for(int j = 0; j < (int)State.size(); ++j) {
            TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
            TableString += "\t[label = \"epsilon \"]\n";
        }
        std::set<char>::iterator iter;
        for(iter = m_InputSet.begin(); iter != m_InputSet.end(); ++iter) {
            pState->GetTransition(*iter, State);
            for(int j = 0; j < (int)State.size(); ++j) {
                TableString += "\t" + pState->getStringID() + " -> " + State[j]->getStringID();
                std::stringstream out;
                out << *iter;
                TableString += "\t[label = \"" + out.str() + " \"]\n";
            }
        }
    }
    std::cout << TableString;
}

void RegEx::PrintDFATable()
{
    PrintTable(m_oDFATable);
}