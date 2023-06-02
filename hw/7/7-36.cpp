// 7.36-求最长路径长度
// Description
// 在图的邻接表存储结构中，为每个顶点增加一个MPL域。试写一算法，求有向无环图G的每个顶点出发的最长路径的长度。

// Input
// 第一行为顶点个数
// 第二行为图中的边，注意图为有向图，左为起点，右为终点

// Output
// 按照顶点序号从小到大输出每个顶点的最长路径长度

// Sample Input 1           // Sample Output 1
// 6                        // 3,2,1,0,3,1
// 1-2,2-3,3-4,5-2,5-6,6-4

// Sample Input 2           // Sample Output 2
// 4                        // 3,2,1,0
// 1-2,2-3,2-4,3-4,1-3

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

status vexVisited[MAX_VERTEX_NUM];

status clear_visit()
{
    int i;
    for (i = 0; i < MAX_VERTEX_NUM; ++i)
        vexVisited[i] = FALSE;
    return TRUE;
}

// find a path in a directed graph with its length equals to k
status findPath_fix(aLGraph *graph, int pathBegin, int pathEnd, int k)
{
    arcNode *i;
    arcNode *arcPass;

    // when it is impossible to has a path between pathBegin and pathEnd
    if (graph == NULL)
        return FALSE;
    if (graph->vexNum == 0)
        return FALSE;
    if (k < 0)
        return FALSE;
    if (k == 0)
    {
        if (pathBegin == pathEnd)
            return TRUE;
        return FALSE;
    }
    if (pathBegin == pathEnd)
        return FALSE;

    vexVisited[pathBegin] = TRUE;
    for (i = graph->vexs[pathBegin].firstArc; i; i = i->nextArc)
    {
        if (vexVisited[i->adjVex] == FALSE || k == 1)
            if (findPath_fix(graph, i->adjVex, pathEnd, k - 1) == TRUE)
                return TRUE;
    }
    vexVisited[pathBegin] = FALSE;

    return FALSE;
}

// write a program, which can help me find if there is any loop in an directed graph
int main()
{
    aLGraph graph;
    int temp1, temp2, begin, end, len, maxlen;
    char c;

    createGraphAL(&graph);

    while (1)
    {
        scanf("%d-%d%c", &temp1, &temp2, &c);
        insertArcAL(&graph, temp1, temp2, 1);
        if (c == '\n' || c == EOF)
            break;
    }

    for (begin = 1; begin <= graph.vexNum; ++begin) // for certain begin point
    {
        maxlen = 0;
        for (len = graph.vexNum; len >= 1; --len) // for certain length
        {
            for (end = 1; end <= graph.vexNum; ++end) // for certain end point
                if (end != begin && clear_visit() && findPath_fix(&graph, begin, end, len) == TRUE) // if there is a path with certain length
                {
                    maxlen = len;
                    break;
                }
            if (maxlen != 0)
                break;
        }
        printf("%d", maxlen);
        if (begin != graph.vexNum)
            printf(",");
    }



    return 0;
}
