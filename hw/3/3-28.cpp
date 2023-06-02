// 3.28-队列实现
// Description
// 假设以带头结点的循环链表表示队列，并且只设一个指针指向队尾元素结点(注意不设斗指针)，试编写相应的队列初始化、入队列和出队列的算法

// Input
// 输入分为两行，第一行表示队列个数，
// 第二行表示入队列的元素序列，中间以逗号隔开

// Output
// 队列出队列的元素序列，中间以逗号隔开

// Sample Input 1 
// 5
// 1,2,3,4,5
// Sample Output 1
// 1,2,3,4,5


#include <bits/stdc++.h>
using namespace std;

#define queueElemType int

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW_QUEUE -2

typedef int status;

typedef struct qNode
{
    queueElemType data;
    struct qNode *next;
}queueNode;

typedef struct
{
    queueNode *front;
    queueNode *rear;
}linkedQueue;

linkedQueue *queueAlloc(void);
status initQueue (linkedQueue *queue);
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
status initQueue (linkedQueue *queue)
{
    queue->front = queue->rear = (queueNode *)malloc(sizeof(queueNode));
    if (!queue)
        return ERROR;
    queue->front->next = queue->front;
    return OK;
}

// check whether a queue is empty. if so, return OK
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
    queue->rear->next = newNode;
    queue->rear = newNode;
    newNode->next = queue->front->next;
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
    queue->rear->next = queue->front->next;
    return OK;
}

int main()
{
    int length, inputElem, outputElem, i;
    char c;
    linkedQueue *queue;

    queue = queueAlloc();
    initQueue(queue);

    cin >> length;
    for (i = 0; i < length; ++i)
    {
        cin >> inputElem >> c;
        enqueue(queue, inputElem);
        if (c != ',')
            break;
    }
    if (!queueEmpty(queue))
    {
        dequeue(queue, &outputElem);
        cout << outputElem;
    }
    while (!queueEmpty(queue))
    {
        dequeue(queue, &outputElem);
        cout << ',' << outputElem;
    }
    cout << endl;
    return 0;
}