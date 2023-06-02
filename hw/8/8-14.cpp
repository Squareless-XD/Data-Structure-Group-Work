// 8.14-伙伴管理系统
// Description
// 试完成伙伴管理系统的存储回收算法。

// Input
// 输入分为两部分，第一部分是当前已有的空闲块链表，每行中的第一个数字代表当前链表的k，第一个数字之后的每个数字代表每个块的起始地址。
// 第二部分中每行代表待回收的空闲块，第一个数字为空闲块的k，第二个数字为该块的起始地址。

// Output
// 输出回收后的空闲块链表

// Sample Input 1       Sample Output 1
// 2 0 8 16             2 8 16
//                      3 0
// 2 4              

// Sample Input 2       Sample Output 2
// 2 0                  4 0
// 3 8
//
// 2 4

// Sample Input 3       Sample Output 3
// 5 0 64               7 0
//
// 5 32
// 5 96

// Sample Input 4       Sample Output 4
// 3 56 64              3 40 64
// 4 0                  4 0 48
//
// 2 52
// 2 48
// 3 40

#include <bits/stdc++.h>
#include <vector>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2

#define DEFAULT_SIZE 10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef int status;

typedef struct spNode
{
    int start;
    struct spNode *prev;
    struct spNode *next;
} spaceNode;

typedef struct
{
    int power;
    spaceNode *first;
} buddySpace;

spaceNode *spaceNodeAlloc(spaceNode *prev, int start)
{
    spaceNode *newNode;

    newNode = (spaceNode *)malloc(sizeof(spaceNode));
    newNode->start = start;
    newNode->prev = prev;
    newNode->next = NULL;

    return newNode;
}

// insert a space into the buddy system
status insertSpace(std::vector<buddySpace> *pbuddySys, int power, int start)
{
    spaceNode *spaceTemp;

    if (power > (*pbuddySys).size())
        (*pbuddySys).resize(power, {0, NULL});
    (*pbuddySys)[power].power = power;

    spaceTemp = (*pbuddySys)[power].first;

    if (spaceTemp == NULL)
    {
        (*pbuddySys)[power].first = spaceNodeAlloc(NULL, start);
        spaceTemp = (*pbuddySys)[power].first;
        // setup the circular linked list
        spaceTemp->prev = spaceTemp->next = spaceTemp;
        return OK;
    }
    if (spaceTemp->start > start)
    {
        spaceTemp->prev = spaceNodeAlloc(spaceTemp->prev, start);
        spaceTemp->prev->next = spaceTemp;
        spaceTemp->prev->prev->next = spaceTemp->prev;
        (*pbuddySys)[power].first = spaceTemp->prev;
        return OK;
    }
    while (spaceTemp->start < start && spaceTemp->next != (*pbuddySys)[power].first)
        spaceTemp = spaceTemp->next;
    if (spaceTemp->start > start)
    {
        spaceTemp->prev = spaceNodeAlloc(spaceTemp->prev, start);
        // setup the circular linked list
        spaceTemp->prev->next = spaceTemp;
        spaceTemp->prev->prev->next = spaceTemp->prev;
        return OK;
    }
    if (spaceTemp->start < start)
    {
        spaceTemp->next = spaceNodeAlloc(spaceTemp, start);
        // setup the circular linked list
        spaceTemp->next->next = (*pbuddySys)[power].first;
        (*pbuddySys)[power].first->prev = spaceTemp->next;
        return OK;
    }
    return ERROR;
}

// delete a space into the buddy system
status deleteSpace(std::vector<buddySpace> *pbuddySys, int power, int start)
{
    spaceNode *spaceTemp = (*pbuddySys)[power].first;

    if (spaceTemp == NULL)
        return ERROR;
    if (spaceTemp->start == start)
    {
        if (spaceTemp->next != (*pbuddySys)[power].first)
        {
            spaceTemp->prev->next = spaceTemp->next;
            spaceTemp->next->prev = spaceTemp->prev;
            (*pbuddySys)[power].first = spaceTemp->next;
        }
        else
            (*pbuddySys)[power].first = NULL;
        free(spaceTemp);
        return OK;
    }
    while (spaceTemp->start != start && spaceTemp->next != (*pbuddySys)[power].first)
        spaceTemp = spaceTemp->next;
    if (spaceTemp->start == start)
    {
        spaceTemp->prev->next = spaceTemp->next;
        spaceTemp->next->prev = spaceTemp->prev;
        free(spaceTemp);
        return OK;
    }
    return ERROR;
}

// get the power of an int, return int
int pow_int(int base, int power)
{
    int result = 1;

    for (int i = 0; i < power; i++)
        result *= base;

    return result;
}

// check whether two spaces can be merged in a buddy system
status mergeCheck(int power, int start1, int start2)
{
    int size = pow_int(2, power + 1);
    if (start1 / size == start2 / size)
        return TRUE;
    return FALSE;
}

// merge two spaces in a buddy system
status mergeSpace(std::vector<buddySpace> *pbuddySys, int power, int start)
{
    spaceNode *spaceTemp;
    int minStart;

    if (power > (*pbuddySys).size())
        (*pbuddySys).resize(power, {0, NULL});
    (*pbuddySys)[power].power = power;

    spaceTemp = (*pbuddySys)[power].first;

    if (spaceTemp == NULL)
    {
        insertSpace(pbuddySys, power, start);
        return ERROR;
    }
    while (!mergeCheck(power, spaceTemp->start, start) && spaceTemp->next != (*pbuddySys)[power].first)
        spaceTemp = spaceTemp->next;
    if (mergeCheck(power, spaceTemp->start, start))
    {
        minStart = MIN(spaceTemp->start, start);
        deleteSpace(pbuddySys, power, spaceTemp->start);
        mergeSpace(pbuddySys, power + 1, minStart);
        return OK;
    }
    insertSpace(pbuddySys, power, start);
    return ERROR;
}

int main()
{
    std::vector<buddySpace> buddySys;
    spaceNode *spaceTemp;
    int k, position;
    char c;

    buddySys.resize(DEFAULT_SIZE);

    if ((c = getchar()) == EOF)
        return 0;
    if (c == '\n')
        goto Jump_Loop;
    ungetc(c, stdin);
    // input for available space
    while (1)
    {
        if ((c = getchar()) == '\n')
            break;
        ungetc(c, stdin);

        scanf("%d%c", &k, &c);
        if (k > buddySys.size())
            buddySys.resize(k, {0, NULL});
        buddySys[k].power = k;
        if (c == '\n')
            break;

        while (1)
        {
            scanf("%d%c", &position, &c);
            mergeSpace(&buddySys, k, position);
            if (c == '\n' || c == EOF)
                break;
        }
    }    

Jump_Loop:

    // input for released space
    while (1)
    {
        if ((c = getchar()) == EOF)
            break;
        ungetc(c, stdin);

        scanf("%d%c", &k, &c);
        if (k > buddySys.size())
            buddySys.resize(k, {0, NULL});
        buddySys[k].power = k;
        if (c == '\n')
            break;
        
        while (1)
        {
            scanf("%d%c", &position, &c);
            mergeSpace(&buddySys, k, position);
            if (c == '\n' || c == EOF)
                break;
        }
    }

    // output
    for (int i = 0; i < buddySys.size(); i++)
    {
        if (buddySys[i].first == NULL)
            continue;
        printf("%d", buddySys[i].power);
        spaceTemp = buddySys[i].first;
        do
        {
            printf(" %d", spaceTemp->start);
            spaceTemp = spaceTemp->next;
        } while (spaceTemp != buddySys[i].first);
        printf("\n");
    }

    return 0;
}