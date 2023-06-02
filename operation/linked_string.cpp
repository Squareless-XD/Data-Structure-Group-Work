#include <bits/stdc++.h>

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
status strEmpty(lString str);
status strCompare(lString str1, lString str2);
int charCmp(strNode node1, strNode node2);
status subString(lString str, lString *sub, int pos, int len);
void get_next(lString pattern, int *next);
int strIndexKMP(lString str, lString pattern, int pos);
int strIndex(lString str, lString pattern, int pos);
status strAssign(lString *str, char *charStr);
status strCat(lString *str, lString substr1, lString substr2);
status strClear(lString *str);
status strRepalce(lString *str, lString pattern, lString replace);
status strDelete(lString *str, int pos, int len);
status strInsert(lString *str, lString sub, int pos);
status strAppend(lString *str, char newChar);
status strReverse(lString *str);
status strInputFlex(lString *str);
status strInputUntilSpace(lString *str);
void strPrint(lString str);
void strPrintNoEndl(lString str);
status strTravese(lString str, status (*visit)(strNode *node));
status strFindElem(lString str, char toFind, strNode *position);

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

status strEmpty(lString str)
{
    if (str.head == NULL && str.tail == NULL && str.len == 0)
        return TRUE;
    return FALSE;
}

// if str1 is larger, then return 1. Smaller: -1. Euqal: 0.
status strCompare(lString str1, lString str2)
{
    int strLen1 = strLength(str1), strLen2 = strLength(str2);
    int minStrLen = std::min(strLen1, strLen2); // find the minimum to check whether it is right to focus on the shared length
    int index = 1, cmpTemp;
    strNode *node1 = str1.head, *node2 = str2.head;

    while (node1 != NULL && node2 != NULL)
    {
        if ((cmpTemp = charCmp(*node1, *node2)) != 0)
            return cmpTemp;
        node1 = node1->next;
        node2 = node2->next;
    }
    if (node1 == NULL && node2 == NULL)
        return 0;
    if (node1 == NULL)
        return 1;
    return -1;
}
int charCmp(strNode node1, strNode node2)
{
    if (node1.chdata < node2.chdata)
        return -1;
    else if (node1.chdata > node2.chdata)
        return 1;
    return 0;
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

void get_next(lString pattern, int *next)
{
    strNode *nodeNow, *nodeBack;
    int posNow = 1, posBack = 0;
    int patternLen = pattern.len;
    int diff, i;

    next[1] = 0;
    nodeNow = pattern.head;

    // run for every char in the array "next"
    while (posNow < patternLen)
    {
        // if (posBack == 0 || pattern[posNow] == pattern[posBack]) // if 0, go 1; if the same, go to the next
        // {
        //     ++posNow;
        //     ++posBack;
        //     next[posNow] = posBack;
        // }
        // else
        //     posBack = next[posBack]; // if not the same, let the "back" go backward to match another sequence

        if (posBack == 0 || nodeNow->chdata == nodeBack->chdata)
        {
            nodeNow = nodeNow->next;
            ++posNow;
            if (posBack == 0)
                nodeBack = pattern.head;
            else
                nodeBack = nodeBack->next;
            ++posBack;
            next[posNow] = posBack;
        }
        else if (next[posBack] == 0)
            posBack = 0;
        else
        {
            diff = posBack - next[posBack];
            for (i = 0; i < diff; ++i)
                nodeBack = nodeBack->prev;
            posBack = next[posBack];
        }
    }
}

int strIndexKMP(lString str, lString pattern, int pos)
{
    strNode *nodeStr, *nodePat;
    int next[pattern.len + 2];
    int indexStr = pos;
    int indexPat = 1;
    int i, diff;

    if (pattern.len == 0 || str.len == 0 || pos + pattern.len - 1 > str.len)
        return 0;

    nodePat = pattern.head;
    nodeStr = str.head;
    for (i = 0; i < pos - 1; ++i)
        nodeStr = nodeStr->next;

    get_next(pattern, next);

    while (indexStr <= str.len && indexPat <= pattern.len)
    {
        if (indexPat == 0 || nodeStr->chdata == nodePat->chdata)
        {
            nodeStr = nodeStr->next;
            ++indexStr;
            if (indexPat == 0)
                nodePat = pattern.head;
            else
                nodePat = nodePat->next;
            ++indexPat;
        }
        else
        {
            indexPat = next[indexPat];
            if (next[indexPat] != 0)
            {
                diff = indexPat - next[indexPat];
                for (i = 0; i < diff; ++i)
                    nodePat = nodePat->prev;
            }
        }
    }
    if (indexPat > pattern.len)
        return indexStr - pattern.len;
    return 0;
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

status strAssign(lString *str, char *charStr)
{
    int len, index = 0;
    char *ptrCharStr = charStr + 1;
    strNode *newNode;

    strDestory(str);
    len = strlen(charStr);
    str->len = len;

    if (len == 0)
        return 0;

    newNode = strNodeAlloc();
    if (newNode == NULL)
        return ERROR;
    newNode->chdata = charStr[0];
    str->head = str->tail = newNode;

    while (newNode->chdata != '\0') // until a '\0' was out into the linked string
    {
        ++index;
        newNode = strNodeAlloc();
        if (newNode == NULL)
            return ERROR;
        newNode->chdata = charStr[index];
        newNode->prev = str->tail;
        str->tail->next = newNode;
        str->tail = newNode;
    }

    return OK;
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

status strReverse(lString *str)
{
    strNode *node, *node2, *node3;

    node = str->head;
    if (node == NULL)
        return OK;
    node2 = node->next;
    node->next = NULL;
    node->prev = node2;
    while (node2 != NULL)
    {
        node3 = node2->next;
        node2->next = node;
        node2->prev = node3;
        node = node2;
        node2 = node3;
    }

    node = str->head;
    str->head = str->tail;
    str->tail = node;

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
        std::cout << node->chdata;
        node = node->next;
    }
    std::cout << std::endl;
}

void strPrintNoEndl(lString str)
{
    strNode *node;
    if (str.len == 0)
        return;
    node = str.head;
    while (node != NULL)
    {
        std::cout << node->chdata;
        node = node->next;
    }
}

status strTravese(lString str, status (*visit)(strNode *node))
{
    strNode *node;

    if (str.head == NULL)
        return OK;
    node = str.head;
    while (node != NULL)
    {
        visit(node);
        node = node->next;
    }
    return OK;
}

status strFindElem(lString str, char toFind, strNode *position)
{
    strNode *node;

    if (str.head == NULL)
    {
        position = NULL;
        return FALSE;
    }
    node = str.head;
    while (node != NULL)
    {
        if (node->chdata == toFind)
        {
            position = node;
            return TRUE;
        }
        node = node->next;
    }
    position = NULL;
    return FALSE;
}