#include <bits/stdc++.h>

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量
#define MAX_VERTEX_NUM 150

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2
#define OVERFLOW_GRAPH 3
#define OVERFLOW_STACK 3

typedef int status;
typedef int infoType;
typedef int vexType;

typedef enum
{
    directGraph,
    directNet,
    undirectGraph,
    undirectNet
} graphKind;

typedef struct arcNode
{
    int adjVex;              // to which point
    struct arcNode *nextArc; // the next arc
    infoType info;           // infomation about it
} arcNode;

typedef struct vNode
{
    vexType data;      // vertex data
    arcNode *firstArc; // the first arc
} vNode, adjList[MAX_VERTEX_NUM];

typedef struct
{
    adjList vexs;
    int vexNum;     // the number of verteces
    int arcNum;     // the number of arcs
    graphKind kind; // graph type
} aLGraph;

typedef int queueElemType;
typedef int stackElemType;

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

status createGraphAL(aLGraph *graph);
status createDGAL(aLGraph *graph, status infoInput);
// status createDNAL(aLGraph *graph, status infoInput);
// status createUDGAL(aLGraph *graph, status infoInput);
// status createUDNAL(aLGraph *graph, status infoInput);
status destoryGraphAL(aLGraph *graph);
status clearGraphAL(aLGraph *graph);
int locateVexAL(aLGraph *graph, vexType vertex);
status getVexAL(aLGraph *graph, int vexPos, vexType *value);
status putVexAL(aLGraph *graph, int vexPos, vexType value);
status firstAdjVexAL(aLGraph *graph, int vexPos, arcNode **pFirstArc);
status nextAdjVexAL(aLGraph *graph, arcNode firstAdjPos, arcNode **pNextArc);
status insertVexAL(aLGraph *graph, vexType vertex);
status deleteVexAL(aLGraph *graph, vexType vertex);
status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight);
status deleteArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo);
status DFSTraverseAL(aLGraph *graph, status (*visit)(vexType vertex));
status BFSTraverseAL(aLGraph *graph, status (*visit)(vexType vertex));
int allVisited(status vexVisited[], int length);

/*
typedef struct arcNode
{
    int adjVex;              // to which point
    struct arcNode *nextArc; // the next arc
    infoType *info;          // infomation about it
} arcNode;

typedef struct vNode
{
    vexType data;     // vertex data
    arcNode *firstArc; // the first arc
} vNode, adjList[MAX_VERTEX_NUM];


typedef struct
{
    adjList vexs;
    int vexNum;     // the number of verteces
    int arcNum;     // the number of arcs
    graphKind kind; // graph type
} aLGraph;
*/
status createGraphAL(aLGraph *graph)
{
    status infoInput;

    graph->kind = directGraph;
    // scanf("%d,%d", &graph->kind, &infoInput);
    switch (graph->kind)
    {
    case directGraph:
        return createDGAL(graph, infoInput);
    // case directNet:
    //     return createDNAL(graph, infoInput);
    // case undirectGraph:
    //     return createUDGAL(graph, infoInput);
    // case undirectNet:
    //     return createUDNAL(graph, infoInput);
    default:
        return ERROR;
    }
}
status createDGAL(aLGraph *graph, status infoInput)
{
    // int incInfo;
    int i, j, k, adjacent;
    vexType vexFrom, vexTo;
    char c;

    std::cin >> graph->vexNum;
    // std::cin >> graph->vexNum >> graph->arcNum;

    for (i = 0; i <= graph->vexNum; ++i) // less than or equal
    {
        graph->vexs[i].data = i;
        graph->vexs[i].firstArc = NULL;
    }

    getchar();

    if ((c = getchar()) == '\n')
        return OK;
    ungetc(c, stdin);

    do
    {
        scanf("%d-%d%c", &vexFrom, &vexTo, &c);
        insertArcAL(graph, vexFrom, vexTo, 1);
    } while (c == ',');

    return OK;
}

// status createDNAL(aLGraph *graph, status infoInput);
// status createUDGAL(aLGraph *graph, status infoInput);
// status createUDNAL(aLGraph *graph, status infoInput);
status destoryGraphAL(aLGraph *graph)
{
    if (clearGraphAL(graph) != OK)
        return ERROR;
    free(graph);

    return OK;
}

