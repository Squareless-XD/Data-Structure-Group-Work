// Description
// 假定用两个一维数组L[n+1]和R[n+1]作为有n个节点的二叉树的存储结构，L[i]和R[i]分别指示节点i(i=1,2,...,n)的左孩子和右孩子，0表示空。然后由L和R建立一维数组T[n+1]，使T中第i(i=1,2,...,n)个分量指示节点i的双亲，试写一个算法判别节点u是否为节点v的子孙。
// 例如，给定数组
// T= 0 0 1 1 2 2
// L= 0 2 4 0 0 0
// R=0 3 5 0 0 0
// 数组表示的二叉树的示意图如下：
// 注意：依据题目T,L,R的数组长度均为n+1,则它们数组的第一个元素均存为0。

// Input
// 第一行,第二行与第三行分别是T,L和R数组，元素之间使用空格分隔
// 第四行输入元素u和元素v，空格分隔

// Output
// 判断u是否为v的子孙，是则输出1，否则输出0

// Sample Input 1
// 0 0 1 1 2 2 3 5 5 6
// 0 2 4 6 0 7 0 0 0 0
// 0 3 5 0 0 8 9 0 0 0
// 7 3
// Sample Output 1
// 0

#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_TREE -2

typedef int status;
typedef int queueElemType;

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

int main()
{
    vector<int> T, L, R;
    linkedQueue *nodeQueue;

    int d, u, v, temp, sign, elemNum;
    char c;

    nodeQueue = queueAlloc();
    initQueue(nodeQueue);

    while (scanf("%d%c", &d, &c), c == ' ')
        T.push_back(d);
    T.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        L.push_back(d);
    L.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        R.push_back(d);
    R.push_back(d);

    cin >> u >> v;

    // u is v's succ?

    temp = u;
    sign = FALSE;
    while (T[temp] != 0)
    {
        if (T[temp] == v) // if correct
        {
            sign = TRUE;
            break;
        }
        temp = T[temp];
    }

    if (sign == TRUE)
        cout << 1 << endl;
    else
        cout << 0 << endl;

    return 0;
}
