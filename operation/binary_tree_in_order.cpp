#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_TREE -2
#define OVERFLOW_STACK -2
#define LINK 0
#define THREAD 1

typedef int status;
typedef char biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    int rTag; // rTag == 0 means it is storing a rchild, 1 means a succ
    int lTag;
    struct _biTNode *lchild;
    struct _biTNode *rchild;
} biTNode;


typedef biTNode *queueElemType;
typedef biTNode *stackElemType;

typedef struct qNode
{
    queueElemType data;
    struct qNode *next;
} queueNode;

typedef struct
{
    queueNode *front;
    queueNode *rear;
} linkedQueue;

typedef struct
{
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

linkedQueue *queueAlloc(void);
status initQueue(linkedQueue *queue);
status destoryQueue(linkedQueue *queue);
status queueEmpty(linkedQueue *queue);
status enqueue(linkedQueue *queue, queueElemType elem);
status dequeue(linkedQueue *queue, queueElemType *elem);

// get a pointer to a linked queue
linkedQueue *queueAlloc(void)
{
    linkedQueue *queue;
    queue = (linkedQueue *)malloc(sizeof(linkedQueue));
    if (!queue)
        return NULL;
    return queue;
}

// initialize a new linked queue
status initQueue(linkedQueue *queue)
{
    queue->front = queue->rear = (queueNode *)malloc(sizeof(queueNode));
    if (!queue)
        return ERROR;
    queue->front->next = NULL;
    return OK;
}

// destory a queue
status destoryQueue(linkedQueue *queue)
{
    while (queue->front)
    {
        queue->rear = queue->front->next;
        free(queue->front);
        queue->front = queue->rear;
    }
    return OK;
}

// check whether a queue is empty. if so, return TRUE
status queueEmpty(linkedQueue *queue)
{
    if (queue->front == queue->rear)
        return TRUE;
    return FALSE;
}

// put a new element into the queue
status enqueue(linkedQueue *queue, queueElemType elem)
{
    queueNode *newNode;
    newNode = (queueNode *)malloc(sizeof(queueNode));
    if (!newNode)
        return ERROR;
    newNode->data = elem;
    newNode->next = NULL;
    queue->rear->next = newNode;
    queue->rear = newNode;
    return OK;
}

// take out the oldest element from the queue
status dequeue(linkedQueue *queue, queueElemType *elem)
{
    queueNode *frontTemp;
    if (queue->front == queue->rear)
        return ERROR;
    frontTemp = queue->front->next; // since this node contains the first element that is ever stored
    *elem = frontTemp->data;
    queue->front->next = frontTemp->next;
    if (queue->rear == frontTemp)
        queue->rear = queue->front;
    free(frontTemp);
    return OK;
}

sqStack *stackAlloc(void);
status initStack_Sq(sqStack *stack);
status destroyStack_Sq(sqStack *stack);
status stackEmpty_Sq(sqStack *stack);
status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);

sqStack *stackAlloc(void)
{
    sqStack *stack;
    stack = (sqStack *)malloc(sizeof(sqStack));
    if (!stack)
        return NULL;
    return stack;
}

status initStack_Sq(sqStack *stack)
{
    stack->base = (stackElemType *)malloc(STACK_INIT_SIZE * sizeof(stackElemType));
    if (!stack->base)
        exit(OVERFLOW_STACK);
    stack->top = stack->base;
    stack->stacksize = STACK_INIT_SIZE;
    return OK;
}

status destroyStack_Sq(sqStack *stack)
{
    free(stack->base);
    return OK;
}

status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return TRUE;
    return FALSE;
}

status getTop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *(stack->top - 1);
    return OK;
}

status push_Sq(sqStack *stack, stackElemType e)
{
    if (stack->top - stack->base >= stack->stacksize)
    {
        stack->base = (stackElemType *)realloc(stack->base, (stack->stacksize + STACK_INCREMENT) * sizeof(stackElemType));
        if (!stack->base)
            exit(OVERFLOW_STACK);
        stack->top = stack->base + stack->stacksize;
        stack->stacksize += STACK_INCREMENT;
    }
    *(stack->top++) = e;
    return OK;
}

status pop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *--stack->top;
    return OK;
}

