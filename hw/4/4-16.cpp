// Description
// 编写算法，实现串的基本操作StrCompare(S,T)

// Input
// 字符串S，T，空格分隔

// Output
// 输出比较结果，0代表S=T，1代表S>T，-1代表S<T

// Sample Input 1 
// acccdd accd
// Sample Output 1
// -1

// Sample Input 2 
// bee abee
// Sample Output 2
// 1

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
int strLength(lString str);
status strCompare(lString str1, lString str2);
int charCmp(strNode node1, strNode node2);
status strAppend(lString *str, char newChar);
status strInputFlex(lString *str);
status strInputUntilSpace(lString *str);
void strPrint(lString str);
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

int strLength(lString str)
{
    return str.len;
}

// if str1 is larger, then return 1. Smaller: -1. Euqal: 0.
status strCompare(lString str1, lString str2)
{
    int strLen1 = strLength(str1), strLen2 = strLength(str2);
    int minStrLen = min(strLen1, strLen2); // find the minimum to check whether it is right to focus on the shared length
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
        return -1;
    return 1;
}
int charCmp(strNode node1, strNode node2)
{
    if (node1.chdata < node2.chdata)
        return -1;
    else if (node1.chdata > node2.chdata)
        return 1;
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

// input until space
status strInputUntilSpace(lString *str)
{
    char c;

    while ((c = getchar()) != ' ' && c != EOF)
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
    lString str1, str2;
    int cmpResult;

    strInitAssign(&str1);
    strInitAssign(&str2);

    strInputUntilSpace(&str1);
    strInputFlex(&str2);

    cmpResult = strCompare(str1, str2);
    cout << cmpResult << endl;

    return 0;
}