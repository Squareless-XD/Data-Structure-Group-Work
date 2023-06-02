// 6.20-二叉树的线索化
// Description
// 将下列森林转换为相应的二叉树，并分别按以下说明进行线索化：
// （1）先序前驱线索化；
// （2）中序全线索化前驱线索和后继线索；
// （3）后序后继线索化

// Input
// 第一行输入节点的个数n。
// 接下来的n行，每行包括3个数值，第一个数值表示当前节点值，第二个数值表示当前节点的第一个子节点的值，第三个数值表示当前节点的下一个兄弟的数值，空节点使用0表示。

// Output
// 按顺序以二叉树的方式输出三种线索化的结果，即输出为6行，1、2行分别为（1）即先序前驱线索化后的L数组和R数组，3、4行分别为（2）即中序全线索化前驱线索和后及线索后的L数组和R数组，5、6行分别为（3）即后继后续线索化的L数组和R数组。
// 二叉树的表示方式详见[6.33](http://42.192.84.166/problem/1035%60)，其中数组下标i表示第i个节点（如i=3表示图中节点值为3的节点）。

// Sample Input 1
// 15
// 1 2 0
// 2 3 0
// 3 0 4
// 4 0 5
// 5 6 0
// 6 8 7
// 7 0 0
// 8 0 0
// 9 10 0
// 10 0 0
// 11 0 0
// 12 13 0
// 13 15 14
// 14 0 0
// 15 0 0
// Sample Output 1
// 1 2 3 2 3 6 8 8 6 10 9 10 13 15 15 13
// 1 9 0 4 5 0 7 0 0 11 0 12 0 14 0 0
// 1 2 3 0 3 6 8 6 4 10 1 9 13 15 13 11
// 1 9 1 4 5 2 7 5 6 11 9 12 0 14 12 13
// 1 2 3 0 0 6 8 0 0 10 0 0 13 15 0 0
// 1 9 10 4 5 4 7 6 7 11 15 12 11 14 13 14

#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -1
#define INFEASIBLE -2
#define OVERFLOW_TREE -3
#define OVERFLOW_STACK -3
#define LINK 0
#define THREAD 1

vector<int> lChildren, rChildren;

