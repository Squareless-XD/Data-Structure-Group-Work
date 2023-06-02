// Description
// 假设以定长顺序存储结构表示串，试写出一个算法，求串s中第一次出现的最长重复字串及其位置。
// 注意：重复的子串可以重叠

// Input
// 输入串s

// Output
// 输出最长重复子串，及其首次出现的位置，如果没出现重复子串则直接输出-1

// Sample Input 1      Sample Input 2      Sample Input 3
// aaaaa               abc                 aaabbbbaaa
// Sample Output 1     Sample Output 2     Sample Output 3
// aaaa 0              -1                  aaa 0

#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_STRING -2

typedef int status;

// claim a variable called "strNode"
typedef struct stringNode
{
    char chdata;
    struct stringNode *next;
    struct stringNode *prev;
} strNode;

typedef struct _lStr
{
    int len;
    strNode *head;
    strNode *tail;
} lString;

strNode *strNodeAlloc(void);
status strInitAssign(lString *str);
status strDestory(lString *str);
status strCopy(lString *strSrc, lString strDst);
int strLength(lString str);
status subString(lString str, lString *sub, int pos, int len);
int strIndex(lString str, lString pattern, int pos);
status strAppend(lString *str, char newChar);
status strInputFlex(lString *str);
void strPrintNoEndl(lString str);

strNode *strNodeAlloc(void)
{
    strNode *newNode;
    newNode = (strNode *)malloc(sizeof(strNode));
    if (newNode == NULL)
        return NULL;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

status strInitAssign(lString *str)
{
    str->head = NULL;
    str->tail = NULL;
    str->len = 0;
    return OK;
}

status strDestory(lString *str)
{
    strNode *node = str->head, *temp;
    while (node != NULL)
    {
        temp = node->next;
        free(node);
        node = temp;
    }
    strInitAssign(str);
    return OK;
}

status strCopy(lString *strDst, lString strSrc)
{
    int i;
    strNode *nodeInsert, *nodeSrc, *newNode;

    strDestory(strDst);
    nodeSrc = strSrc.head;
    while (nodeSrc != NULL)
    {
        nodeInsert = strNodeAlloc();
        if (nodeSrc == strSrc.head)
            strDst->head = strDst->tail = nodeInsert;
        else
        {
            strDst->tail->next = nodeInsert;
            nodeInsert->prev = strDst->tail;
            strDst->tail = nodeInsert;
        }
        nodeInsert->chdata = nodeSrc->chdata;
        nodeSrc = nodeSrc->next;
    }
    strDst->len = strSrc.len;
    return OK;
}

int strLength(lString str)
{
    return str.len;
}

// get a sub-string from "str", beginning at "pos", with the length of "len". The first node is the 1st.
status subString(lString str, lString *substr, int pos, int len)
{
    strNode *markNode, *newNode;
    int index;

    if (pos + len - 1 > str.len || pos < 1 || len < 1)
        return ERROR;

    markNode = str.head;
    if (markNode == NULL)
        return ERROR;
    while (--pos)
        markNode = markNode->next; // the sub string

    strDestory(substr);

    newNode = strNodeAlloc();
    if (newNode == NULL)
        return ERROR;
    newNode->chdata = markNode->chdata;
    substr->head = substr->tail = newNode;
    ++(substr->len);
    markNode = markNode->next;
    for (index = 1; index < len; ++index)
    {
        newNode = strNodeAlloc();
        if (newNode == NULL)
            return ERROR;
        newNode->chdata = markNode->chdata;
        newNode->prev = substr->tail;
        substr->tail->next = newNode;
        substr->tail = newNode;
        ++(substr->len);
        markNode = markNode->next;
    }
    return OK;
}

int strIndex(lString str, lString pattern, int pos)
{
    strNode *nodeStr, *nodeStrCmp, *nodePat;
    // int next[pattern.len + 2];

    if (pattern.len == 0)
        return 0;
    if (str.len == 0)
        return 0;

    int indexStr = pos;
    int i, j, breakSign = FALSE;

    // nodePat = pattern.head;
    nodeStr = str.head;
    for (i = 0; i < pos - 1; ++i)
        nodeStr = nodeStr->next;

    // get_next(pattern, next);

    // all posibility: starting with first "str.len-pattern.len+1" nodes.
    while (indexStr <= str.len - pattern.len + 1)
    {
        breakSign = FALSE;

        // compare the first character
        nodePat = pattern.head;
        if (nodeStr->chdata != nodePat->chdata)
        {
            nodeStr = nodeStr->next;
            ++indexStr;
            continue;
        }

        nodeStrCmp = nodeStr;
        // since the length is indexPat, we need pattern.len-1 more comparations
        for (j = 0; j < pattern.len - 1; ++j)
        {
            nodeStrCmp = nodeStrCmp->next;
            nodePat = nodePat->next;
            if (nodeStrCmp->chdata != nodePat->chdata)
            {
                breakSign = TRUE;
                break;
            }
        }
        if (breakSign == FALSE) // this means the corrct substring was found
            return indexStr;

        nodeStr = nodeStr->next;
        ++indexStr;
    }

    return 0;
}

status strAppend(lString *str, char newChar)
{
    strNode *newNode;

    ++(str->len);

    if (str->len == 1) // the first character
    {
        newNode = strNodeAlloc();
        if (newNode == NULL)
            return ERROR;
        newNode->chdata = newChar;
        str->head = str->tail = newNode;
        return OK;
    }

    newNode = strNodeAlloc();
    if (newNode == NULL)
        return ERROR;
    newNode->chdata = newChar;
    newNode->prev = str->tail;
    str->tail->next = newNode;
    str->tail = newNode;

    return OK;
}

// input until another line
status strInputFlex(lString *str)
{
    char c;

    while ((c = getchar()) != '\n' && c != EOF)
        strAppend(str, c);
    return OK;
}

void strPrintNoEndl(lString str)
{
    strNode *node;
    if (str.len == 0)
        return;
    node = str.head;
    while (node != NULL)
    {
        cout << node->chdata;
        node = node->next;
    }
}

int main()
{
    lString str, sub, result;
    int strLen, resultPos = -1, i, j, k;

    strInitAssign(&str);
    strInitAssign(&sub);
    strInitAssign(&result);

    strInputFlex(&str);

    strLen = strLength(str);

    // "i" means the length of repeating substring that we are finding
    for (i = 1; i <= strLen - 1; ++i)
    {
        for (j = 0; j < strLen - i; ++j)
        {
            // 1 offset from the "position of result" and "position of function"
            subString(str, &sub, j + 1, i);
            if (strIndex(str, sub, j + 2) != 0) // if substring found in "str"
            {
                strCopy(&result, sub);
                resultPos = j;
                break;
            }
        }
        // if in this turn we did not find any new repeating string, then no longer string will be found
        if (result.len != i)
            break;
    }

    if (resultPos == -1)
    {
        cout << "-1" << endl;
        return 0;
    }

    strPrintNoEndl(result);
    cout << ' ' << resultPos << endl;

    return 0;
}
