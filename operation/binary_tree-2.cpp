#include <bits/stdc++.h>

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -1
#define INFEASIBLE -2
#define OVERFLOW_TREE -3
#define OVERFLOW_STACK -3

typedef int status;
typedef char biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    struct _biTNode *lchild;
    struct _biTNode *rchild;
} biTNode;

typedef biTNode *biTree;

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

status biTreeInit(biTree *tree);
// status biTreeCreate(biTree tree); // --
status biTreeDestroy(biTree *ptree);
status biTreeDesNode(biTNode *node);
status biTreeClear(biTree tree);
status biTreeEmpty(biTree tree);
int biTreeDepth(biTNode *root);
status biTreeFindElem(biTree tree, biTElemType elemFind, biTNode **firstNode);
status biTreeFindNode(biTree tree, biTNode *tNodeFind);
// status biTreeAssign(biTree tree, biTNode *tNode, biTElemType elemAssign);
status biTreeParent(biTree tree, biTNode *tNodeFind, biTNode **parent);
status biTreeLSibling(biTree tree, biTNode *tNodeFind, biTNode **pLSibling);
status biTreeRSibling(biTree tree, biTNode *tNodeFind, biTNode **pRSibling);
status biTreeDeleteChild(biTree tree, biTNode *tNodeFind, int isRChild);
status biTreeTrvPre(biTNode *root, status (*visit)(biTNode *tNode)); // --
status biTreeTrvPreStack(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeTrvIn(biTNode *root, status (*visit)(biTNode *tNode)); // --
status biTreeTrvPost(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeTrvLevel(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeFullJudge(biTNode *root);
status biTreeCompleteJudge(biTNode *root);

// alloc a node, with the lchild storing "root" and rchild storing "NULL". data is 0 since we don't need it
status biTreeInit(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    *ptree = (biTree)malloc(sizeof(biTNode));
    (*ptree)->data = 0;
    (*ptree)->lchild = NULL;
    (*ptree)->rchild = NULL;
    return OK;
}

// // create a binary tree from input in pre-order.
// status biTreeCreate(biTree tree)
// {
//     biTNode *tNode;
//     char chdata;

//     if (isspace(chdata = getchar()) || chdata == EOF)
//         tNode = NULL;
//     else
//     {
//         if ((tNode = (biTNode *)malloc(sizeof(biTNode))) == NULL)
//             return OVERFLOW_TREE;
//         tNode->data = chdata;
//         biTreeCreate(tNode->lchild);
//         biTreeCreate(tNode->rchild);
//     }
//     return OK;
// }

// destroy a tree
status biTreeDestroy(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    if (*ptree == NULL)
        return OK;
    if (biTreeDesNode((*ptree)->lchild) != OK || biTreeDesNode((*ptree)->rchild) != OK)
        return ERROR;
    free(*ptree);
    *ptree = NULL;
    return OK;
}

status biTreeDesNode(biTNode *node)
{
    if (node == NULL)
        return OK;
    if (biTreeDesNode(node->lchild) != OK || biTreeDesNode(node->rchild) != OK)
        return ERROR;
    free(node);
    return OK;
}

status biTreeClear(biTree tree)
{
    if (tree == NULL)
        return ERROR;
    if (biTreeDesNode(tree->lchild) != OK || biTreeDesNode(tree->rchild) != OK)
        return ERROR;
    tree->data = 0;
    tree->lchild = NULL;
    tree->rchild = NULL;
    return OK;
}

status biTreeEmpty(biTree tree)
{
    if (tree == NULL)
        return ERROR;
    if (tree->lchild == NULL)
        return TRUE;
    return FALSE;
}

int biTreeDepth(biTNode *root)
{
    if (root == NULL)
        return 0;
    return std::max(biTreeDepth(root->lchild), biTreeDepth(root->rchild)) + 1;
}

status biTreeFindElem(biTree tree, biTElemType elemFind, biTNode **firstNode)
{

    if (tree == NULL)
    {
        *firstNode = NULL;
        return FALSE;
    }
    if (tree->lchild != NULL && tree->lchild->data == elemFind)
    {
        *firstNode = tree->lchild;
        return TRUE;
    }
    if (tree->rchild != NULL && tree->rchild->data == elemFind)
    {
        *firstNode = tree->rchild;
        return TRUE;
    }
    if (biTreeFindElem(tree->lchild, elemFind, firstNode) == TRUE || biTreeFindElem(tree->rchild, elemFind, firstNode) == TRUE)
        return TRUE;
    return FALSE;
}

status biTreeFindNode(biTree tree, biTNode *tNodeFind)
{
    if (tree == NULL)
        return FALSE;
    if (tree == tNodeFind)
        return TRUE;
    if (biTreeFindNode(tree->lchild, tNodeFind) == TRUE || biTreeFindNode(tree->rchild, tNodeFind) == TRUE)
        return TRUE;
    return FALSE;
}

// status biTreeAssign(biTNode *tNode, biTElemType elemAssign)
// {
//     tNode->data = elemAssign;
//     return OK;
// }

status biTreeParent(biTree tree, biTNode *tNodeFind, biTNode **pParent)
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;
    int findSign;

    *pParent = NULL;
    if (tree == NULL || tNodeFind == NULL)
        return ERROR;
    if (tree->lchild == NULL || tree->lchild == tNodeFind)
        return FALSE;

    findSign = FALSE;
    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, tree->lchild);

    while (queueEmpty(tNodeQueue) == FALSE)
    {
        dequeue(tNodeQueue, &nodeTemp);
        if (nodeTemp->lchild != NULL)
        {
            if (nodeTemp->lchild == tNodeFind)
            {
                *pParent = nodeTemp;
                findSign = TRUE;
                break;
            }
            enqueue(tNodeQueue, nodeTemp->lchild);
        }
        if (nodeTemp->rchild != NULL)
        {
            if (nodeTemp->rchild == tNodeFind)
            {
                *pParent = nodeTemp;
                findSign = TRUE;
                break;
            }
            enqueue(tNodeQueue, nodeTemp->rchild);
        }
    }

    destoryQueue(tNodeQueue);
    free(tNodeQueue);
    return findSign;
}

status biTreeLSibling(biTree tree, biTNode *tNodeFind, biTNode **pLSibling)
{
    biTNode *parent;

    if (biTreeParent(tree, tNodeFind, &parent) == ERROR)
        return ERROR;
    if (parent->rchild == tNodeFind)
    {
        *pLSibling = parent->lchild;
        return OK;
    }
    *pLSibling = NULL;
    return ERROR;
}

status biTreeRSibling(biTree tree, biTNode *tNodeFind, biTNode **pRSibling)
{
    biTNode *parent;

    if (biTreeParent(tree, tNodeFind, &parent) == ERROR)
        return ERROR;
    if (parent->lchild == tNodeFind)
    {
        *pRSibling = parent->rchild;
        return OK;
    }
    *pRSibling = NULL;
    return ERROR;
}

status biTreeDeleteChild(biTree tree, biTNode *tNodeFind, int isRChild)
{
    if (biTreeFindNode(tree, tNodeFind) == FALSE)
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

status biTreeTrvPre(biTNode *root, status (*visit)(biTNode *tNode))
{
    if (root == NULL)
        return OK;
    if (visit(root) == OK && biTreeTrvPre(root->lchild, visit) == OK && biTreeTrvPre(root->rchild, visit) == OK)
        return OK;
    return ERROR;
}

status biTreeTrvPreStack(biTNode *root, status (*visit)(biTNode *tNode))
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        pop_Sq(tNodeStack, &nodeTemp);
        if (nodeTemp == NULL)
            continue;
        visit(nodeTemp);
        push_Sq(tNodeStack, nodeTemp->rchild); // mind the sequence. right push first
        push_Sq(tNodeStack, nodeTemp->lchild);
    }
    return OK;
}

status biTreeTrvIn(biTNode *root, status (*visit)(biTNode *tNode))
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);

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

status biTreeTrvPost(biTNode *root, status (*visit)(biTNode *tNode))
{
    if (root == NULL)
        return OK;
    if (biTreeTrvPost(root->lchild, visit) == OK && biTreeTrvPost(root->rchild, visit) == OK && visit(root) == OK)
        return OK;
    return ERROR;
}

status biTreeTrvLevel(biTNode *root, status (*visit)(biTNode *tNode))
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;

    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, root);

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
status biTreeFullJudge(biTNode *root)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;
    int fullSign;

    if (root == NULL)
        return TRUE;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);
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

status biTreeCompleteJudge(biTNode *root)
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;
    int reachNull, completeSign;

    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, root);
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
        if (reachNull == TRUE) // this "if" passed only when last "if" isn't met
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
