// 3.20-图像区域颜色置换
// Description
// 假设以二维数组g(1..m, 1..n)表示一个图像区域, g[i, j]表示该区域中点(i, j)所具颜色，其值为从0到k的整数。
// 编写算法置换点(i_, j_)所在区域的颜色。约定和(i_, j_)同色的上、下、左、右的邻接点为同色区域的点。

// Input
// 输入分为两部分，第一部分为第一行输入二维数组的行数、列数、染色起点位置的行坐标与列坐标以及染色开始方向
// 中间以空格隔开，(其中，行坐标与列坐标均以０开始，染色方向为上下左右，为枚举结构(0,1,2,3))
// 第二部分为二维数组形状输入，开始图像着色，用数字２来替换二维数组中１的位置。

// Output
// 着色完成的图像(即，二维数组)打印输出

// Sample Input 1
// 10 17 9 8 3
// 0 0 1 1 1 0 0 0 0 0 0 0 1 1 1 0 0
// 0 1 1 1 1 1 1 0 0 0 1 1 1 1 1 1 0
// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
// 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1
// 0 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 0
// 0 0 0 1 1 1 1 1 1 1 1 1 1 1 0 0 0
// 0 0 0 0 1 1 1 1 1 1 1 1 1 0 0 0 0
// 0 0 0 0 0 0 1 1 1 1 1 0 0 0 0 0 0
// 0 0 0 0 0 0 0 1 1 1 0 0 0 0 0 0 0
// 0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 0
// Sample Output 1
// 00222000000022200
// 02222220002222220
// 22222222222222222
// 22222222222222222
// 02222222222222220
// 00022222222222000
// 00002222222220000
// 00000022222000000
// 00000002220000000
// 00000000200000000

#include <bits/stdc++.h>
using namespace std;

#define queueElemType position

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW_QUEUE -2

typedef int status;

typedef struct posi
{
    int row;
    int col;
} position;

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
status enqueue(linkedQueue *queue, queueElemType elem);
status dequeue(linkedQueue *queue, queueElemType *elem);
status colorChange(position startPosi, int row, int col, linkedQueue *queue);
status inBound(int rowStart, int colStart, int row, int col);

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

vector<vector<int>> colorMatrix;

int main()
{
    int row, col, rowStart, colStart, direction;
    int i, j;
    linkedQueue *queue;
    position startPosi;

    queue = queueAlloc();
    initQueue(queue);

    cin >> row >> col >> rowStart >> colStart >> direction;
    colorMatrix = vector<vector<int>>(row, vector<int>(col, 0));

    for (i = 0; i < row; ++i)
        for (j = 0; j < col; ++j)
            cin >> colorMatrix[i][j];

    startPosi.row = rowStart;
    startPosi.col = colStart;
    colorChange(startPosi, row, col, queue);

    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < col; ++j)
            cout << colorMatrix[i][j];
        cout << endl;
    }
    return 0;
}

status colorChange(position startPosi, int row, int col, linkedQueue *queue)
{
    int rowOut, colOut;
    position elemOut, elemIn;

    enqueue(queue, startPosi);
    while (dequeue(queue, &elemOut) != ERROR)
    {
        rowOut = elemOut.row;
        colOut = elemOut.col;
        if (inBound(rowOut, colOut, row, col) && colorMatrix[rowOut][colOut] == 1)
        {
            colorMatrix[rowOut][colOut] = 2;
            elemIn.row = rowOut;
            elemIn.col = colOut + 1;
            enqueue(queue, elemIn);

            // elemIn.row = rowOut;
            // elemIn.col = colOut - 1;
            elemIn.col -= 2;
            enqueue(queue, elemIn);

            // elemIn.row = rowOut - 1;
            elemIn.row -= 1;
            // elemIn.col = colOut;
            elemIn.col += 1;
            enqueue(queue, elemIn);

            // elemIn.row = rowOut + 1;
            elemIn.row += 2;
            // elemIn.col = colOut;
            enqueue(queue, elemIn);
        }
    }
    return OK;
}

status inBound(int rowStart, int colStart, int row, int col)
{
    if (rowStart >= 0 && rowStart < row && colStart >= 0 && colStart < col)
        return TRUE;
    return FALSE;
}
