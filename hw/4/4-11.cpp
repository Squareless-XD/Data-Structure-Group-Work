// 4.11-删除字符
// Description
// 编写算法，求得所有包含在串s中而不包含在串t中的字符（s中重复的字符只选一个）构成的新串r，以及r中每个字符在s中第一次出现的位置。

// Input
// 串s和串t，空格分隔

// Output
// 串r，以及r中每个字符在s中第一次出现的位置，如果串r为空，则输出-1

// Sample Input 1
// abbbaacdx cx
// Sample Output 1
// abd 017

// Sample Input 2
// dddd dc
// Sample Output 2
// -1

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
    int count;
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
status strAppend(lString *str, char newChar, int index);
status strInputUntilSpace(lString *str);
void strPrintNoEndl(lString str);
status strFindElem(lString str, char toFind, strNode *position);
status addNewElem(int count, char chdata, lString t, lString *r);

strNode *strNodeAlloc(void)
{
    strNode *newNode;
    newNode = (strNode *)malloc(sizeof(strNode));
    if (newNode == NULL)
        exit(ERROR);
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

status strAppend(lString *str, char newChar, int index)
{
    strNode *newNode;

    ++(str->len);

    if (str->len == 1) // the first character
    {
        newNode = strNodeAlloc();
        if (newNode == NULL)
            exit(ERROR);
        newNode->chdata = newChar;
        newNode->count = index; // This is new, please delete it soon.
        str->head = str->tail = newNode;
        return OK;
    }

    newNode = strNodeAlloc();
    if (newNode == NULL)
        exit(ERROR);
    newNode->chdata = newChar;
    newNode->prev = str->tail;
    newNode->count = index; // This is new, please delete it soon.
    str->tail->next = newNode;
    str->tail = newNode;

    return OK;
}

// input until space
status strInputUntilSpace(lString *str)
{
    char c;

    while ((c = getchar()) != ' ' && c != EOF && c != '\n')
        strAppend(str, c, 0);
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

status addNewElem(int count, char chdata, lString t, lString *r)
{
    strNode *nodeTemp;

    if (strFindElem(t, chdata, nodeTemp) == FALSE && strFindElem(*r, chdata, nodeTemp) == FALSE)
    {
        strAppend(r, chdata, count);
    }
    return OK;
}

int main()
{
    lString s, t, r;
    strNode *node;
    int count = 0;

    strInitAssign(&s);
    strInitAssign(&t);
    strInitAssign(&r);

    strInputUntilSpace(&s);
    strInputUntilSpace(&t);

    if (s.head == NULL)
        return OK;
    node = s.head;
    while (node != NULL)
    {
        addNewElem(count, node->chdata, t, &r);
        node = node->next;
        ++count;
    }

    if (r.len == 0)
    {
        cout << -1 << endl;
        return 0;
    }

    strPrintNoEndl(r);
    cout << ' ';

    node = r.head;
    while (node != NULL)
    {
        cout << node->count;
        node = node->next;
    }
    cout << endl;

    return 0;
}