status biTreeInit(biTNode *biTree);
status biTreeCreate(biTNode *biTree); // --
status biTreeThreaded(biTNode *biTree);
status addNewThread(biTNode *node_next, biTNode *node_prev);
status biTreeSetFlag(biTNode *biTree);
status biTreeDestroy(biTNode **biTree);
status biTreeClear(biTNode *biTree);
status biTreeEmpty(biTNode *biTree);
int biTreeDepth(biTNode *biTree);
status biTreeFindElem(biTNode *biTree, biTElemType elemFind, biTNode **firstNode);
status biTreeFindNode(biTNode *biTree, biTNode *tNodeFind);
status biTreeAssign(biTNode *biTree, biTNode *tNode, biTElemType elemAssign);
status biTreeParent(biTNode *biTree, biTNode *tNodeFind, biTNode **parent);
status biTreeLSibling(biTNode *biTree, biTNode *tNodeFind, biTNode **pLSibling);
status biTreeRSibling(biTNode *biTree, biTNode *tNodeFind, biTNode **pRSibling);
status biTreeDeleteChild(biTNode *biTree, biTNode *tNodeFind, int isRChild);
status biTreeTrvPre(biTNode *biTree, status (*visit)(biTNode *tNode)); // --
status biTreeTrvPreStack(biTNode *biTree, status (*visit)(biTNode *tNode));
status biTreeTrvPost(biTNode *biTree, status (*visit)(biTNode *tNode));
status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode));  // --
status biTreeTrvLevel(biTNode *biTree, status (*visit)(biTNode *tNode));

status biTreeInit(biTNode **biTree)
{
    if (biTree == NULL)
        return ERROR;
    *biTree = (biTNode *)malloc(sizeof(biTNode));
    return OK;
}

// create a binary tree from input in pre-order.
status biTreeCreate(biTNode **biTree)
{
    biTNode *tNode;
    char chdata;

    if (isspace(chdata = getchar()) || chdata == EOF)
        tNode = NULL;
    else
    {
        if ((tNode = (biTNode *)malloc(sizeof(biTNode))) == NULL)
            return OVERFLOW_TREE;
        tNode->data = chdata;
        biTreeCreate(tNode->lchild);
        biTreeCreate(tNode->rchild);
    }
    *biTree = tNode;

    return OK;
}

// create thread in a binary tree
status biTreeThreaded(biTNode *biTree)
{
    biTreeSetFlag(biTree);

    sqStack *tNodeStack;
    biTNode *nodeTemp, *nodeLast;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);
    nodeLast = NULL;

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == FALSE)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            addNewThread(nodeTemp, nodeLast);   // add new thread between two node
            nodeLast = nodeTemp;                // store the last visited node
            push_Sq(tNodeStack, nodeTemp->rchild);
        }
    }
    addNewThread(NULL, nodeLast);

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return OK;
}

status addNewThread(biTNode *node_next, biTNode *node_prev)
{
    if (node_next != NULL && node_next->lTag == THREAD)
        node_next->lchild = node_prev;
    if (node_prev != NULL && node_prev->rTag == THREAD)
        node_prev->rchild = node_next;
    return OK;
}

// set lTag and rTag
status biTreeSetFlag(biTNode *biTree)
{
    if (biTree->lchild == NULL)
        biTree->lTag = THREAD;
    else
    {
        biTree->lTag = LINK;
        biTreeSetFlag(biTree->lchild);
    }

    if (biTree->rchild == NULL)
        biTree->rTag = THREAD;
    else
    {
        biTree->rTag = LINK;
        biTreeSetFlag(biTree->rchild);
    }
    return OK;
}

status biTreeDestroy(biTNode **biTree)
{
    if (*biTree == NULL)
        return OK;
    biTreeClear((*biTree)->lchild);
    biTreeClear((*biTree)->rchild);
    free(*biTree);
    *biTree = NULL;
    return OK;
}

status biTreeClear(biTNode *biTree)
{
    if (biTree == NULL)
        return OK;
    biTreeClear(biTree->lchild);
    biTreeClear(biTree->rchild);
    free(biTree);
    return OK;
}

status biTreeEmpty(biTNode *biTree)
{
    if (biTree == NULL)
        return TRUE;
    return FALSE;
}

int biTreeDepth(biTNode *biTree)
{
    if (biTree == NULL)
        return 0;
    if (biTree->lTag == THREAD && biTree->rTag == THREAD) // no child
        return 1;
    if (biTree->lTag == THREAD) // no lchild, but it has rchild
        return biTreeDepth(biTree->rchild) + 1;
    if (biTree->rTag == THREAD) // no rchild, but it has lchild
        return biTreeDepth(biTree->lchild) + 1;
    // has 2 children
    return max(biTreeDepth(biTree->lchild), biTreeDepth(biTree->rchild)) + 1;
}

