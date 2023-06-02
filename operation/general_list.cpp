#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_LIST -2
#define MAXSTRLEN 255


typedef char atomType;
typedef int status;

typedef enum
{
    ATOM,
    LIST,
    EMPTY
} elemTag; // ATOM = 0, LIST = 1

typedef struct _gLNodePtr
{
    struct _gLNode *lsHead;
    struct _gLNode *lsTail;
} gLPtr; // if this is a general list

typedef struct _gLNode
{
    elemTag tag; // to distinguish the node is atom or list
    union
    {
        atomType atom; // if this is an atom
        gLPtr ptr; // if this is a general list
    };         // storing an atom or a list, depending on the value of "tag"
} genList;


status gListAlloc(genList **glist);
status gListInit(genList *glist);
status gListCreate(genList *glist, char *listChars_init, int length);
status gListDestroy(genList *glist);
status gListDestroyStay(genList *glist);
status gListDestroyFree(genList *glist);
status gListCopy(genList *listDst, genList listSrc);
status gListLength(genList glist);
status gListDepth(genList glist);
status gListEmpty(genList glist);
status gListHead(genList glist);
status gListTail(genList glist);
status gListInsertFirst(genList *glist, atomType elem);
status gListDeleteFirst(genList *glist);
status gListDeleteMatch(genList *glist, atomType elem);
status gListPrint(genList *glist);


status gListAlloc(genList **glist)
{
    *glist = (genList *)malloc(sizeof(genList));
    if (*glist == NULL)
        return ERROR;
    return OK;
}

status gListInit(genList *glist)
{
    if (glist == NULL)
        return ERROR;

    glist->tag = EMPTY;
    glist->ptr = {NULL, NULL};

    return OK;
}

// Must make sure that lsitChars is an "array of char"!
// note: the "listChars" will be changed
status gListCreate(genList *glist, char *listChars_init, int length)
{
    int index, index2, index3, indexEnd, indexCopy, count, recLength;
    status tailExist;
    elemTag headTag;
    atomType headAtom;
    char c, recList[MAXSTRLEN + 1], listChars[MAXSTRLEN + 1];
    genList *newList;

    if (length >= MAXSTRLEN)
        return OVERFLOW_LIST;
    if (glist == NULL)
        return ERROR;

    for (index = 0; index <= length; ++index)
        listChars[index] = listChars_init[index];

    index = -1;
    while (isspace(listChars[++index])) // move until the next non-space char
        ;
    indexEnd = length;
    while (isspace(listChars[--indexEnd])) // move until the previous non-space char
        ;

    glist->tag = LIST; // the ATOM situation is excluded

    if (listChars[index] != '(' || listChars[indexEnd] != ')') // not a list
        return ERROR;

    while (isspace(listChars[++index])) // move until next non-space char
        ;
    if (listChars[index] == '(') // "head" can only be a list
    {
        headTag = LIST;

        index2 = index;
        count = 1;
        // use a stack to check the brackets
        while (count && index2 < indexEnd)
        {
            ++index2;
            if (listChars[index2] == '(')
                ++count;
            else if (listChars[index2] == ')')
                --count;
        }
        if (index2 == indexEnd) // reach the last close bracket
            return ERROR;

        // copy the string
        indexCopy = index2 - index;
        recLength = indexCopy + 1;
        index = index2; // for later matching
        recList[indexCopy + 1] = '\0';
        while (indexCopy >= 0)
        {
            recList[indexCopy] = listChars[index2];
            --index2;
            --indexCopy;
        }
    }
    else if (isalnum(listChars[index]))
    {
        headTag = ATOM;
        headAtom = listChars[index];
    }
    else if (index == indexEnd) // both empty. then we do not need to consider "lsHead==NULL"
    {
        glist->ptr.lsHead = NULL;
        glist->ptr.lsTail = NULL;
        return OK;
    }
    else
        return ERROR;

    // move to the next non-space position
    while (isspace(listChars[++index])) // move until next non-space char
        ;
    if (listChars[index] == ',')
    {
        tailExist = TRUE;
        listChars[index] = '(';
    }
    else if (index == indexEnd)
        tailExist = FALSE;
    else
        return ERROR;

    if (headTag == ATOM)
    {
        if (gListAlloc(&newList) != OK)
            return ERROR;
        glist->ptr.lsHead = newList;
        newList = NULL;

        glist->ptr.lsHead->tag = ATOM;
        glist->ptr.lsHead->atom = headAtom;
    }
    else if (headTag == LIST)
    {
        if (gListAlloc(&newList) != OK)
            return ERROR;
        glist->ptr.lsHead = newList;
        newList = NULL;

        if (gListCreate(glist->ptr.lsHead, recList, recLength) != OK)
        {
            gListDestroy(glist);
            return ERROR;
        }
    }
    else
        ERROR;

    if (tailExist == TRUE)
    {
        if (gListAlloc(&newList) != OK)
            return ERROR;
        glist->ptr.lsTail = newList;
        // newList = NULL;

        // copy the string
        indexCopy = indexEnd - index;
        recLength = indexCopy + 1;
        recList[indexCopy + 1] = '\0';
        while (indexCopy >= 0)
        {
            recList[indexCopy] = listChars[indexEnd];
            --indexEnd;
            --indexCopy;
        }

        if (gListCreate(glist->ptr.lsTail, recList, recLength) != OK)
        {
            gListDestroy(glist);
            return ERROR;
        }
    }
    else
        glist->ptr.lsTail = NULL;

    return OK;
}

