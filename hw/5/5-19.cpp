// Description
// 若矩阵A(m*n)中的某个元素a是第i行中的最小值，同时又是第j列中的最大值，则称此元素为该矩阵中的一个马鞍点。
// 假设以二维数组存储矩阵Ａ(m*n)，试编写求出矩阵中所有马鞍点的算法，并分析你的算法在最坏情况下的时间复杂度。

// Input
// 输入分为两部分：
// 第一部分位于第一行，输入二维数组的行数与列数（中间由空格间隔），
// 从第二行开始作为第二部分，表示为整数二维数组的读入。

// Output
// 输出所有在二维数组中表示马鞍点的元素的值，中间由空格隔开

// Sample Input 1
// 3 4
// 10 16 13 14
// 15 18 15 20
// 5 8 12 32

// Sample Output 1
// 15 15

#include <bits/stdc++.h>
using namespace std;

#define maxArrayDim 8 // the maximun dimensions of the array

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_ARRAY -2
#define UNDERFLOW_ARRAY -3

typedef int arrayElemType;
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

status initQueue(linkedQueue *queue);
status destoryQueue(linkedQueue *queue);
status clearQueue(linkedQueue *queue);
status queueEmpty(linkedQueue *queue);
status enqueue(linkedQueue *queue, queueElemType elem);
status dequeue(linkedQueue *queue, queueElemType *elem);

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
    if (queue == NULL)
        return ERROR;
    if (queue->front == NULL)
    {
        queue->rear = NULL;
        return OK;
    }
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

    if (queue == NULL || queue->front == NULL)
        return ERROR;
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
    if (!queue || queue == NULL || queue->front != queue->rear)
        return FALSE;
    return TRUE;
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
    if (queue->front == NULL)
        return ERROR;
    else
    {
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    
    return OK;
}

// take out the oldest element from the queue
status dequeue(linkedQueue *queue, queueElemType *elem)
{
    queueNode *frontTemp;

    if (queue->front == queue->rear || queue->front == NULL)
        return ERROR;
    frontTemp = queue->front->next; // since this node contains the first element that is ever stored
    *elem = frontTemp->data;
    queue->front->next = frontTemp->next;
    if (queue->rear == frontTemp)
        queue->rear = queue->front;
    free(frontTemp);
    return OK;
}

typedef struct _seq_array
{
    arrayElemType *base; // 数组的基地址，由InitArray分配
    int dim;             // 数组维数
    int *bounds;         // 数组维界基址，由InitArray分配
    int *constants;      // 数组映像函数常量基址，由InitArray分配
} seqArray;

status arrayInit(seqArray *array, int dim, ...);
status arrayDestroy(seqArray *array);
status arrayFillZero(seqArray *array);
status arrayLocate(seqArray array, va_list ap, int *off);
status arrayValue(seqArray array, arrayElemType *e, ...);
status arrayAssign(seqArray *array, arrayElemType e, ...);
status arrayInput(seqArray *array);

status arrayInit(seqArray *array, int dim, ...)
{
    int elemtotal, i;
    va_list ap;

    if (dim < 1 || dim > maxArrayDim)
        return ERROR;
    array->dim = dim;
    // get "elem length" of each dimension
    array->bounds = (int *)malloc(dim * sizeof(int));
    if (!array->bounds)
        return OVERFLOW_ARRAY;

    elemtotal = 1;
    va_start(ap, dim);
    for (i = 0; i < dim; ++i)
    {
        array->bounds[i] = va_arg(ap, int);
        if (array->bounds[i] < 0)
            return UNDERFLOW_ARRAY;
        elemtotal *= array->bounds[i];
    }
    va_end(ap);

    // how many memory should we assign to this array
    array->base = (arrayElemType *)malloc(elemtotal * sizeof(arrayElemType));
    if (!array->base)
        return OVERFLOW_ARRAY;

    // when one axis is changed, how much offset should we give
    array->constants = (int *)malloc(dim * sizeof(int));
    if (!array->constants)
        return OVERFLOW_ARRAY;
    array->constants[dim - 1] = 1;
    for (i = dim - 2; i >= 0; --i)
        array->constants[i] = array->bounds[i + 1] * array->constants[i + 1];

    return OK;
}

status arrayDestroy(seqArray *array)
{
    if (array->base != NULL)
        free(array->base);
    if (array->bounds != NULL)
        free(array->bounds);
    if (array->constants != NULL)
        free(array->constants);

    array->base = array->bounds = array->constants = NULL;

    return OK;
}

