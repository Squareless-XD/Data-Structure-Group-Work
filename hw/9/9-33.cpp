// 9.33-递归处理二叉排序树中关键字输出问题
// Description
// 编写递归算法，从大到小输出给定二叉排序树中所有关键字不小于x的数据元素

// Input
// 输入分为两部分：
// 第一行作为第一部分，就是给定的的二叉排序树各个数据元素
// 第二行作为第二部分，就是表示题目中指代的判断条件x元素

// Output
// 将满足条件的数据元素从大到小输出一行

// Sample Input 1                       Sample Output 1
// -9 0 1 2 7 8 15 24 31 36 43 45       45 43 36 31 24 15 8
// 8

#include <bits/stdc++.h>

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量

// #define true 1
// #define false 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_TREE -2
#define OVERFLOW_STACK -2

typedef int status;
typedef int biTElemType;

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

// check whether a queue is empty. if so, return true
status queueEmpty(linkedQueue *queue)
{
    if (queue->front == queue->rear)
        return true;
    return false;
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
        return true;
    return false;
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

status biTreeInit(biTNode **pbiTree);
biTNode *biTreeCreate(biTNode *biTree); // --
status biTreeTrvInRev(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType minimum), biTElemType minimum);  // --
status biSortTreeInsert(biTNode *biTree, biTElemType elemInsert);
status biSortTreeDelete(biTNode **pbiTree, biTElemType elemDelete);

status biTreeInit(biTNode **pbiTree)
{
    if (pbiTree == NULL)
        return ERROR;
    *pbiTree = (biTNode *)malloc(sizeof(biTNode));
    return OK;
}

// create a binary tree from input in pre-order.
biTNode *biTreeCreate(void)
{
    biTNode *tNode;
    biTElemType data;

    if ((tNode = (biTNode *)malloc(sizeof(biTNode))) == NULL)
        return NULL;
    scanf("%d", &data);
    tNode->data = data;

    if (data == -1)
        return NULL;

    tNode->data = data;
    tNode->lchild = biTreeCreate();
    tNode->rchild = biTreeCreate();
    return tNode;
}

status biTreeTrvInRev(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType minimum), biTElemType minimum)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);

    while (stackEmpty_Sq(tNodeStack) == false)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->rchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == false)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            if (visit(nodeTemp, minimum) != OK)
                return ERROR;
            push_Sq(tNodeStack, nodeTemp->lchild);
        }
    }

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return OK;
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

bool firstPrint = false;

int main()
{
    biTNode *tree;
    biTElemType data, minimum;
    char c;

    biTreeInit(&tree);
    scanf("%d%c", &data, &c);
    tree->data = data;
    while (c != '\n')
    {
        scanf("%d%c", &data, &c);
        biSortTreeInsert(tree, data);
    }

    std::cin >> minimum;

    // print the merged tree
    biTreeTrvInRev(tree, [](biTNode *tNode, biTElemType minimum) -> status {
        if (tNode->data < minimum)
            return OK;
        if (firstPrint == false)
            firstPrint = true;
        else
            printf(" ");
        printf("%d", tNode->data);
        return OK;
    }, minimum);

    return 0;
}
