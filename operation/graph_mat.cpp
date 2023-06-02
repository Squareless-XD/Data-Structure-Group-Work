#include <bits/stdc++.h>

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量
#define MAX_VERTEX_NUM 100

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2
#define OVERFLOW_GRAPH 3
#define OVERFLOW_STACK 3

typedef int status;
typedef int relaType;
// typedef int infoType;
typedef int vexType;

typedef enum
{
    directGraph,
    directNet,
    undirectGraph,
    undirectNet
} graphKind;

// typedef struct arcCell
// {
//     relaType adj;
//     // infoType *info;
// } arcCell, adjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef relaType adjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

typedef struct
{
    vexType vexs[MAX_VERTEX_NUM];
    adjMatrix arcs;
    int vexNum, arcNum;
    graphKind kind;
} matGraph;

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

status createGraphMat(matGraph *graph);
status createDGMat(matGraph *graph);
status createDNMat(matGraph *graph);
status createUDGMat(matGraph *graph);
status createUDNMat(matGraph *graph);
status destoryGraphMat(matGraph *graph);
status clearGraphMat(matGraph *graph);
int locateVexMat(matGraph *graph, vexType vertex);
status getVex(matGraph *graph, int vexPos, vexType *value);
status putVex(matGraph *graph, int vexPos, vexType value);
int firstAdjVex(matGraph *graph, int vexPos);
int nextAdjVex(matGraph *graph, int vexPos, int firstAdjPos);
status insertVex(matGraph *graph, vexType vertex);
status deleteVex(matGraph *graph, vexType vertex);
status insertArc(matGraph *graph, vexType vexFrom, vexType vexTo, int weight);
status deleteArc(matGraph *graph, vexType vexFrom, vexType vexTo);
status DFSTraverse(matGraph *graph, status (*visit)(vexType vertex));
status BFSTraverse(matGraph *graph, status (*visit)(vexType vertex));

status createGraphMat(matGraph *graph)
{
    scanf("%d", &graph->kind);
    switch (graph->kind)
    {
    case directGraph:
        return createDGMat(graph);
    case directNet:
        return createDNMat(graph);
    case undirectGraph:
        return createUDGMat(graph);
    case undirectNet:
        return createUDNMat(graph);
    default:
        return ERROR;
    }
}

status createDGMat(matGraph *graph)
{
    // int incInfo;
    int i, j, k, adjacent;
    vexType vertex1, vertex2;

    std::cin >> graph->vexNum >> graph->arcNum; // >> incInfo;

    for (i = 0; i < graph->vexNum; ++i)
        std::cin >> graph->vexs[i];

    for (i = 0; i < graph->vexNum; ++i)
        for (j = 0; j < graph->vexNum; ++j)
            graph->arcs[i][j] = 0;
    // graph->arcs[i][j] = {INT_MAX, NULL};

    for (k = 0; k < graph->arcNum; ++k)
    {
        std::cin >> vertex1 >> vertex2 >> adjacent;
        i = locateVexMat(graph, vertex1);
        j = locateVexMat(graph, vertex2);
        graph->arcs[i][j] = adjacent;
        // if (incInfo == 1)
    }
    return OK;
}
status createDNMat(matGraph *graph)
{
    // int incInfo;
    int i, j, k, weight;
    vexType vertex1, vertex2;

    std::cin >> graph->vexNum >> graph->arcNum; // >> incInfo;

    for (i = 0; i < graph->vexNum; ++i)
        std::cin >> graph->vexs[i];

    for (i = 0; i < graph->vexNum; ++i)
        for (j = 0; j < graph->vexNum; ++j)
            graph->arcs[i][j] = INT_MAX;
    // graph->arcs[i][j] = {INT_MAX, NULL};

    for (k = 0; k < graph->arcNum; ++k)
    {
        std::cin >> vertex1 >> vertex2 >> weight;
        i = locateVexMat(graph, vertex1);
        j = locateVexMat(graph, vertex2);
        graph->arcs[i][j] = weight;
        // if (incInfo == 1)
    }
    return OK;
}
status createUDGMat(matGraph *graph)
{
    // int incInfo;
    int i, j, k, adjacent;
    vexType vertex1, vertex2;

    std::cin >> graph->vexNum >> graph->arcNum; // >> incInfo;

    for (i = 0; i < graph->vexNum; ++i)
        std::cin >> graph->vexs[i];

    for (i = 0; i < graph->vexNum; ++i)
        for (j = 0; j < graph->vexNum; ++j)
            graph->arcs[i][j] = 0;
    // graph->arcs[i][j] = {INT_MAX, NULL};

    for (k = 0; k < graph->arcNum; ++k)
    {
        std::cin >> vertex1 >> vertex2 >> adjacent;
        i = locateVexMat(graph, vertex1);
        j = locateVexMat(graph, vertex2);
        graph->arcs[j][i] = graph->arcs[i][j] = adjacent;
        // if (incInfo == 1)
    }
    return OK;
}
status createUDNMat(matGraph *graph)
{
    // int incInfo;
    int i, j, k, weight;
    vexType vertex1, vertex2;

    std::cin >> graph->vexNum >> graph->arcNum; // >> incInfo;

    for (i = 0; i < graph->vexNum; ++i)
        std::cin >> graph->vexs[i];

    for (i = 0; i < graph->vexNum; ++i)
        for (j = 0; j < graph->vexNum; ++j)
            graph->arcs[i][j] = INT_MAX;
    // graph->arcs[i][j] = {INT_MAX, NULL};

    for (k = 0; k < graph->arcNum; ++k)
    {
        std::cin >> vertex1 >> vertex2 >> weight;
        i = locateVexMat(graph, vertex1);
        j = locateVexMat(graph, vertex2);
        graph->arcs[j][i] = graph->arcs[i][j] = weight;
        // if (incInfo == 1)
    }
    return OK;
}