typedef int status;
typedef int biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    int lTag;
    int rTag;
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
status biTreeThreaded(biTree tree);
status addNewThread(biTNode *node_next, biTNode *node_prev);
status biTreeSetFlag(biTree tree);
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
status biTreeThreadedPre(biTNode *root);
status biTreeTrvIn(biTNode *root, status (*visit)(biTNode *tNode)); // --
status biTreeTrvPost(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeTrvPostStack(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeThreadedPost(biTNode *root);
status biTreeTrvLevel(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeTrvLevelNew(biTNode *root);
status biTreeFullJudge(biTNode *root);
status biTreeCompleteJudge(biTNode *root);
status printTwoVector(vector<int> lChildren, vector<int> rChildren, int elemNum);

// alloc a node, with the lchild storing "root" and rchild storing "NULL". data is 0 since we don't need it
status biTreeInit(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    *ptree = (biTree)malloc(sizeof(biTNode));
    (*ptree)->data = 0;
    (*ptree)->lTag = 0;
    (*ptree)->rTag = 0;
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

// create thread in a binary tree
status biTreeThreaded(biTree tree)
{
    biTreeSetFlag(tree->lchild);

    sqStack *tNodeStack;
    biTNode *nodeTemp, *nodeLast;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, tree->lchild);
    nodeLast = NULL;

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == FALSE)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            addNewThread(nodeTemp, nodeLast); // add new thread between two node
            nodeLast = nodeTemp;              // store the last visited node
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
status biTreeSetFlag(biTree root)
{
    if (root->lchild == NULL)
        root->lTag = THREAD;
    else
    {
        root->lTag = LINK;
        biTreeSetFlag(root->lchild);
    }

    if (root->rchild == NULL)
        root->rTag = THREAD;
    else
    {
        root->rTag = LINK;
        biTreeSetFlag(root->rchild);
    }
    return OK;
}

// destroy a tree
status biTreeDestroy(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    if (*ptree == NULL)
        return OK;
    if ((*ptree)->lTag == LINK)
        if (biTreeDesNode((*ptree)->lchild) != OK)
            return ERROR;
    if ((*ptree)->rTag == LINK)
        if (biTreeDesNode((*ptree)->rchild) != OK)
            return ERROR;
    free(*ptree);
    *ptree = NULL;
    return OK;
}

status biTreeDesNode(biTNode *node)
{
    if (node == NULL)
        return OK;
    if (node->lTag == LINK)
        if (biTreeDesNode(node->lchild) != OK)
            return ERROR;
    if (node->rTag == LINK)
        if (biTreeDesNode(node->rchild) != OK)
            return ERROR;
    free(node);
    return OK;
}

status biTreeClear(biTree tree)
{
    if (tree == NULL)
        return ERROR;
    if (tree->lTag == LINK)
        if (biTreeDesNode(tree->lchild) != OK)
            return ERROR;
    if (tree->rTag == LINK)
        if (biTreeDesNode(tree->rchild) != OK)
            return ERROR;
    tree->data = 0;
    tree->lTag = 0;
    tree->rTag = 0;
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
    if (root->lTag == THREAD && root->rTag == THREAD) // no child
        return 1;
    if (root->lTag == THREAD) // no lchild, but it has rchild
        return biTreeDepth(root->rchild) + 1;
    if (root->rTag == THREAD) // no rchild, but it has lchild
        return biTreeDepth(root->lchild) + 1;
    // has 2 children
    return max(biTreeDepth(root->lchild), biTreeDepth(root->rchild)) + 1;
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

status biTreeThreadedPre(biTNode *root)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp, *lastNode;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);
    lastNode = NULL;

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        pop_Sq(tNodeStack, &nodeTemp);
        if (nodeTemp == NULL)
            continue;
        push_Sq(tNodeStack, nodeTemp->rchild); // mind the sequence. right push first
        push_Sq(tNodeStack, nodeTemp->lchild);

        if (nodeTemp->lchild == NULL)
        {
            nodeTemp->lTag = THREAD;
            nodeTemp->lchild = lastNode;
        }
        else
            nodeTemp->lTag = LINK;
        nodeTemp->rTag = LINK;
        lastNode = nodeTemp;
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

// a very cool way to perform post-order traverse
status biTreeTrvPostStack(biTNode *root, status (*visit)(biTNode *tNode))
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    if (root == NULL)
        return OK;
    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        getTop_Sq(tNodeStack, &nodeTemp);
        if (nodeTemp != NULL)
        {
            push_Sq(tNodeStack, NULL);
            if (nodeTemp->rchild != NULL)
                push_Sq(tNodeStack, nodeTemp->rchild);
            if (nodeTemp->lchild != NULL)
                push_Sq(tNodeStack, nodeTemp->lchild);
        }
        else
        {
            pop_Sq(tNodeStack, &nodeTemp); // get NULL
            pop_Sq(tNodeStack, &nodeTemp); // get node
            visit(nodeTemp);
        }
    }

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return OK;
}

status biTreeThreadedPost(biTNode *root)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp, *lastNode;

    if (root == NULL)
        return OK;
    lastNode = NULL;
    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, root);

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        getTop_Sq(tNodeStack, &nodeTemp);
        if (nodeTemp != NULL)
        {
            push_Sq(tNodeStack, NULL);
            if (nodeTemp->rchild != NULL)
                push_Sq(tNodeStack, nodeTemp->rchild);
            if (nodeTemp->lchild != NULL)
                push_Sq(tNodeStack, nodeTemp->lchild);
        }
        else
        {
            pop_Sq(tNodeStack, &nodeTemp); // get NULL
            pop_Sq(tNodeStack, &nodeTemp); // get node

            if (lastNode == NULL)
            {
                lastNode = nodeTemp;
                continue;
            }

            if (lastNode->rchild == NULL)
            {
                lastNode->rTag = THREAD;
                lastNode->rchild = nodeTemp;
            }
            else
                lastNode->rTag = LINK;
            lastNode->lTag = LINK;
            lastNode = nodeTemp;
        }
    }
    if (lastNode != NULL)
    {
        if (lastNode->rchild == NULL)
        {
            lastNode->rTag = THREAD;
            lastNode->rchild = NULL;
        }
        else
            lastNode->rTag = LINK;
        lastNode->lTag = LINK;
    }

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return OK;
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

