#include <bits/stdc++.h>

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

status createDGAL(aLGraph *graph, status infoInput);
status destoryGraphAL(aLGraph *graph);
status clearGraphAL(aLGraph *graph);
status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight);

status createDGAL(aLGraph *graph, status infoInput)
{
    // int incInfo;
    int i;
    vexType vexFrom, vexTo;
    char c;

    std::cin >> graph->vexNum >> c >> graph->arcNum;

    if (graph->vexNum >= MAX_VERTEX_NUM)
        return ERROR;

    for (i = 0; i <= graph->vexNum; ++i)
    {
        graph->vexs[i].data = i;
        graph->vexs[i].firstArc = NULL;
    }

    graph->vexs[0].data = FALSE;
    for (i = 0; i < graph->arcNum; ++i)
    {
        scanf("%d-%d%c", &vexFrom, &vexTo, &c);
        if (vexFrom == 0 || vexTo == 0)
            graph->vexs[0].data = -1;
        insertArcAL(graph, vexFrom, vexTo, 0);
    }

    return OK;
}

status destoryGraphAL(aLGraph *graph)
{
    if (clearGraphAL(graph) != OK)
        return ERROR;
    free(graph);

    return OK;
}

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

status insertArcAL(aLGraph *graph, vexType vexFrom, vexType vexTo, int weight)
{
    int posFrom, posTo;
    vNode *vexTemp;
    arcNode *insertPos, *newArc;

    posFrom = vexFrom;
    posTo = vexTo;

    newArc = (arcNode *)malloc(sizeof(arcNode));
    newArc->adjVex = vexTo;
    newArc->info = weight;

    // we require lager to-node is put near the vertex node
    vexTemp = &(graph->vexs[posFrom]);
    if ((insertPos = vexTemp->firstArc) == NULL)
    {
        newArc->nextArc = NULL;
        vexTemp->firstArc = newArc;
    }
    else if (insertPos->adjVex == posTo)
    {
        free(newArc);
        return INFEASIBLE;
    }
    else if (insertPos->adjVex < posTo)
    {
        newArc->nextArc = insertPos;
        vexTemp->firstArc = newArc;
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
        {
            free(newArc);
            return INFEASIBLE;
        }
        else
        {
            newArc->nextArc = insertPos->nextArc;
            insertPos->nextArc = newArc;
        }
    }

    return OK;
}

int main()
{
    int vexNumber, arcNumber, i;
    status startIndex;
    aLGraph *graph;
    arcNode *arcTemp;

    graph = (aLGraph *)malloc(sizeof(aLGraph));
    graph->kind = directGraph;
    createDGAL(graph, 0);

    if (graph->vexs[0].data == -1)
        startIndex = 0;
    else
        startIndex = 1;
    for (i = startIndex; i < graph->vexNum + startIndex; ++i)
    {
        std::cout << i;
        if ((arcTemp = graph->vexs[i].firstArc) != NULL)
        {
            std::cout << ' ' << arcTemp->adjVex;
            arcTemp = arcTemp->nextArc;
            while (arcTemp != NULL)
            {
                std::cout << ',' << arcTemp->adjVex;
                arcTemp = arcTemp->nextArc;
            }
        }
        std::cout << std::endl;
    }

    // destoryGraphAL(&graph);

    return 0;
}