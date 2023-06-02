// Description
// 编写算法，实现串的基本操作Replace(&S, T, V)，替换

// Input
// 输入串，待替换字符串，替换字符串
// 使用空格分隔

// Output
// 输出替换后的字符串

// Sample Input 1       Sample Input 2      Sample Input 3
// abcde cd f           dddf dd f           eeee dd f
// Sample Output 1      Sample Output 2     Sample Output 3
// abfe                 fdf                 eeee

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
int strIndex(lString str, lString pattern, int pos);
status strAssign(lString *str, char *charStr);
status strCat(lString *str, lString substr1, lString substr2);
status strClear(lString *str);
status strRepalce(lString *str, lString pattern, lString replace);
status strDelete(lString *str, int pos, int len);
status strInsert(lString *str, lString sub, int pos);
status strAppend(lString *str, char newChar);
status strInputFlex(lString *str);
status strInputUntilSpace(lString *str);
void strPrint(lString str);

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

    nodeInsert = strNodeAlloc();
    strDst->head = strDst->tail = nodeInsert;

    while (nodeSrc != NULL)
    {
        nodeInsert = strNodeAlloc();

        nodeInsert->chdata = nodeSrc->chdata;
        nodeInsert->prev = strDst->tail;
        strDst->tail->next = nodeInsert;
        strDst->tail = nodeInsert;

        nodeSrc = nodeSrc->next;
    }
    strDst->len = strSrc.len;

    return OK;
}

int strLength(lString str)
{
    return str.len;
}

int strIndex(lString str, lString pattern, int pos)
{
    strNode *nodeStr, *nodeStrCmp, *nodePat;
    // int next[pattern.len + 2];

    int indexStr = pos;
    int i, j, breakSign;

    if (pattern.len == 0 || str.len == 0 || pos + pattern.len - 1 > str.len)
        return 0;

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

status strCat(lString *str, lString substr1, lString substr2)
{
    lString temp;

    if (substr1.len == 0)
    {
        strCopy(str, substr2);
        return OK;
    }
    if (substr2.len == 0)
    {
        strCopy(str, substr1);
        return OK;
    }

    strInitAssign(&temp);
    strCopy(&temp, substr2);
    strCopy(str, substr1);
    str->tail->next = temp.head;
    str->tail = temp.tail;
    str->len += temp.len;

    return OK;
}

// totally clear a piece of memory that we assigned with "malloc"
status strClear(lString *str)
{
    strDestory(str);
    free(str);
    return OK;
}

status strRepalce(lString *str, lString pattern, lString replace)
{
    int index = 1;

    while ((index = strIndex(*str, pattern, index)) != 0)
    {
        strDelete(str, index, pattern.len);
        strInsert(str, replace, index);
        index += replace.len;
    }

    return OK;
}

status strDelete(lString *str, int pos, int len)
{
    strNode *delNode = str->head;
    int strlen = str->len;

    if (strlen < pos + len - 1 || len < 1 || pos < 1)
        return ERROR;

    str->len -= len;

    while (--pos) // move "pos-1"
        delNode = delNode->next;
    while (len-- && delNode != NULL) // delete "len"
    {
        if (delNode->next != NULL && delNode->prev != NULL)
        {
            delNode->next->prev = delNode->prev;
            delNode = delNode->next;
            free(delNode->prev->next);
            delNode->prev->next = delNode;
        }
        else if (delNode->next == NULL && delNode->prev != NULL)
        {
            str->tail = delNode->prev;
            delNode->prev->next = NULL;
            free(delNode);
            delNode = NULL;
        }
        else if (delNode->next != NULL && delNode->prev == NULL)
        {
            str->head = delNode->next;
            delNode = delNode->next;
            free(delNode->prev);
            delNode->prev = NULL;
        }
        else
        {
            str->head = str->tail = NULL;
            free(delNode);
            delNode = NULL;
        }
    }
    return OK;
}

status strInsert(lString *str, lString sub, int pos)
{
    lString temp;
    strNode *newNode, *strNode = str->head, *subNode = sub.head;
    int strlen = str->len;
    if (strlen < pos - 1)
        return ERROR;


    if (pos == 1)
    {
        strInitAssign(&temp);
        strCat(&temp, sub, *str);
        strCopy(str, temp);
        strDestory(&temp);
        return OK;
    }
    if (pos == strlen + 1)
    {
        strInitAssign(&temp);
        strCat(&temp, *str, sub);
        strCopy(str, temp);
        strDestory(&temp);
        return OK;
    }

    str->len += sub.len;
    while (--pos > 1) // move "pos-2", then insert new nodes after strNode.
        strNode = strNode->next;

    while (subNode != NULL) // delete "len"
    {
        newNode = strNodeAlloc();
        if (newNode == NULL)
            return ERROR;
        newNode->chdata = subNode->chdata;

        newNode->next = strNode->next;
        newNode->prev = strNode;

        strNode->next->prev = newNode;
        strNode->next = newNode;

        strNode = newNode;
        subNode = subNode->next;
    }
    return OK;
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

// input until space
status strInputUntilSpace(lString *str)
{
    char c;

    while ((c = getchar()) != ' ' && c != EOF && c != '\n')
        strAppend(str, c);
    return OK;
}

void strPrint(lString str)
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
    cout << endl;
}

int main()
{
    lString str, removeSubstr, replaceStr;

    strInitAssign(&str);
    strInitAssign(&removeSubstr);
    strInitAssign(&replaceStr);

    strInputUntilSpace(&str);
    strInputUntilSpace(&removeSubstr);
    strInputFlex(&replaceStr);

    strRepalce(&str, removeSubstr, replaceStr);

    strPrint(str);

    return 0;
}