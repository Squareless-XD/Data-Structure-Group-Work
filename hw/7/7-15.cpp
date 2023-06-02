#include <bits/stdc++.h>

#define MAX_VERTEX_NUM 100

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2
#define OVERFLOW_GRAPH 3

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

status destoryGraphMat(matGraph *graph);
status clearGraphMat(matGraph *graph);
status insertVex(matGraph *graph, vexType vertex);
status deleteVex(matGraph *graph, vexType vertex);
status insertArc(matGraph *graph, vexType vexFrom, vexType vexTo, int weight);
status deleteArc(matGraph *graph, vexType vexFrom, vexType vexTo);

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

// this function is wrong, since I miss understood the meaning of it
status insertVex(matGraph *graph, vexType vertex)
{
    int index, i, j;

    if (graph == NULL)
        return ERROR;
    if (graph->vexNum == MAX_VERTEX_NUM) // when space is not enough
        return OVERFLOW_GRAPH;
    // for (index = 0; index < graph->vexNum; ++index) // check whther the vertex already exists
    //     if (graph->vexs[index] == vertex)
    //         return ERROR;
    if (graph->vexNum < vertex)
        return ERROR;
    
    for (i = graph->vexNum; i > vertex; --i)
        for (j = graph->vexNum; j > vertex; --j)
            graph->arcs[i][j] = graph->arcs[i - 1][j - 1];

    for (i = graph->vexNum; i > vertex; --i)
        for (j = vertex - 1; j >= 0; --j)
            graph->arcs[i][j] = graph->arcs[i - 1][j];

    for (i = vertex - 1; i >= 0; --i)
        for (j = graph->vexNum; j > vertex; --j)
            graph->arcs[i][j] = graph->arcs[i][j - 1];

    // for (i = vertex; i < graph->vexNum; ++i)
    graph->vexs[graph->vexNum] = graph->vexNum; // inserte vertex
    ++graph->vexNum;

    // use the new vexNum to clear the arc matrix
    if (graph->kind == directGraph || graph->kind == undirectGraph)
        for (index = 0; index < graph->vexNum; ++index)
            graph->arcs[vertex][index] = graph->arcs[index][vertex] = FALSE;

    if (graph->kind == directNet || graph->kind == undirectNet)
        for (index = 0; index < graph->vexNum; ++index)
            graph->arcs[vertex][index] = graph->arcs[index][vertex] = INT_MAX;

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
    // while (index1 < graph->vexNum) // delete vertex
    // {
    //     graph->vexs[index1] = graph->vexs[index1 + 1];
    //     ++index1;
    // }

    // calculate the number that we will sub from arcNum
    deleteArcNum = 0;
    for (index1 = 0; index1 < graph->vexNum + 1; ++index1)
    {
        if (graph->kind == directGraph && graph->arcs[index1][pos] == TRUE ||
            graph->kind == directNet && graph->arcs[index1][pos] != INT_MAX)
            ++deleteArcNum;
        if ((graph->kind == directGraph || graph->kind == undirectGraph) && graph->arcs[pos][index1] == TRUE ||
            (graph->kind == directNet || graph->kind == undirectNet) && graph->arcs[pos][index1] != INT_MAX)
            ++deleteArcNum;
    }
    if (graph->kind == directGraph && graph->arcs[pos][pos] == TRUE ||
        graph->kind == directNet && graph->arcs[pos][pos] != INT_MAX)
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
        graph->arcs[vexFrom][vexTo] = TRUE;
    else
        graph->arcs[vexFrom][vexTo] = weight;

    // if symmetric
    if (graph->kind == undirectGraph)
        graph->arcs[vexTo][vexFrom] = TRUE;
    else if (graph->kind == undirectNet)
        graph->arcs[vexTo][vexFrom] = weight;

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
        graph->arcs[vexFrom][vexTo] = FALSE;
    else
        graph->arcs[vexFrom][vexTo] = INT_MAX;

    // if symmetric
    if (graph->kind == undirectGraph)
        graph->arcs[vexTo][vexFrom] = FALSE;
    else if (graph->kind == undirectNet)
        graph->arcs[vexTo][vexFrom] = INT_MAX;

    return OK;
}


int main()
{
    matGraph *graph;
    int i, j;
    char c, op1, op2;
    vexType num1, num2;

    graph = (matGraph *)malloc(sizeof(matGraph));
    std::cin >> graph->vexNum;
    graph->kind = undirectGraph;
    graph->arcNum = 0;

    for (i = 0; i < graph->vexNum; ++i)
    {
        graph->vexs[i] = i;
        for (j = 0; j < graph->vexNum; ++j)
        {
            std::cin >> graph->arcs[i][j];
            if (i <= j && graph->arcs[i][j] == 1)
                ++graph->arcNum;
        }
    }

    while ((c = getchar()) != EOF)
    {
        op1 = getchar();
        if (op1 == EOF)
            break;
        op2 = getchar();
        if (op2 == EOF)
            break;

        if (op1 == 'I' && op2 == 'V')
        {
            std::cin >> num1;
            insertVex(graph, num1);
        }
        else if (op1 == 'I' && op2 == 'A')
        {
            std::cin >> num1 >> num2;
            insertArc(graph, num1, num2, 1);
        }
        else if (op1 == 'D' && op2 == 'V')
        {
            std::cin >> num1;
            deleteVex(graph, num1);
        }
        else if (op1 == 'D' && op2 == 'A')
        {
            std::cin >> num1 >> num2;
            deleteArc(graph, num1, num2);
        }
    }

    for (i = 0; i < graph->vexNum; ++i)
    {
        for (j = 0; j < graph->vexNum - 1; ++j)
            std::cout << graph->arcs[i][j] << ' ';
        std::cout << graph->arcs[i][j] << '\n';
    }

    return 0;
}