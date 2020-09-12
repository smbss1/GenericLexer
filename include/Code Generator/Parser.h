#include <cstddef>
#include <string>
#include <vector>
#include "MyAllocator.h"
#include "LinearAllocator.h"

using namespace std;

class Tokenizer;
class Token;

enum class EnodeBinaryOpType
{
    Invalid,
    Add,
    Subtract,
    Multiply,
    Divide,
    Modulus,
    BitshiftLeft,
    BitshiftRight,
    BitwiseAnd,
    BitwiseOr,
    BooleanAnd,
    BooleanOr,
    Max
};

enum class EnodeUnaryOpType
{
    Invalid,
    Negative,
    Not,
    BitwiseNegate,
    Max
};

enum class NodeType {
    NodeIdentifier,
    NodeDeclaration,
    NodeTypeUsage,
    NodeStructDeclaration,
    NodeTag,
    NodeEnumDeclaration,
    NodeUnionDeclaration,
    NodeConstantDefinition,
    NodeNamespace,
    NodeNumber,
    NodeString,
    NodeChar,
    NodeBinaryOperator,
    NodeUnaryOperator,
    NodeProcedureHeader,
};

struct EnodeNode
{
public:
    EnodeNode();
    ~EnodeNode();

    EnodeNode* m_pNext;
    NodeType m_eType;
    int m_iLength;
    EnodeNode* m_pFirstTag;
    EnodeNode* m_pNamespaceNode;
    std::string m_strNamespace;
    std::string m_strText;

    union {
        struct {
            EnodeNode* m_pDeclaration;
        } Identifier;

        struct {
            EnodeUnaryOpType m_eUnaryOpType;
            EnodeNode* m_pOperand;
        } UnaryOperator;

        struct {
            EnodeBinaryOpType m_eBinaryOpType;
            EnodeNode* m_pLeft;
            EnodeNode* m_pRight;
        } BinaryOperator;

        struct {
            EnodeNode* m_pFirstMember;
        } StructDeclaration;

        struct {
            EnodeNode* m_pFirstMember;
        } UnionDeclaration;

        struct {
            EnodeNode* m_pFirstConst;
        } EnumDeclaration;

        struct {
            EnodeNode* m_pFirstFlag;
        } FlagsDeclaration;

        struct {
            EnodeNode* m_pType;
            EnodeNode* m_pinitialization;
        } Declaration;

        struct {
            int m_iPtrCount;
            EnodeNode* m_pFirstArraySizeExpression;
            EnodeNode* m_pStructDeclaration;
            EnodeNode* m_pUnionDeclaration;
            EnodeNode* m_pTypeDefinition;
        } TypeUsage;

        struct {
            EnodeNode* m_pFirstTagParam;
        } Tag;

        struct {
            EnodeNode* m_pExpression;
        } ConstantDefinition;

        struct {
            EnodeNode* m_pReturnType;
            EnodeNode* m_pFirstParam;
        } ProcedureHeader;
    };
};

struct ParseError
{
    ParseError() {};
    ParseError(char* strText, char* strFilename, int iLine) { m_strText = strText; m_strFilename = strFilename; m_iLine = iLine; };
    char* m_strText;
    int m_iLine;
    char* m_strFilename;
};

class Parser
{
public:
    Allocator& m_oAllocator;
    int m_iErrorStackSize;
    int m_iErrorStackMax;
    vector<ParseError> m_vErrorStack;
    ParseError* m_pErrorStack;
    EnodeNode* m_pTagStackHead;

    Parser(Allocator& oAllocator);
    ~Parser();

private:
    EnodeNode* Parse_Code(Tokenizer& oTokenizer);
    EnodeNode* ParseDeclarationBody(Tokenizer& oTokenizer, Token oName);
    EnodeNode* ParseDeclarationList(Tokenizer& oTokenizer);
    EnodeNode* ParseStructDeclarationList(Tokenizer& oTokenizer);
    EnodeNode* ParseStructBody(Tokenizer& oTokenizer, Token oName);
    EnodeNode* ParseTypeUsage(Tokenizer& oTokenizer);
    EnodeNode* ParseUnionBody(Tokenizer& oTokenizer, Token oName);
    EnodeNode* ParseEnumBody(Tokenizer& oTokenizer, Token oName);
    EnodeNode* ParseIdentifierList(Tokenizer& oTokenizer);
    int BinaryOpPrecedence(int iType);
    EnodeNode* ParseExpression_p(Tokenizer& oTokenizer, int iPrecedenceIn);
    EnodeNode* ParseExpression(Tokenizer& oTokenizer);
    int UnaryOpPrecedence(int iType);
    EnodeNode* ParseUnarySetExpr(Tokenizer& oTokenizer, Token oToken, NodeType eType);
    EnodeNode* ParseUnaryExpression(Tokenizer& oTokenizer);
    void PushTag(EnodeNode* pTagNode);
    EnodeNode *PopAllTag();
    void ParseTagParams(Tokenizer& oTokenizer, EnodeNode* pTagNode);
    void ParseTagList(Tokenizer& oTokenizer);
    EnodeNode* ParseNamespace(Tokenizer& oTokenizer, Token oName);
    EnodeNode* ParseProcedureHeaderBody(Tokenizer& oTokenizer, Token oName);

public:
    EnodeNode* AllocateNode(NodeType eType, Token oToken);
    EnodeNode* AllocateNode(NodeType eType);
    EnodeNode* ParseConstBody(Tokenizer& oTokenizer, Token name);
    void PushError(Tokenizer& oTokenizer, char *msg, ...);
    EnodeNode* ParseCode(Tokenizer& oTokenizer);

};