status destoryGraphMat(matGraph *graph);
status clearGraphMat(matGraph *graph);
int locateVexMat(matGraph *graph, vexType vertex)
{
    int pos;

    if (graph == NULL)
        return ERROR;
    for (pos = 0; pos < graph->vexNum; ++pos)
        if (graph->vexs[pos] == vertex)
            return pos;
    return -1;
}

status getVex(matGraph *graph, int vexPos, vexType *value)
{
    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    *value = graph->vexs[vexPos];
    return OK;
}

status putVex(matGraph *graph, int vexPos, vexType value)
{
    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    graph->vexs[vexPos] = value;
    return OK;
}

int firstAdjVex(matGraph *graph, int vexPos)
{
    int adjPos;

    if (graph == NULL || graph->vexNum <= vexPos)
        return ERROR;
    for (adjPos = 0; adjPos < graph->vexNum; ++adjPos) // check all verteces until one adjacent vertex is found
    {
        if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[vexPos][adjPos] == 1 ||
            (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[vexPos][adjPos] != INT_MAX)
            return adjPos;
    }
    return -1;
}

int nextAdjVex(matGraph *graph, int vexPos, int firstAdjPos)
{
    int adjPos;

    if (graph == NULL)
        return ERROR;
    for (adjPos = firstAdjPos + 1; adjPos < graph->vexNum; ++adjPos) // check all verteces until one adjacent vertex is found
    {
        if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[vexPos][adjPos] == 1 ||
            (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[vexPos][adjPos] != INT_MAX)
            return adjPos;
    }
    return -1;
}

status insertVex(matGraph *graph, vexType vertex)
{
    int index;
    
    if (graph == NULL)
        return ERROR;
    if (graph->vexNum == MAX_VERTEX_NUM) // whther the space is enough
        return OVERFLOW_GRAPH;
    for (index = 0; index < graph->vexNum; ++index) // check whther the vertex already exists
        if (graph->vexs[index] == vertex)
            return ERROR;
    graph->vexs[graph->vexNum++] = vertex; // inserte vertex

    // use the new vexNum to clear the arc matrix
    if (graph->kind == directGraph || graph->kind == undirectGraph)
    {
        for (index = 0; index < graph->vexNum - 1; ++index)
        {
            graph->arcs[graph->vexNum - 1][index] = FALSE;
            graph->arcs[index][graph->vexNum - 1] = FALSE;
        }
        graph->arcs[graph->vexNum - 1][graph->vexNum - 1] = FALSE;
    }
    if (graph->kind == directNet || graph->kind == undirectNet)
    {
        for (index = 0; index < graph->vexNum - 1; ++index)
        {
            graph->arcs[graph->vexNum - 1][index] = INT_MAX;
            graph->arcs[index][graph->vexNum - 1] = INT_MAX;
        }
        graph->arcs[graph->vexNum - 1][graph->vexNum - 1] = INT_MAX;
    }

    return OK;
}

status deleteVex(matGraph *graph, vexType vertex)
{
    int pos, index1, index2, deleteArcNum;

    if (graph == NULL || graph->vexNum == 0)
        return ERROR;
    for (pos = 0; pos < graph->vexNum; ++pos) // check whther the vertex already exists
        if (graph->vexs[pos] == vertex)
            break;
    if (pos == graph->vexNum) // if it does not exist
        return ERROR;

    // delete one node. note the affect of the end condition of "while" loop below
    --graph->vexNum;
    index1 = pos;
    while (index1 < graph->vexNum) // delete vertex
    {
        graph->vexs[index1] = graph->vexs[index1 + 1];
        ++index1;
    }

    // calculate the number that we will sub from arcNum
    deleteArcNum = 0;
    for (index1 = 0; index1 < graph->vexNum + 1; ++index1)
    {
        if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[index1][pos] == TRUE ||
            (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[index1][pos] != INT_MAX)
            ++deleteArcNum;
        if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[pos][index1] == TRUE ||
            (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[pos][index1] != INT_MAX)
            ++deleteArcNum;
    }
    if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[pos][pos] == TRUE ||
        (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[pos][pos] != INT_MAX)
        --deleteArcNum;
    graph->arcNum -= deleteArcNum;

    // delete arcs
    for (index1 = pos; index1 < graph->vexNum; ++index1)
        for (index2 = 0; index2 < pos; ++index2)
            graph->arcs[index1][index2] = graph->arcs[index1][index2 + 1];

    for (index1 = 0; index1 < pos; ++index1)
        for (index2 = pos; index2 < graph->vexNum; ++index2)
            graph->arcs[index1][index2] = graph->arcs[index1 + 1][index2];

    for (index1 = pos; index1 < graph->vexNum; ++index1)
        for (index2 = pos; index2 < graph->vexNum; ++index2)
            graph->arcs[index1][index2] = graph->arcs[index1 + 1][index2 + 1];

    return OK;
}

