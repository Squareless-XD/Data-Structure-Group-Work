// 4.29-串匹配的改进算法
// Description

// 假设以结点大小为1（带头结点）的链表结构表示串，则在利用next函数值进行串匹配时，在每个结点中需设三个域：数据域chdata、指针域succ和指针域next。其中chdata域存放一个字符；succ域存放指向同一链表中后继结点的指针；next域在主串中存放指向同一链表中前驱结点的指针；在模式串中，存放指向当该结点的字符与主串中的字符不等时，在模式串中下一个应进行比较的字符结点（即与该字符的next函数值相对应的字符结点）的指针，若该节点字符的next函数值为0，则其next域的值应指向头结点。试按上述定义的结构改写串匹配的改进算法（KMP算法）。

// Input
// 输入分为三行，第一行输入两个数，分别为待搜索串的长度和目标串的长度。

// 第二行表示待搜索串的元素序列。

// 第三行表示目标串的元素序列。

// Output
// 若能在待搜索串中匹配到目标串，则输出目标串的第一个字符在待搜索串中的位置（从0开始）；若不能，则输出-1。

// Sample Input 1

// 7 3
// samples
// sam
// Sample Output 1

// 0

#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
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
void get_next(lString pattern, int *next);
int strIndexKMP(lString str, lString pattern, int pos);
status strAppend(lString *str, char newChar);

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
    int next[pattern.len + 1];
    int indexStr = pos;
    int indexPat = 1;
    int i, diff;

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

int main()
{
    lString string, pattern;
    int stringLen, patternLen, i;
    int searchResult;

    strInitAssign(&string);
    strInitAssign(&pattern);

    cin >> stringLen >> patternLen;
    getchar();

    for (i = 0; i < stringLen; ++i)
        strAppend(&string, getchar());
    getchar();
    for (i = 0; i < patternLen; ++i)
        strAppend(&pattern, getchar());
    getchar();
    searchResult = strIndexKMP(string, pattern, 1);

    cout << searchResult - 1 << endl;

    return 0;
}
