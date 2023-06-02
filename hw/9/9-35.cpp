// 9.35-按题目要求输出二叉排序树中的数据元素
// Description
// 假设二叉排序树以后继线索链表作存储结构，编写输出该二叉排序树中所有大于a且小于b的关键字的算法。

// Input
// 输入分为三行：
// 第一行是创建了中序后继线索二叉排序树后，按中序输出的数据元素
// 第二行是表示题目中指代的判断条件a元素
// 第三行是表示题目中指代的判断条件b元素

// Output
// 将满足条件的数据元素按中序遍历输出

// Sample Input 1                                   Sample Output 1
// -45 -7 -6 -5 1 2 5 11 15 26 30 33 63 76 86       33 63
// 30
// 70

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
#define LINK 0
#define THREAD 1

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
status biTreeThreaded(biTNode *tree);
status addNewThread(biTNode *node_next, biTNode *node_prev);
status biTreeSetFlag(biTNode *tree);
status biTreeTrvPre(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType elem), biTNode *biTree2); // --
status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType minimum, biTElemType maximum), biTElemType minimum, biTElemType maximum);
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

status biTreeTrvPre(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType elem), biTNode *biTree2)
{
    if (biTree == NULL)
        return OK;
    if (visit(biTree2, biTree->data) == OK && biTreeTrvPre(biTree->lchild, visit, biTree2) == OK && biTreeTrvPre(biTree->rchild, visit, biTree2) == OK)
        return OK;
    return ERROR;
}

// create thread in a binary tree
status biTreeThreaded(biTNode *tree)
{
    biTreeSetFlag(tree->lchild);

    sqStack *tNodeStack;
    biTNode *nodeTemp, *nodeLast;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, tree->lchild);
    nodeLast = NULL;

    while (stackEmpty_Sq(tNodeStack) == false)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == false)
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
status biTreeSetFlag(biTNode *root)
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

status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode, biTElemType minimum, biTElemType maximum), biTElemType minimum, biTElemType maximum)
{
    sqStack *tNodeStack;
    biTNode *nodeTemp;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);

    while (stackEmpty_Sq(tNodeStack) == false)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == false)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            if (visit(nodeTemp, minimum, maximum) != OK)
                return ERROR;
            push_Sq(tNodeStack, nodeTemp->rchild);
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
    biTElemType data, minimum, maximum;
    char c;

    biTreeInit(&tree);
    scanf("%d%c", &data, &c);
    tree->data = data;
    while (c != '\n')
    {
        scanf("%d%c", &data, &c);
        biSortTreeInsert(tree, data);
    }

    // biTreeThreaded(tree);

    std::cin >> minimum >> maximum;

    // print the merged tree
    biTreeTrvIn(tree, [](biTNode *tNode, biTElemType minimum, biTElemType maximum) -> status {
        if (tNode->data <= minimum || tNode->data >= maximum)
            return OK;
        if (firstPrint == false)
            firstPrint = true;
        else
            printf(" ");
        printf("%d", tNode->data);
        return OK;
    }, minimum, maximum);

    return 0;
}

