// 6.49 判定完全二叉树
// Description
// 编写算法判定给定二叉树是否为完全二叉树。

// Input
// 输入为 n 行，表示 n 个节点的信息。
// 第 i，i∈[1,n] 行为节点 i 的信息，包括一个字符 c 和两个数字 p 和 q， 这表示第 i 个节点的内容为字符 c, 其左孩子为 p, 右孩子为 q。空节点使用 -1 表示。

// Output
// 如果是完全二叉树，则输出 字符串 `Yes`；如果不是，则输出字符串 `No`。

// Sample Input 1       Sample Input 2      Sample Input 3
// A -1 -1              A 2 -1              A 2 4
//                      B -1 -1             B -1 3
//                                          C -1 -1
//                                          D -1 -1
// Sample Output 1      Sample Output 2     Sample Output 3
// Yes                  Yes                 No

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

status biTreeCompleteJudge(biTNode *biTree);

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

int main()
{
    vector<int> lChildren, rChildren;
    vector<char> treeData;
    vector<biTNode *> tNodeAssign;
    biTNode *tree;
    int lTemp, rTemp, d, elemNum, i, completeSign;
    char dataTemp, c;

    treeData.push_back('0');
    lChildren.push_back(-1);
    rChildren.push_back(-1);
    while ((dataTemp = getchar()) != EOF)
    {
        scanf(" %d %d%c", &lTemp, &rTemp, &c);
        if (c == EOF)
            break;
        treeData.push_back(dataTemp);
        lChildren.push_back(lTemp);
        rChildren.push_back(rTemp);
    }

    elemNum = treeData.size() - 1;
    tNodeAssign = vector<biTNode *>(elemNum + 1);
    for (i = 1; i <= elemNum; ++i)
    {
        tNodeAssign[i] = (biTNode *)malloc(sizeof(biTNode));
        tNodeAssign[i]->data = treeData[i];
    }
    tree = tNodeAssign[1];
    for (i = 1; i <= elemNum; ++i)
    {
        if (lChildren[i] == -1)
            tNodeAssign[i]->lchild = NULL;
        else
            tNodeAssign[i]->lchild = tNodeAssign[lChildren[i]];
        if (rChildren[i] == -1)
            tNodeAssign[i]->rchild = NULL;
        else
            tNodeAssign[i]->rchild = tNodeAssign[rChildren[i]];
    }

    completeSign = biTreeCompleteJudge(tree);
    if (completeSign == TRUE)
        cout << "Yes" << endl;
    else
        cout << "No" << endl;

    return 0;
}
