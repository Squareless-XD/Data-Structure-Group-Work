// 6.48-求距离两个二叉树节点最近的共同祖先
// Description
// 已知在二叉树中，*root为根节点，*p和*q为二叉树中的两个节点，试编写求距离它们最近的共同祖先的算法。
// 其中，建立二叉树的节点输入按照先序序列依次读入，比如一个二叉树的示意图如下：
// 它的先序序列为124^^5^^3^^,其中^表示搜索到空节点。

// Input
// 输入分为两行：
// 第一行将初始化二叉树各个节点的先序序列作为输入；
// 第二行将输入两个需要搜索距离它们最近的共同祖先的节点字符，中间以空格隔开。
// 注意节点的类型是字符

// Output
// 输出则是距离给定的两个节点最近的共同祖先的节点字符。

// Sample Input 1       Sample Input 2
// 124^^5^^3^^          124^^57^^8^^36^9^^^
// 4 3                  6 9
// Sample Output 1      Sample Output 2
// 1                    3

// Hint
// 如果p和q两个节点本身互为祖先子孙关系，则输出祖先节点(即p)的父亲节点，如sample2所示。
// 如果此时p为根节点则直接输出p。

#include <bits/stdc++.h>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -1
#define OVERFLOW_TREE -3

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

status biTreeInit(biTree *tree);
status biTreeCreate(biTree tree); // --
status judgeDataClass(char data);
status biTreePrintExprs(biTNode *root, status bracketSign);
status biTreeParent(biTree tree, biTElemType elemFind, biTNode **pParent);

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

// create a binary tree from input in pre-order.
status biTreeCreate(biTNode **proot)
{
    biTNode *tNode;
    char chdata;

    if ((chdata = getchar()) == '^' || chdata == EOF)
        tNode = NULL;
    else
    {
        if ((tNode = (biTNode *)malloc(sizeof(biTNode))) == NULL)
            return OVERFLOW_TREE;
        tNode->data = chdata;
        biTreeCreate(&tNode->lchild);
        biTreeCreate(&tNode->rchild);
    }
    *proot = tNode;
    return OK;
}

status judgeDataClass(char data)
{
    if (data == '*' || data == '/')
        return 2;
    if (data == '+' || data == '-')
        return 1;
    if (isdigit(data))
        return 0;
    return -1;
}

status biTreePrintExprs(biTNode *root, status bracketSign)
{
    if (root == NULL)
        return OK;

    if (bracketSign == TRUE && judgeDataClass(root->data) >= 1)
        putchar('(');

    // left child
    if (root->lchild != NULL && judgeDataClass(root->data) == 2 && judgeDataClass(root->lchild->data) == 1)
        biTreePrintExprs(root->lchild, TRUE);
    else if (root->lchild != NULL)
        biTreePrintExprs(root->lchild, FALSE);
    // itself
    putchar(root->data);
    // right child
    if (root->rchild != NULL &&
        (root->data == '/' || root->data == '*' && judgeDataClass(root->rchild->data) == 1 || root->data == '-' && judgeDataClass(root->rchild->data) == 1))
        biTreePrintExprs(root->rchild, TRUE);
    else if (root->rchild != NULL)
        biTreePrintExprs(root->rchild, FALSE);

    if (bracketSign == TRUE && judgeDataClass(root->data) >= 1)
        putchar(')');

    return OK;
}

status biTreeParent(biTree tree, biTElemType elemFind, biTNode **pParent)
{
    linkedQueue *tNodeQueue;
    biTNode *nodeTemp;
    int findSign;

    *pParent = NULL;
    if (tree == NULL || elemFind == '^')
        return ERROR;
    if (tree->lchild == NULL || tree->lchild->data == elemFind)
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
            if (nodeTemp->lchild->data == elemFind)
            {
                *pParent = nodeTemp;
                findSign = TRUE;
                break;
            }
            enqueue(tNodeQueue, nodeTemp->lchild);
        }
        if (nodeTemp->rchild != NULL)
        {
            if (nodeTemp->rchild->data == elemFind)
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

int main()
{
    biTree tree;
    char elem1, elem2, elemTemp;
    status printSign = FALSE;
    std::set<biTElemType> node1Anct;
    biTNode *parentNode;

    biTreeInit(&tree);
    biTreeCreate(&tree->lchild);
    getchar();

    std::cin >> elem1 >> elem2;

    elemTemp = elem1;
    if (tree->lchild->data == elem1 || tree->lchild->data == elem2)
    {
        std::cout << tree->lchild->data << '\n';
        return 0;
    }

    while (biTreeParent(tree, elemTemp, &parentNode) == TRUE)
    {
        elemTemp = parentNode->data;
        node1Anct.insert(elemTemp);
    }

    elemTemp = elem2;
    while (biTreeParent(tree, elemTemp, &parentNode) == TRUE)
    {
        if (node1Anct.find(parentNode->data) != node1Anct.end()) // if found
        {
            printSign = TRUE;
            std::cout << parentNode->data << '\n';
            break;
        }
        elemTemp = parentNode->data;
    }

    if (printSign == FALSE)
        std::cout << tree->lchild->data << '\n';

    return 0;
}