status biTreeFindElem(biTNode *biTree, biTElemType elemFind, biTNode **firstNode)
{
    if (biTree == NULL)
    {
        *firstNode = NULL;
        return FALSE;
    }
    if (biTree->data == elemFind)
    {
        *firstNode = biTree;
        return TRUE;
    }
    if (biTreeFindElem(biTree->lchild, elemFind, firstNode) == TRUE || biTreeFindElem(biTree->rchild, elemFind, firstNode) == TRUE)
        return TRUE;
    return FALSE;
}

status biTreeFindNode(biTNode *biTree, biTNode *tNodeFind)
{
    if (biTree == NULL)
        return FALSE;
    if (biTree == tNodeFind)
        return TRUE;
    if (biTreeFindNode(biTree->lchild, tNodeFind) == TRUE || biTreeFindNode(biTree->rchild, tNodeFind) == TRUE)
        return TRUE;
    return FALSE;
}

status biTreeAssign(biTNode *tNode, biTElemType elemAssign)
{
    tNode->data = elemAssign;
    return OK;
}

status biTreeParent(biTNode *biTree, biTNode *tNodeFind, biTNode **pParent)
{
    *pParent = NULL;
    if (biTree == NULL) // if the tree is empty, then the node is not in this tree
        return ERROR;
    if (biTree == tNodeFind)
        return OK;
    if (biTree->lchild == tNodeFind || biTree->rchild == tNodeFind)
    {
        *pParent = biTree;
        return OK;
    }

    if (biTreeParent(biTree->lchild, tNodeFind, pParent) == OK || biTreeParent(biTree->rchild, tNodeFind, pParent) == OK)
        return OK;
    return ERROR;
}

status biTreeLSibling(biTNode *biTree, biTNode *tNodeFind, biTNode **pLSibling)
{
    biTNode *parent;
    biTreeParent(biTree, tNodeFind, &parent);
    if (parent->rchild == tNodeFind)
    {
        *pLSibling = parent->lchild;
        return OK;
    }
    *pLSibling = NULL;
    return ERROR;
}

status biTreeRSibling(biTNode *biTree, biTNode *tNodeFind, biTNode **pRSibling)
{
    biTNode *parent;
    biTreeParent(biTree, tNodeFind, &parent);
    if (parent->lchild == tNodeFind)
    {
        *pRSibling = parent->rchild;
        return OK;
    }
    *pRSibling = NULL;
    return ERROR;
}

status biTreeDeleteChild(biTNode *biTree, biTNode *tNodeFind, int isRChild)
{
    if (biTreeFindNode(biTree, tNodeFind) == FALSE)
        return ERROR;
    if (isRChild == TRUE)
    {
        biTreeClear(tNodeFind->rchild);
        tNodeFind->rchild = NULL;
    }
    else if (isRChild == FALSE)
    {
        biTreeClear(tNodeFind->lchild);
        tNodeFind->lchild = NULL;
    }
    else
        return ERROR;
    return OK;
}

status biTreeTrvPre(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    if (biTree == NULL)
        return OK;
    if (visit(biTree) == OK && biTreeTrvPre(biTree->lchild, visit) == OK && biTreeTrvPre(biTree->rchild, visit) == OK)
        return OK;
    return ERROR;
}

status biTreeTrvPreStack(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        pop_Sq(tNodeStack, &nodeTemp);
        if (nodeTemp == NULL)
            continue;
        visit(nodeTemp);
        push_Sq(tNodeStack, nodeTemp->rchild);  // mind the sequence. right push first
        push_Sq(tNodeStack, nodeTemp->lchild);
    }
    return OK;
}

status biTreeTrvPost(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    if (biTree == NULL)
        return OK;
    if (biTreeTrvPost(biTree->lchild, visit) == OK && biTreeTrvPost(biTree->rchild, visit) == OK && visit(biTree) == OK)
        return OK;
    return ERROR;
}

status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    biTNode *temp;

    if (biTree == NULL)
        return OK;
    temp = biTree;
    while (temp != NULL)
    {
        while (temp->lTag == LINK)
            temp = temp->lchild;
        if (visit(temp) != OK)  // visit after reaching the bottom
            return ERROR;
        while (temp->rTag == THREAD && temp->rchild != NULL)
        {
            temp = temp->rchild;
            visit(temp);
        }
        temp = temp->rchild;
    }

    return OK;
}

status biTreeTrvLevel(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;

    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, biTree);

    while (queueEmpty(tNodeQueue) == FALSE)
    {
        dequeue(tNodeQueue, &nodeTemp);
        if (nodeTemp == NULL)
            continue;
        if (visit(nodeTemp) == ERROR)
            return ERROR;
        enqueue(tNodeQueue, nodeTemp->lchild);
        enqueue(tNodeQueue, nodeTemp->rchild);
    }

    destoryQueue(tNodeQueue);
    free(tNodeQueue);
    return OK;
}
