
typedef struct EnodeNode EnodeNode;

typedef void EnodeInitCallback(void);
typedef void EnodeParseCallback(EnodeNode *root, char *filename);
typedef void EnodeParseStructCallback(EnodeNode *root, char *filename);
typedef void EnodeParseUnionCallback(EnodeNode *root, char *filename);
typedef void EnodeParseEnumCallback(EnodeNode *root, char *filename);
typedef void EnodeCleanUpCallback(void);

struct EnodeCustom
{
    EnodeCustom()
    {
        InitCallback = 0;
        ParseCallback = 0;
        ParseStructCallback = 0;
        ParseUnionCallback = 0;
        ParseEnumCallback = 0;
        CleanUpCallback = 0;
    }

    EnodeInitCallback*          InitCallback;
    EnodeParseCallback*         ParseCallback;
    EnodeParseStructCallback*   ParseStructCallback;
    EnodeParseUnionCallback*    ParseUnionCallback;
    EnodeParseEnumCallback*     ParseEnumCallback;
    EnodeCleanUpCallback*       CleanUpCallback;

    
#if _WIN32
    HANDLE custom_dll;
#elif __linux__
    void* custom_dll;
#endif
    
};