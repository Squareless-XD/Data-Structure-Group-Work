// 4.10-字符串求逆
// Description
// 编写对串求逆的递推算法。

// Input
// 原始字符串

// Output
// 求逆之后的字符串

// Sample Input 1 
// abcde
// Sample Output 1
// edcba

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
}strNode;

typedef struct _lStr
{
    int len;
    strNode *head;
    strNode *tail;
}lString;


strNode *strNodeAlloc(void);
status strInitAssign(lString *str);
status strDestory(lString *str);
status strAppend(lString *str, char newChar);
status strReverse(lString *str);
status strInputFlex(lString *str);
void strPrint(lString str);

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

status strAppend(lString *str, char newChar)
{
    strNode *newNode;

    ++(str->len);

    if (str->len == 1) // the first character
    {
        newNode = strNodeAlloc();
        if (newNode == NULL)
            exit(ERROR);
        newNode->chdata = newChar;
        str->head = str->tail = newNode;
        return OK;
    }

    newNode = strNodeAlloc();
    if (newNode == NULL)
        exit(ERROR);
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

status strInputFlex(lString *str)
{
    char c;

    while ((c = getchar()) != '\n' && c != EOF)
        strAppend(str, c);
    return OK;
}

void strPrint(lString str)
{
    strNode *node;
    if (str.len == 0)
        return;
        node = str.head;
    while(node != NULL)
    {
        cout << node->chdata;
        node = node->next;
    }
    cout << endl;
}

int main()
{
    lString str;
    strInitAssign(&str);
    strInputFlex(&str);
    strReverse(&str);
    strPrint(str);

    return 0;
}