status arrayFillZero(seqArray *array)
{
    int i;

    for (i = array->constants[0] * array->bounds[0] - 1; i >= 0; --i)
        *(array->base + i) = 0;

    return OK;
}

// return the offset of one element if "ap" is legal under "array"
status arrayLocate(seqArray array, va_list ap, int *off)
{
    int i, ind;

    *off = 0;
    for (i = 0; i < array.dim; ++i)
    {
        ind = va_arg(ap, int);
        // cout << ind << endl;
        if (ind < 0 || ind >= array.bounds[i])
            return OVERFLOW_ARRAY;
        *off += array.constants[i] * ind;
    }

    return OK;
}

status arrayValue(seqArray array, arrayElemType *e, ...)
{
    va_list ap;
    status result;
    int off;

    va_start(ap, e);
    if ((result = arrayLocate(array, ap, &off)) != OK)
    {
        va_end(ap);
        return result;
    }
    *e = *(array.base + off);

    va_end(ap);
    return OK;
}

status arrayAssign(seqArray *array, arrayElemType e, ...)
{
    va_list ap;
    status result;
    int off;

    va_start(ap, e);
    if ((result = arrayLocate(*array, ap, &off)) != OK)
        return result;
    *(array->base + off) = e;
    return OK;
}

status arrayInput(seqArray *array, int row, int col)
{
    int i;
    arrayElemType temp;

    if (arrayInit(array, 2, row, col) != OK)
        return ERROR;

    for (i = 0; i < array->constants[0] * array->bounds[0]; ++i)
    {
        cin >> temp;
        *(array->base + i) = temp;
    }

    return OK;
}

int main()
{
    seqArray array, rowMin, colMax;
    int row, col, i, j, minPos, maxPos;
    arrayElemType maxTemp, minTemp, valueTemp, valueTemp2;
    linkedQueue posTemp;
    int judgeFirstPrint;

    cin >> row >> col;
    arrayInput(&array, row, col);
    arrayInit(&rowMin, 2, row, col);
    arrayInit(&colMax, 2, row, col);
    arrayFillZero(&rowMin);
    arrayFillZero(&colMax);
    initQueue(&posTemp);

    // min in each row
    for (i = 0; i < row; ++i)
    {
        arrayValue(array, &valueTemp, i, 0);
        minTemp = valueTemp;
        enqueue(&posTemp, 0);
        for (j = 1; j < col; ++j)
        {
            arrayValue(array, &valueTemp, i, j);

            if (valueTemp < minTemp)
            {
                minTemp = valueTemp;
                clearQueue(&posTemp);
                enqueue(&posTemp, j);
            }
            else if (valueTemp == minTemp)
                enqueue(&posTemp, j);
        }
        // clear, and store all the row-min in a matrix
        while (queueEmpty(&posTemp) == FALSE)
        {
            dequeue(&posTemp, &valueTemp);
            arrayAssign(&rowMin, 1, i, valueTemp);
        }
    }

    // max in each column
    for (j = 0; j < col; ++j)
    {
        arrayValue(array, &valueTemp, 0, j);
        maxTemp = valueTemp;
        enqueue(&posTemp, 0);
        for (i = 1; i < row; ++i)
        {
            arrayValue(array, &valueTemp, i, j);

            if (valueTemp > maxTemp)
            {
                maxTemp = valueTemp;
                clearQueue(&posTemp);
                enqueue(&posTemp, i);
            }
            else if (valueTemp == maxTemp)
                enqueue(&posTemp, i);
        }
        while (queueEmpty(&posTemp) == FALSE)
        {
            dequeue(&posTemp, &valueTemp);
            arrayAssign(&colMax, 1, valueTemp, j);
        }
    }

    destoryQueue(&posTemp);

    judgeFirstPrint = TRUE;

    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < col; ++j)
        {
            arrayValue(rowMin, &valueTemp, i, j);
            arrayValue(colMax, &valueTemp2, i, j);
            if (valueTemp == 1 && valueTemp2 == 1)
            {
                if (judgeFirstPrint == TRUE)
                {
                    judgeFirstPrint = FALSE;
                    arrayValue(array, &valueTemp, i, j);
                    cout << valueTemp;
                }
                else
                {
                    arrayValue(array, &valueTemp, i, j);
                    cout << ' ' << valueTemp;
                }
            }
        }
    }
    cout << endl;

    arrayDestroy(&array);
    arrayDestroy(&rowMin);
    arrayDestroy(&colMax);

    return OK;
}