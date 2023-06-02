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

typedef int status;
typedef char biTElemType;

typedef struct _biTNode
{
    biTElemType data;
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

status biTreeInit(biTNode **biTree);
status biTreeCreate(biTNode *biTree); // --
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
status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode));  // --
status biTreeTrvPost(biTNode *biTree, status (*visit)(biTNode *tNode));
status biTreeTrvLevel(biTNode *biTree, status (*visit)(biTNode *tNode));
status biTreeFullJudge(biTNode *biTree);
status biTreeCompleteJudge(biTNode *biTree);

status biTreeInit(biTNode **biTree)
{
    if (biTree == NULL)
        return ERROR;
    *biTree = (biTNode *)malloc(sizeof(biTNode));
    return OK;
}

// create a binary tree from input in pre-order.
status biTreeCreate(biTNode *biTree)
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

status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == FALSE)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            if (visit(nodeTemp) != OK)
                return ERROR;
            push_Sq(tNodeStack, nodeTemp->rchild);
        }
    }

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
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

// this "full" means every node has 0/2 children
status biTreeFullJudge(biTNode *biTree)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;
    int fullSign;

    if (biTree == NULL)
        return TRUE;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);
    fullSign = TRUE;

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        pop_Sq(tNodeStack, &nodeTemp);
        if ((nodeTemp->lchild != NULL) != (nodeTemp->rchild != NULL))
        {
            fullSign = FALSE;
            break;
        }
        if (nodeTemp->rchild != NULL && nodeTemp->lchild != NULL)
        {
            push_Sq(tNodeStack, nodeTemp->rchild);
            push_Sq(tNodeStack, nodeTemp->lchild);
        }
    }

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return fullSign;
}

status biTreeCompleteJudge(biTNode *biTree)
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;
    int reachNull, completeSign;

    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, biTree);
    reachNull = FALSE;
    completeSign = TRUE;

    while (queueEmpty(tNodeQueue) == FALSE)
    {
        dequeue(tNodeQueue, &nodeTemp);
        if (nodeTemp == NULL)
        {
            reachNull = TRUE;
            continue;
        }
        if (reachNull == TRUE)      // this "if" passed only when last "if" isn't met
        {
            completeSign = FALSE;
            break;
        }
        enqueue(tNodeQueue, nodeTemp->lchild);
        enqueue(tNodeQueue, nodeTemp->rchild);
    }

    destoryQueue(tNodeQueue);
    free(tNodeQueue);
    return completeSign;
}

// insert a new element into the binary sort tree
status biSortTreeInsert(biTNode *biTree, biTElemType elemInsert)
{
    biTNode *nodeTemp, *nodeNew;

    // check if the tree is empty
    if (biTree == NULL)
        return ERROR;

    // find the position to insert
    nodeTemp = biTree;
    while (nodeTemp != NULL)
    {
        // check if the element is already in the tree
        if (nodeTemp->data == elemInsert)
            return ERROR;
        // insert the element into the left child if it is smaller than the current node
        if (nodeTemp->data > elemInsert)
        {
            // if the left child is empty, then insert the element into the left child
            if (nodeTemp->lchild == NULL)
            {
                nodeNew = (biTNode *)malloc(sizeof(biTNode));
                nodeNew->data = elemInsert;
                nodeNew->lchild = nodeNew->rchild = NULL;
                nodeTemp->lchild = nodeNew;
                return OK;
            }
            // else, go to the left child
            nodeTemp = nodeTemp->lchild;
        }
        else
        {
            if (nodeTemp->rchild == NULL)
            {
                nodeNew = (biTNode *)malloc(sizeof(biTNode));
                nodeNew->data = elemInsert;
                nodeNew->lchild = nodeNew->rchild = NULL;
                nodeTemp->rchild = nodeNew;
                return OK;
            }
            nodeTemp = nodeTemp->rchild;
        }
    }
    return ERROR;
}