status gListDestroy(genList *glist)
{
    if (glist == NULL)
        return OK;

    if (glist->tag == LIST)
    {
        gListDestroyFree(glist->ptr.lsHead);
        gListDestroyFree(glist->ptr.lsTail);
    }
    glist->tag = EMPTY;

    return OK;
}

status gListDestroyStay(genList *glist)
{
    if (glist == NULL)
        return OK;

    glist->tag = EMPTY;

    return OK;
}

status gListDestroyFree(genList *glist)
{
    if (glist == NULL)
        return OK;

    if (glist->tag == LIST)
    {
        gListDestroyFree(glist->ptr.lsHead);
        gListDestroyFree(glist->ptr.lsTail);
    }
    free(glist);

    return OK;
}

status gListCopy(genList *listDst, genList listSrc);
status gListLength(genList glist);
status gListDepth(genList glist);
status gListEmpty(genList glist);
status gListHead(genList glist);
status gListTail(genList glist);
status gListInsertFirst(genList *glist, atomType elem);
status gListDeleteFirst(genList *glist)
{
    gLPtr gListPtr;
    atomType atomTemp;

    if (glist == NULL || glist->tag == EMPTY)
        return ERROR;

    if (glist->tag == ATOM)
        glist->tag = EMPTY;
    else if (glist->tag == LIST)
    {
        gListDestroyFree(glist->ptr.lsHead);
        if (glist->ptr.lsTail == NULL)
            glist->tag = EMPTY;
        else if (glist->ptr.lsTail->tag == LIST)
        {
            gListPtr = glist->ptr.lsTail->ptr;
            free(glist->ptr.lsTail);
            glist->ptr = gListPtr;
        }
        else
            return ERROR;
    }
    else
        return ERROR;

    return OK;
}

status gListDeleteMatch(genList *glist, atomType elem)
{
    if (glist == NULL || glist->tag == EMPTY)
        return OK;

    else if (glist->tag == ATOM)
    {
        if (glist->atom == elem)
            glist->tag = EMPTY;
    }
    else if (glist->tag == LIST)
    {
        gListDeleteMatch(glist->ptr.lsTail, elem); // clear the tail
        // if with a tag EMPTY, change it into a "NULL pointer"
        if (glist->ptr.lsTail != NULL && glist->ptr.lsTail->tag == EMPTY)
        {
            free(glist->ptr.lsTail);
            glist->ptr.lsTail = NULL;
        }

        gListDeleteMatch(glist->ptr.lsHead, elem); // clear the head
        // if the head has nothing
        if (glist->ptr.lsHead == NULL || glist->ptr.lsHead->tag == EMPTY)
            gListDeleteFirst(glist);
    }
    else
        return ERROR;

    return OK;
}

// 5 means empty, OK means non-empty, ERROR means some error occured
status gListPrint(genList *glist)
{
    if (glist == NULL || glist->tag == EMPTY)
    {
        return 5;
    }
    else if (glist->tag == ATOM)
    {
        cout << glist->atom;
    }
    else if (glist->tag == LIST)
    {
        if (glist->ptr.lsHead == NULL)
            return 5;
        else if (glist->ptr.lsHead->tag == LIST)
        {
            putchar('(');
            gListPrint(glist->ptr.lsHead);
            putchar(')');
        }
        else if (glist->ptr.lsHead->tag == ATOM)
            gListPrint(glist->ptr.lsHead);
        else
            return ERROR;

        if (glist->ptr.lsTail == NULL || glist->ptr.lsTail->tag == EMPTY)
            return 5;
        if (glist->ptr.lsTail->tag == ATOM)
            return ERROR;
        putchar(',');
        gListPrint(glist->ptr.lsTail);
    }
    else
        return ERROR;
    return OK;
}