// clear
status clearGraphAL(aLGraph *graph)
{
    int i;
    vNode *vexTemp;
    arcNode *arcTemp;

    for (i = 0; i < graph->vexNum; ++i)
    {
        vexTemp = &(graph->vexs[i]);
        while ((arcTemp = vexTemp->firstArc) != NULL)
        {
            vexTemp->firstArc = arcTemp->nextArc;
            free(arcTemp);
        }
    }
    return OK;
}

int locateVexAL(aLGraph *graph, vexType vertex)
{
    int pos;

    if (graph == NULL)
        return -1;
    for (pos = 0; pos < graph->vexNum; ++pos)
        if (graph->vexs[pos].data == vertex)
            return pos;
    return -1;
}

status getVexAL(aLGraph *graph, int vexPos, vexType *value)
{
    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    *value = graph->vexs[vexPos].data;
    return OK;
}

status putVexAL(aLGraph *graph, int vexPos, vexType value)
{
    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    graph->vexs[vexPos].data = value;
    return OK;
}

status firstAdjVexAL(aLGraph *graph, int vexPos, arcNode **pFirstArc)
{
    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    *pFirstArc = graph->vexs[vexPos].firstArc;
    return OK;
}

status nextAdjVexAL(aLGraph *graph, arcNode firstAdjPos, arcNode **pNextArc)
{
    if (graph == NULL)
        return ERROR;
    *pNextArc = firstAdjPos.nextArc;
    return OK;
}

status insertVexAL(aLGraph *graph, vexType vertex)
{
    if (graph == NULL)
        return ERROR;
    if (graph->vexNum == MAX_VERTEX_NUM)
        return OVERFLOW_GRAPH;

    graph->vexs[graph->vexNum].data = vertex;
    graph->vexs[graph->vexNum].firstArc = NULL;
    ++graph->vexNum;

    return OK;
}

status deleteVexAL(aLGraph *graph, vexType vertex)
{
    int pos;
    vNode *vexTemp;
    arcNode *arcTemp;

    if (graph == NULL || graph->vexNum == 0)
        return ERROR;
    for (pos = 0; pos < graph->vexNum; ++pos) // check whther the vertex already exists
        if (graph->vexs[pos].data == vertex)
            break;
    if (pos == graph->vexNum) // if it does not exist
        return ERROR;

    // delete one node. note the affect of the end condition of "while" loop below
    --graph->vexNum;
    vexTemp = &(graph->vexs[pos]);
    while ((arcTemp = vexTemp->firstArc) != NULL)
    {
        vexTemp->firstArc = arcTemp->nextArc;
        free(arcTemp);
    }

    while (pos < graph->vexNum) // delete vertex
    {
        graph->vexs[pos] = graph->vexs[pos + 1];
        ++pos;
    }
    return OK;
}

status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight)
{
    int posFrom = vexFrom, posTo = vexTo;
    arcNode *insertPos, *newArc;

    if (graph->vexs[posFrom].data != posFrom)
    {
        graph->vexs[posFrom].data = posFrom;
        graph->vexs[posFrom].firstArc = NULL;
    }
    if (graph->vexs[posTo].data != posTo)
    {
        graph->vexs[posTo].data = posTo;
        graph->vexs[posTo].firstArc = NULL;
    }

    newArc = (arcNode *)malloc(sizeof(arcNode));
    newArc->adjVex = vexTo;
    newArc->info = weight;

    // we require lager to-node is put near the vertex node
    if ((insertPos = graph->vexs[posFrom].firstArc) == NULL)
    {
        newArc->nextArc = NULL;
        graph->vexs[posFrom].firstArc = newArc;
    }
    else if (insertPos->adjVex == posTo)
        return INFEASIBLE;
    else if (insertPos->adjVex < posTo)
    {
        newArc->nextArc = insertPos;
        graph->vexs[posFrom].firstArc = newArc;
    }
    else
    {
        while (insertPos->nextArc != NULL && insertPos->nextArc->adjVex > posTo)
            insertPos = insertPos->nextArc;
        if (insertPos->nextArc == NULL)
        {
            newArc->nextArc = NULL;
            insertPos->nextArc = newArc;
        }
        else if (insertPos->nextArc->adjVex == posTo)
            return INFEASIBLE;
        else
        {
            newArc->nextArc = insertPos->nextArc;
            insertPos->nextArc = newArc;
        }
    }

    return OK;
}

status deleteArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo)
{
    int posFrom, posTo;
    vNode *vexTemp;
    arcNode *insertPos, *deletePos;

    if ((posFrom = locateVexAL(graph, vexFrom)) == -1)
        return ERROR;
    if ((posTo = locateVexAL(graph, vexTo)) == -1)
        return ERROR;

    // we require lager to-node is put near the vertex node
    vexTemp = &(graph->vexs[posFrom]);
    if ((insertPos = vexTemp->firstArc) == NULL || insertPos->adjVex < posTo)
        return ERROR;
    else if (insertPos->adjVex == posTo)
    {
        vexTemp->firstArc = insertPos->nextArc;
        free(insertPos);
    }
    else
    {
        while (insertPos->nextArc != NULL && insertPos->nextArc->adjVex > posTo)
            insertPos = insertPos->nextArc;
        if (insertPos->nextArc == NULL || insertPos->nextArc->adjVex < posTo)
            return ERROR;
        else
        {
            deletePos = insertPos->nextArc;
            insertPos->nextArc = deletePos->nextArc;
            free(deletePos);
        }
    }

    return OK;
}

status DFSTraverseAL(aLGraph *graph, status (*visit)(vexType vertex))
{
    sqStack *vexStack;
    status vexVisited[MAX_VERTEX_NUM];
    int i, pos, sta;
    arcNode *arcPass;

    if (graph == NULL)
        return ERROR;
    if (graph->vexNum == 0)
        return OK;

    vexStack = stackAlloc();
    initStack_Sq(vexStack);

    for (i = 0; i < graph->vexNum; ++i)
        vexVisited[i] = FALSE;

    while (pos = (allVisited(vexVisited, graph->vexNum)) != -1)
    {
        push_Sq(vexStack, pos);
        while (stackEmpty_Sq(vexStack) == FALSE)
        {
            pop_Sq(vexStack, &pos);      // get the vertex
            if (vexVisited[pos] == TRUE) // if the vertex has been visited, continue
                continue;
            vexVisited[pos] = TRUE;       // set the vertex as having visited
            visit(graph->vexs[pos].data); // visit the vertex

            arcPass = graph->vexs[pos].firstArc;
            while (arcPass != NULL)
            {
                push_Sq(vexStack, arcPass->adjVex);
                arcPass = arcPass->nextArc;
            }
        }
    }
    destroyStack_Sq(vexStack);
    free(vexStack);

    return OK;
}

status BFSTraverseAL(aLGraph *graph, status (*visit)(vexType vertex))
{
    linkedQueue *vexQueue;
    status vexVisited[MAX_VERTEX_NUM];
    int i, pos, sta;
    arcNode *arcPass;

    if (graph == NULL)
        return ERROR;
    if (graph->vexNum == 0)
        return OK;

    vexQueue = queueAlloc();
    initQueue(vexQueue);

    for (i = 0; i < graph->vexNum; ++i)
        vexVisited[i] = FALSE;

    while (pos = (allVisited(vexVisited, graph->vexNum)) != -1)
    {
        enqueue(vexQueue, pos);
        vexVisited[pos] = TRUE;
        while (queueEmpty(vexQueue) == FALSE)
        {
            dequeue(vexQueue, &pos);      // get the vertex
            visit(graph->vexs[pos].data); // visit the vertex

            arcPass = graph->vexs[pos].firstArc;
            while (arcPass != NULL)
            {
                vexVisited[arcPass->adjVex] = TRUE;       // set the vertex as having visited
                enqueue(vexQueue, arcPass->adjVex);
                arcPass = arcPass->nextArc;
            }
        }
    }
    destoryQueue(vexQueue);
    free(vexQueue);

    return OK;
}

int allVisited(status vexVisited[], int length)
{
    int i;

    for (i = 0; i < length; ++i)
        if (vexVisited[i] == FALSE)
            return i;
    return -1;
}