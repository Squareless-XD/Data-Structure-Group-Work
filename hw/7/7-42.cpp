// Description
// 以邻接表作为存储结构实现求源点到其余各顶点最短路径的Dijkstra算法

// Input
// 第一行为顶点个数以及源点
// 第二行开始到结束为图中的边，以及边的权重

// Output
// 输出源点到各个顶点的最短距离，按顶点序号从小到大排列

// Sample Input 1   // Sample Output 1
// 4 1              // 1,2,3
// 1-2 1
// 1-4 3
// 2-3 1
// 3-4 2
// 3-1 5
// Sample Input 2   // Sample Output 2
// 6 1              // 7,9,20,16,11
// 1-2 7
// 1-3 9
// 1-6 14
// 3-6 2
// 2-3 10
// 2-4 15
// 3-4 11
// 6-5 9
// 5-4 6
// 3-5 7

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

    for (i = 0; i <= graph->vexNum; ++i)
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

int adjLength(aLGraph *graph, vexType vexFrom, vexType vexTo)
{
    arcNode *arcTemp;
    if (graph->vexs[vexFrom].firstArc == NULL)
        return -1;
    for (arcTemp = graph->vexs[vexFrom].firstArc; arcTemp; arcTemp = arcTemp->nextArc)
    {
        if (arcTemp->adjVex == vexTo)
            return arcTemp->info;
    }

    return -1;
}

void Dijkstra(aLGraph *graph, vexType sourceVex)
{
    std::vector<status> vexVisited;
    std::vector<int> vexPathLen;
    int temp, minLen, minLenVex, i, j;

    vexVisited = std::vector<status>(graph->vexNum + 1, FALSE);
    // initialize vertex distance
    vexPathLen = std::vector<int>(graph->vexNum + 1, INT_MAX);
    vexPathLen[sourceVex] = 0;

    // Dijkstra algorithm
    for (i = 0; i < graph->vexNum; ++i) // while S != V
    {
        // find a minimum L(u)
        minLen = INT_MAX;
        for (j = 1; j <= graph->vexNum; ++j)
        {
            if (vexVisited[j] == TRUE)
                continue;
            if (minLen > vexPathLen[j])
            {
                minLen = vexPathLen[j];
                minLenVex = j;
            }
        }

        // add {u} into S
        vexVisited[minLenVex] = TRUE;

        // refresh all v \notin S for there L(v) 
        for (j = 1; j <= graph->vexNum; ++j)
        {
            if (vexVisited[j] == TRUE)
                continue;
            if ((temp = adjLength(graph, minLenVex, j)) != -1 && minLen + temp < vexPathLen[j])
                vexPathLen[j] = minLen + temp;
        }
    }

    // print out the results
    for (i = 1; i <= graph->vexNum; ++i)
    {
        if (i == sourceVex)
            continue;
        printf("%d", vexPathLen[i]);
        if (i != graph->vexNum)
            putchar(',');
    }
    putchar('\n');
}

// write a program, which can help me find if there is any loop in an directed graph
int main()
{
    aLGraph graph;
    int i, j, temp1, temp2, temp3;
    char c;
    vexType sourceVex;

    createGraphAL(&graph);
    std::cin >> sourceVex;
    getchar();

    while (1)
    {
        if ((c = getchar()) == EOF)
            break;
        ungetc(c, stdin);
        scanf("%d-%d %d%c", &temp1, &temp2, &temp3, &c);
        insertArcAL(&graph, temp1, temp2, temp3);
        if (c == EOF)
            break;
    }

    Dijkstra(&graph, sourceVex);

    return 0;
}
