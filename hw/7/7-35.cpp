#include <bits/stdc++.h>
#include <vector>

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

typedef int stackElemType;

typedef struct
{
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

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
status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight);
status findPath(aLGraph *graph, int pathBegin, int pathEnd);
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

    graph->arcNum = 0;

    // getchar();

    // if ((c = getchar()) == '\n')
    //     return OK;
    // ungetc(c, stdin);

    // do
    // {
    //     scanf("%d-%d%c", &vexFrom, &vexTo, &c);
    //     insertArcAL(graph, vexFrom, vexTo, 1);
    // } while (c == ',');

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

// insert an arc into a given graph
status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight)
{
    int posFrom = vexFrom, posTo = vexTo;
    arcNode *insertPos, *newArc;

    // if (graph->vexs[posFrom].data != posFrom)
    // {
    //     graph->vexs[posFrom].data = posFrom;
    //     graph->vexs[posFrom].firstArc = NULL;
    // }
    // if (graph->vexs[posTo].data != posTo)
    // {
    //     graph->vexs[posTo].data = posTo;
    //     graph->vexs[posTo].firstArc = NULL;
    // }

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

status findPath(aLGraph *graph, int pathBegin, int pathEnd)
{
    sqStack *vexStack;
    status vexVisited[MAX_VERTEX_NUM];
    int i, pos, sta;
    arcNode *arcPass;

    // when it is impossible to has a path between pathBegin and pathEnd
    if (graph == NULL)
        return FALSE;
    if (graph->vexNum == 0)
        return FALSE;

    vexStack = stackAlloc();
    initStack_Sq(vexStack);

    for (i = 0; i < graph->vexNum; ++i)
        vexVisited[i] = FALSE;

    push_Sq(vexStack, pathBegin);
    while (stackEmpty_Sq(vexStack) == FALSE)
    {
        pop_Sq(vexStack, &pos);      // get the vertex
        if (vexVisited[pos] == TRUE) // if the vertex has been visited, continue
            continue;
        vexVisited[pos] = TRUE; // set the vertex as having visited

        // visit(graph->vexs[pos].data); // visit the vertex
        if (pos == pathEnd)
            return TRUE;

        arcPass = graph->vexs[pos].firstArc;
        while (arcPass != NULL)
        {
            push_Sq(vexStack, arcPass->adjVex);
            arcPass = arcPass->nextArc;
        }
    }

    destroyStack_Sq(vexStack);
    free(vexStack);

    return FALSE;
}

// use findPath function, find if a graph has a circuit
status findCircuit(aLGraph *graph, int circuitBegin)
{
    arcNode *arcTemp;

    // when it is impossible to has a path between pathBegin and pathEnd
    if (graph == NULL)
        return FALSE;
    if (graph->vexNum == 0)
        return FALSE;

    arcTemp = graph->vexs[circuitBegin].firstArc; // get the first arc
    if (arcTemp == NULL) // if there is no arc, return FALSE
        return FALSE;
    if (findPath(graph, arcTemp->adjVex, circuitBegin)) // if there is a path from the first arc to the begin vertex, return TRUE
        return TRUE;
    while ((arcTemp = arcTemp->nextArc) != NULL) // if there is no path from one arc to the begin vertex, try the next arc
    {
        if (findPath(graph, arcTemp->adjVex, circuitBegin)) // if there is a path from one arc to the begin vertex, return TRUE
            return TRUE;
    }

    return FALSE;
}


int allVisited(status vexVisited[], int length)
{
    int i;

    for (i = 0; i < length; ++i)
        if (vexVisited[i] == FALSE)
            return i;
    return -1;
}

// write a program, which can help me find if there is any loop in an directed graph
int main()
{
    aLGraph graph;
    int temp, i, j;
    char c;
    std::vector<int> s, fst, lst;

    createGraphAL(&graph);

    while (1)
    {
        scanf("%d%c", &temp, &c);
        s.push_back(temp);
        if (c == '\n' || c == EOF)
            break;
    }
    for (i = 0; i <= graph.vexNum; ++i)
    {
        scanf("%d", &temp);
        fst.push_back(temp);
    }
    for (i = 0; i <= graph.vexNum; ++i)
    {
        scanf("%d", &temp);
        lst.push_back(temp);
    }

    for (i = 1; i <= graph.vexNum; ++i)
        for (j = fst[i]; j < lst[i]; ++j)
            insertArcAL(&graph, i, s[j], 1);

    for (i = 1; i < graph.vexNum; ++i)
        if (findCircuit(&graph, i))
        {
            std::cout << "yes\n";
            return 0;
        }

    std::cout << "no\n";

    return 0;
}
