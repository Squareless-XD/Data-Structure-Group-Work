// Description
// 试编写一个算法将ｍ维数组表示的ｍ元多项式以常规表示的形式(按降幂顺序)输出。可将其中一项印成 (不好打字, 不写了)
// 提示: 本题假设输入的数组的维数为二维，且以x,y顺次为主序，即本题的多项式形式为二元多次。当系数为零，这一项是不需要打印出来的，每一项之间都通过"+"相连，具体以样例为准。

// Input
// 输入为两部分:
// 第一部分在第一行，输入二维数组的行数与列数，其中行数代表x的指数，列数则比代表ｙ的指数
// 第二部分是从第二行起逐行读入二维数组中各个元素的值，代表各个项前的系数

// Output
// 输出：将二元多次多项式按样例显示的形式输出

// Sample Input 1
// 3 3
// 2 0 4
// -1 3 0
// 1 0 0
// Sample Output 1
// xE2+3xE1yE1+-1xE1+4yE2+2

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
    int dim;             // 数组维数
    int *bounds;         // 数组维界基址，由InitArray分配
    int *constants;      // 数组映像函数常量基址，由InitArray分配
} seqArray;

status arrayInit(seqArray *array, int dim, ...);
status arrayDestroy(seqArray *array);
status arrayLocate(seqArray array, va_list ap, int *off);
status arrayValue(seqArray array, arrayElemType *e, ...);
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
    seqArray polyMat;
    int row, col, rowPos, colPos, firstTermSign;
    arrayElemType value;

    cin >> row >> col;
    arrayInput(&polyMat, row, col);

    firstTermSign = TRUE;

    for (rowPos = row - 1; rowPos >= 0; --rowPos)
    {
        for (colPos = col - 1; colPos >= 0; --colPos)
        {
            arrayValue(polyMat, &value, rowPos, colPos);
            if (value != 0)
            {
                if (firstTermSign == TRUE)
                    firstTermSign = FALSE;
                else
                    cout << '+';
                if (value != 1 || rowPos == 0 && colPos == 0)
                    cout << value;
                if (rowPos != 0)
                    cout << "xE" << rowPos;
                if (colPos != 0)
                    cout << "yE" << colPos;
            }
        }
    }

    arrayDestroy(&polyMat);

    return 0;
}
