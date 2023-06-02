
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
typedef int status;

typedef struct _seq_array
{
    arrayElemType *base; // 数组的基地址，由InitArray分配
    int dim;        // 数组维数
    int *bounds;    // 数组维界基址，由InitArray分配
    int *constants; // 数组映像函数常量基址，由InitArray分配
} seqArray;

status arrayInit(seqArray *array, int dim, ...);
status arrayDestroy(seqArray *array);
status arrayFillZero(seqArray *array);
status arrayLocate(seqArray array, va_list ap, int *off);
status arrayValue(seqArray array, arrayElemType *e, ...);
status arrayAssign(seqArray *array, arrayElemType e, ...);
status arrayInput(seqArray *array);
status arrayPrint_2Dim(seqArray *array);

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

status arrayPrint_2Dim(seqArray *array)
{
    if (array->dim != 2)
        return ERROR;

    int row = array->bounds[0], col = array->bounds[1], i, j;
    arrayElemType *elem = array->base;

    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < col - 1; ++j)
            cout << *(elem++) << ' ';
        cout << *(elem++) << endl;
    }

    return OK;
}