// delete an element from the binary sort tree
status biSortTreeDelete(biTNode **pbiTree, biTElemType elemDelete)
{
    biTNode *nodeTemp, *nodeParent, *nodeChild, *nodeChildParent, *nodeChildLeft, *nodeChildRight;

    // check if the tree is empty
    if (*pbiTree == NULL)
        return ERROR;
    
    // find the node to delete
    nodeTemp = *pbiTree;
    nodeParent = NULL;
    while (nodeTemp != NULL)
    {
        // if the element is found
        if (nodeTemp->data == elemDelete)
            break;
        // else, go to the left child if the element is smaller than the current node
        if (nodeTemp->data > elemDelete)
        {
            nodeParent = nodeTemp;
            nodeTemp = nodeTemp->lchild;
        }
        // else, go to the right child if the element is larger than the current node
        else
        {
            nodeParent = nodeTemp;
            nodeTemp = nodeTemp->rchild;
        }
    }
    // if the element is not found
    if (nodeTemp == NULL)
        return ERROR;
    
    // if the node to delete is a leaf node
    if (nodeTemp->lchild == NULL && nodeTemp->rchild == NULL)
    {
        // if the node to delete is the root node
        if (nodeParent == NULL)
            *pbiTree = NULL;
        // else, delete the node
        if (nodeParent->lchild == nodeTemp)
            nodeParent->lchild = NULL;
        else
            nodeParent->rchild = NULL;
        free(nodeTemp);
        return OK;
    }

    // if the node to delete has only one child
    if (nodeTemp->lchild == NULL || nodeTemp->rchild == NULL)
    {
        // if the node to delete is the root node
        if (nodeParent == NULL)
        {
            if (nodeTemp->lchild == NULL)
                *pbiTree = nodeTemp->rchild;
            else
                *pbiTree = nodeTemp->lchild;
        }
        // else, if the node to delete is the left child of its parent
        if (nodeParent->lchild == nodeTemp)
        {
            if (nodeTemp->lchild == NULL)
                nodeParent->lchild = nodeTemp->rchild;
            else
                nodeParent->lchild = nodeTemp->lchild;
        }
        // else, if the node to delete is the right child of its parent
        else
        {
            if (nodeTemp->lchild == NULL)
                nodeParent->rchild = nodeTemp->rchild;
            else
                nodeParent->rchild = nodeTemp->lchild;
        }
        free(nodeTemp);
        return OK;
    }

    // if the node to delete has two children. the situations before are all considered, so no if-else is needed

    // find the rightmost node of the left child of the node to delete
    nodeChildParent = nodeTemp;
    nodeChild = nodeTemp->lchild;
    while (nodeChild->rchild != NULL)
    {
        nodeChildParent = nodeChild;
        nodeChild = nodeChild->rchild;
    }

    // if the rightmost node of the left child of the node to delete is the left child of the node to delete
    if (nodeChildParent == nodeTemp)
    {
        nodeChild->rchild = nodeTemp->lchild;
        if (nodeParent == NULL)
            *pbiTree = nodeChild;
        else
        {
            if (nodeParent->lchild == nodeTemp)
                nodeParent->lchild = nodeChild;
            else
                nodeParent->rchild = nodeChild;
        }
    }

    // else, if the leftmost node of the right child of the node to delete is not the right child of the node to delete
    else
    {
        nodeChildLeft = nodeChild->lchild;
        nodeChild->lchild = nodeTemp->lchild;
        nodeChild->rchild = nodeTemp->rchild;
        nodeChildParent->rchild = nodeChildLeft;

        if (nodeParent == NULL)
            *pbiTree = nodeChild;
        else
        {
            if (nodeParent->lchild == nodeTemp)
                nodeParent->lchild = nodeChild;
            else
                nodeParent->rchild = nodeChild;
        }
    }
    free(nodeTemp);
    return OK;
}


