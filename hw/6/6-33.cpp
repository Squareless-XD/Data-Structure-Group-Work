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
    vector<int> L;
    vector<int> R;
    linkedQueue *nodeQueue;

    int d, u, v, temp, sign, elemNum;
    char c;

    nodeQueue = queueAlloc();
    initQueue(nodeQueue);

    while (scanf("%d%c", &d, &c), c == ' ')
        L.push_back(d);
    L.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        R.push_back(d);
    R.push_back(d);

    cin >> u >> v;

    elemNum = L.size() - 1;

    // is u a successor of v? or, is v an ancestor of u?
    temp = v;
    sign = FALSE;

    if (L[temp] != 0)                       // 丢进去两个不为空的子结点
        enqueue(nodeQueue, L[temp]);
    if (R[temp] != 0)
        enqueue(nodeQueue, R[temp]);
    while (queueEmpty(nodeQueue) == FALSE)  // 还有选项可以检查
    {
        dequeue(nodeQueue, &temp);          // 弹出队列一个元素
        if (temp == u)                      // 如果是u, 那么找到
        {
            sign = TRUE;
            break;
        }
        if (temp == 0)                      // 如果是0, 那么为空
            continue;
        if (L[temp] != 0)                   // 没找到且不为空, 就继续往里塞
            enqueue(nodeQueue, L[temp]);
        if (R[temp] != 0)
            enqueue(nodeQueue, R[temp]);
    }

    if (sign == TRUE)
        cout << 1 << endl;
    else
        cout << 0 << endl;
    
    return 0;

}