status biTreeTrvLevelNew(biTNode *root)
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;

    if (root == NULL)
        return OK;

    tNodeQueue = queueAlloc();
    initQueue(tNodeQueue);
    enqueue(tNodeQueue, root);

    while (queueEmpty(tNodeQueue) == FALSE)
    {
        dequeue(tNodeQueue, &nodeTemp);

        if (nodeTemp->lchild != NULL)
            lChildren[nodeTemp->data] = nodeTemp->lchild->data;
        else
            lChildren[nodeTemp->data] = 0;
        if (nodeTemp->rchild != NULL)
            rChildren[nodeTemp->data] = nodeTemp->rchild->data;
        else
            rChildren[nodeTemp->data] = 0;

        if (nodeTemp->lTag == LINK && nodeTemp->lchild != NULL)
            enqueue(tNodeQueue, nodeTemp->lchild);
        if (nodeTemp->rTag == LINK && nodeTemp->rchild != NULL)
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

status printTwoVector(vector<int> lChildren, vector<int> rChildren, int elemNum)
{
    int i;

    for (i = 0; i < elemNum; ++i)
        cout << lChildren[i] << ' ';
    cout << lChildren[elemNum] << endl;

    for (i = 0; i < elemNum; ++i)
        cout << rChildren[i] << ' ';
    cout << rChildren[elemNum] << endl;

    return OK;
}

int main()
{
    int elemNum, i, lastRoot;
    vector<int> dataInput, childInput, siblingInput, isRootCheck;
    vector<biTree> preTree, inTree, postTree;
    // vector<biTree>::iterator tBegin, tEnd;
    biTree preOrdTree, inOrdTree, postOrdTree;

    biTreeInit(&preOrdTree);
    biTreeInit(&inOrdTree);
    biTreeInit(&postOrdTree);

    cin >> elemNum;
    dataInput = vector<int>(elemNum + 1);
    childInput = vector<int>(elemNum + 1);
    siblingInput = vector<int>(elemNum + 1);
    isRootCheck = vector<int>(elemNum + 1, TRUE);
    lChildren = vector<int>(elemNum + 1);
    rChildren = vector<int>(elemNum + 1);
    preTree = vector<biTree>(elemNum + 1);
    inTree = vector<biTree>(elemNum + 1);
    postTree = vector<biTree>(elemNum + 1);
    // tBegin = preTree.begin();
    // tEnd = preTree.end();
    for (i = 1; i <= elemNum; ++i)
    {
        preTree[i] = (biTree)malloc(sizeof(biTNode));
        inTree[i] = (biTree)malloc(sizeof(biTNode));
        postTree[i] = (biTree)malloc(sizeof(biTNode));
    }
    for (i = 1; i <= elemNum; ++i)
    {
        cin >> dataInput[i] >> childInput[i] >> siblingInput[i];
        preTree[i]->data = inTree[i]->data = postTree[i]->data = dataInput[i];

        if (childInput[i] != 0)
        {
            preTree[i]->lchild = preTree[childInput[i]];
            inTree[i]->lchild = inTree[childInput[i]];
            postTree[i]->lchild = postTree[childInput[i]];
            isRootCheck[childInput[i]] = FALSE;
        }
        else
            preTree[i]->lchild = inTree[i]->lchild = postTree[i]->lchild = NULL;
        if (siblingInput[i] != 0)
        {
            preTree[i]->rchild = preTree[siblingInput[i]];
            inTree[i]->rchild = inTree[siblingInput[i]];
            postTree[i]->rchild = postTree[siblingInput[i]];
            isRootCheck[siblingInput[i]] = FALSE;
        }
        else
            preTree[i]->rchild = inTree[i]->rchild = postTree[i]->rchild = NULL;
    }

    // consider connecting the forest

    lastRoot = 1;
    for (i = 2; i <= elemNum; ++i)
    {
        if (isRootCheck[i] != TRUE)
            continue;
        preTree[lastRoot]->rchild = preTree[i];
        inTree[lastRoot]->rchild = inTree[i];
        postTree[lastRoot]->rchild = postTree[i];
        lastRoot = i;
    }

    preOrdTree->lchild = preTree[1];
    inOrdTree->lchild = inTree[1];
    postOrdTree->lchild = postTree[1];

    rChildren[0] = 1;
    lChildren[0] = 1;

    // need to translate into output
    biTreeThreadedPre(preOrdTree->lchild);
    biTreeTrvLevelNew(preOrdTree->lchild);
    printTwoVector(lChildren, rChildren, elemNum);

    biTreeThreaded(inOrdTree);
    biTreeTrvLevelNew(inOrdTree->lchild);
    printTwoVector(lChildren, rChildren, elemNum);

    biTreeThreadedPost(postOrdTree->lchild);
    biTreeTrvLevelNew(postOrdTree->lchild);
    printTwoVector(lChildren, rChildren, elemNum);

    return 0;
}
