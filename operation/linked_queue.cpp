#include <bits/stdc++.h>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1

typedef int queueElemType;
typedef int status;

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
status clearQueue(linkedQueue *queue);
status queueEmpty(linkedQueue *queue);
int queueLength(linkedQueue *queue);
status getFront(linkedQueue *queue, queueElemType *elem);
status enqueue(linkedQueue *queue, queueElemType elem);
status dequeue(linkedQueue *queue, queueElemType *elem);
status queueTraverse(linkedQueue *queue, status (*visit)(queueNode *node));

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

// clear a queue
status clearQueue(linkedQueue *queue)
{
    queueNode *p, *temp;
    p = queue->front->next;
    queue->rear = queue->front;
    while (p)
    {
        temp = p->next;
        free(p);
        p = temp;
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

// get the length of the queue
int queueLength(linkedQueue *queue)
{
    int length = 0;
    queueNode *p;

    if (queue->front == queue->rear)
        return 0;
    p = queue->front->next;
    while (p)
    {
        ++length;
        p = p->next;
    }
    return length;
}

// get the front element from the queue without letting it out
status getFront(linkedQueue *queue, queueElemType *elem)
{
    if (queue->front == queue->rear)
        return ERROR;
    *elem = queue->front->data;
    return OK;
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

status queueTraverse(linkedQueue *queue, status (*visit)(queueNode *node))
{
    queueNode *p;
    if (queue->front == queue->rear)
        return ERROR;
    p = queue->front->next;
    while (p)
    {
        visit(p);
        p = p->next;
    }
    return OK;
}
