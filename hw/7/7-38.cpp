// 7.38-有向无环图转逆波兰式
// Description
// 一个四则运算算术表达式以有向无环图的邻接表方式存储，每个操作数原子都由单个字母表示。写一个算法输出其逆波兰表达式。

// Input
// 输入第一行为图的顶点个数n
// 第二行开始的n行输入每个节点，第一个元素为节点的字符，第二个元素开始为对应邻接表的节点

// Output
// 输出逆波兰式

// Sample Input 1   // Sample Output 1
// 3                // ab+
// + 1 2
// b
// a
// Sample Input 2   // Sample Output 2
// 5                // ab+c*
// * 2 1
// + 4 3
// c
// a
// b
// Sample Input 3   // Sample Output 3
// 7                // ab*cd+-
// - 2 1
// * 6 5
// + 4 3
// c
// d
// a
// b

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
typedef char vexType;

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
    int i, j, k, adjancent;
    vexType vexFrom, vexTo;
    char c;

    std::cin >> graph->vexNum;

    for (i = 0; i <= graph->vexNum; ++i) // less than or equal
    {
        // graph->vexs[i].data = i;
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

void print_boolean(aLGraph *graph, int root)
{
    int i;
    arcNode *arcTemp;

    for (arcTemp = graph->vexs[root].firstArc; arcTemp; arcTemp = arcTemp->nextArc)
        print_boolean(graph, arcTemp->adjVex);
    putchar(graph->vexs[root].data);
}

// write a program, which can help me find if there is any loop in an directed graph
int main()
{
    aLGraph graph;
    int i, temp1, temp2;
    char c, temp3;

    createGraphAL(&graph);
    getchar();

    for (i = 0; i < graph.vexNum; ++i)
    {
        scanf("%c%c", &temp3, &c);
        graph.vexs[i].data = temp3;
        if (c == '\n' || c == EOF)
            continue;

        scanf("%d %d%c", &temp1, &temp2, &c);
        // first output is temp2
        graph.vexs[i].firstArc = (arcNode *)malloc(sizeof(arcNode));
        graph.vexs[i].firstArc->adjVex = temp2;
        graph.vexs[i].firstArc->nextArc = (arcNode *)malloc(sizeof(arcNode));
        graph.vexs[i].firstArc->nextArc->adjVex = temp1;
        graph.vexs[i].firstArc->nextArc->nextArc = NULL;
    }

    print_boolean(&graph, 0);
    putchar('\n');


    return 0;
}