// if this is a graph rather than new, then "weight" is not needed
status insertArc(matGraph *graph, vexType vexFrom, vexType vexTo, int weight)
{
    // if the arc exsists
    if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[vexFrom][vexTo] == TRUE ||
        (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[vexFrom][vexTo] != INT_MAX)
        return ERROR;

    // assign the arc
    ++graph->arcNum;
    if (graph->kind == directGraph || graph->kind == undirectGraph)
        graph->arcs[vexFrom][vexTo] == TRUE;
    else
        graph->arcs[vexFrom][vexTo] == weight;

    // if symmetric
    if (graph->kind == undirectGraph)
        graph->arcs[vexTo][vexFrom] == TRUE;
    else if (graph->kind == undirectNet)
        graph->arcs[vexTo][vexFrom] == weight;

    return OK;
}

status deleteArc(matGraph *graph, vexType vexFrom, vexType vexTo)
{
    // if the arc does not exsist
    if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[vexFrom][vexTo] == FALSE ||
        (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[vexFrom][vexTo] == INT_MAX)
        return ERROR;

    --graph->arcNum;
    if (graph->kind == directGraph || graph->kind == undirectGraph)
        graph->arcs[vexFrom][vexTo] == FALSE;
    else
        graph->arcs[vexFrom][vexTo] == INT_MAX;

    // if symmetric
    if (graph->kind == undirectGraph)
        graph->arcs[vexTo][vexFrom] == FALSE;
    else if (graph->kind == undirectNet)
        graph->arcs[vexTo][vexFrom] == INT_MAX;

    return OK;
}

status DFSTraverse(matGraph *graph, status (*visit)(vexType vertex))
{
    int travTemp[MAX_VERTEX_NUM];
    int i, pos, nextPos;
    sqStack *vexStack;

    if (graph->vexNum == 0) // if empty, return OK
        return OK;

    vexStack = stackAlloc();  // open a stack
    initStack_Sq(vexStack);

    for (i = 0; i < graph->vexNum; ++i) // initialize marking array
        travTemp[i] = FALSE;
    push_Sq(vexStack, 0); // push the first vertex into the stack

    while (stackEmpty_Sq(vexStack) == FALSE)
    {
        pop_Sq(vexStack, &pos);   // get the vertex
        if (travTemp[pos] == TRUE)          // if the vertex has been visited, continue
            continue;
        travTemp[pos] = TRUE;       // set the vertex as having visited
        visit(graph->vexs[pos]);    // visit the vertex

        for (nextPos = 0; nextPos < graph->vexNum; ++nextPos) // pushing new verteces into the stack
            if (travTemp[nextPos] == FALSE &&
                ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[pos][nextPos] == TRUE ||
                 (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[pos][nextPos] != INT_MAX))
                push_Sq(vexStack, nextPos);
    }

    destroyStack_Sq(vexStack);
    free(vexStack);

    return OK;
}

status BFSTraverse(matGraph *graph, status (*visit)(vexType vertex))
{
    int travTemp[MAX_VERTEX_NUM];
    int i, pos, nextPos;
    linkedQueue *vexQueue;

    if (graph->vexNum == 0) // if empty, return OK
        return OK;

    vexQueue = queueAlloc();  // open a stack
    initQueue(vexQueue);

    for (i = 1; i < graph->vexNum; ++i) // initialize marking array
        travTemp[i] = FALSE;
    travTemp[0] = TRUE;
    enqueue(vexQueue, 0); // push the first vertex into the stack

    while (queueEmpty(vexQueue) == FALSE)
    {
        dequeue(vexQueue, &pos);   // get the vertex
        visit(graph->vexs[pos]);    // visit the vertex

        for (nextPos = 0; nextPos < graph->vexNum; ++nextPos) // pushing new verteces into the stack
            if (travTemp[nextPos] == FALSE &&
                ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[pos][nextPos] == TRUE ||
                 (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[pos][nextPos] != INT_MAX))
            {
                enqueue(vexQueue, nextPos);
                travTemp[nextPos] = TRUE;
            }
    }

    destoryQueue(vexQueue);
    free(vexQueue);

    return OK;
}
