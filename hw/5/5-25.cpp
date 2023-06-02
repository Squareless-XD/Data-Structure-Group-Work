// Description
// 若将稀疏矩阵 A 的非零元素以行序为主序的顺序存于一维数组 V 中，并用二维数组 B 表示 A 中的相应元素是否为零元素（以0和1分别表示零元素和非零元素）。
// 试写一算法，实现在上述表示法中实现矩阵相加的运算。并分析你的算法时间复杂度（在代码注释中给出）。

// Input
// 第一行 为用空格隔开的两个个数 m_1, n_1，表示两个矩阵的大小
// 第二行 为第一个矩阵的 V 数组，元素用空格隔开。（如果没有非零元素，则会空出这一行）
// 第3到 m_1+2 行：每行分别包含 n_1 个元素，用空格隔开。表示第一个矩阵的 B 数组。
// 第 m_1+3 行： 第二个矩阵的 V 数组，元素用空格隔开。（如果没有非零元素，则会空出这一行）
// 第 m_1+4到 2×m_1+3 行：第二个矩阵的 B 数组。
// 提示：V 元素可能为负数。

// Output
// 第一行: 和矩阵的 V 数组。如果没有非零元素，则输出空行。
// 第2到 m_1+1 行: 和矩阵的 B 矩阵。
// 提示：每行的最后一个元素之后不要输出多余空格！

// Sample Input 1       Sample Output 1
// 2 2                  -1
// 1 1                  1 0
// 1 0                  0 0
// 0 1
// -2 -1
// 1 0
// 0 1

// 复杂度: O(m_1 n_1)

#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define maxArrayDim 8 // the maximun dimensions of the array
#define MAXSTRLEN 255

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
status arrayFillZero(seqArray *array);
status arrayLocate(seqArray array, va_list ap, int *off);
status arrayValue(seqArray array, arrayElemType *e, ...);
status arrayAssign(seqArray *array, arrayElemType e, ...);
status arrayInput(seqArray *array);
status arrayPrint_2Dim(seqArray *array);
status arrayPrint_2DimSpec(seqArray *arrayBin, string arrayVec);
status arrayAdd_2DimSpe(seqArray arrayBin1, vector<arrayElemType> arrayVec1, seqArray arrayBin2, vector<arrayElemType> arrayVec2, seqArray *arrayBin3, vector<arrayElemType> *arrayVec3);

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

status arrayPrint_2DimSpec(seqArray *arrayBin, vector<arrayElemType> arrayVec)
{
    if (arrayBin->dim != 2)
        return ERROR;

    int row = arrayBin->bounds[0], col = arrayBin->bounds[1], i, j;
    arrayElemType *elem = arrayBin->base;
    vector<arrayElemType>::iterator it = arrayVec.begin();

    for (i = 0; i < row; ++i)
    {
        for (j = 0; j < col - 1; ++j)
        {
            if (*(elem++) == 0)
                cout << "0 ";
            else
                cout << *(it++) << ' ';
            if (it == arrayVec.end())
                return OK;
        }
        if (*(elem++) == 0)
            cout << '0' << endl;
        else
            cout << *(it++) << endl;
        if (it == arrayVec.end())
            return OK;
    }

    return OK;
}

status arrayAdd_2DimSpe(seqArray arrayBin1, vector<arrayElemType> arrayVec1, seqArray arrayBin2, vector<arrayElemType> arrayVec2, seqArray *arrayBin3, vector<arrayElemType> *arrayVec3)
{
    if (arrayBin1.dim != 2 || arrayBin2.dim != 2)
        return ERROR;

    int row = arrayBin1.bounds[0], col = arrayBin1.bounds[1], i, valueTemp;

    if (arrayBin2.bounds[0] != row || arrayBin2.bounds[1] != col || arrayBin3->bounds[0] != row || arrayBin3->bounds[1] != col)
        return ERROR;

    arrayElemType *elem1 = arrayBin1.base, *elem2 = arrayBin2.base, *elem3 = arrayBin3->base;
    vector<arrayElemType>::iterator it1 = arrayVec1.begin(), it2 = arrayVec2.begin();

    for (i = 0; i < row * col; ++i)
    {
        if (*elem1 != 0 && *elem2 != 0)
        {
            // if not equal to 0, then push; if equal to 0, then not push
            if ((valueTemp = *(it1++) + *(it2++)) != 0)
                arrayVec3->push_back(valueTemp);
            *elem3 = (valueTemp != 0) ? 1 : 0;
        }
        else if (*elem1 != 0 && *elem2 == 0)
        {
            *elem3 = 1;
            arrayVec3->push_back(*(it1++));
        }
        else if (*elem1 == 0 && *elem2 != 0)
        {
            *elem3 = 1;
            arrayVec3->push_back(*(it2++));
        }
        else
            *elem3 = 0;
        if (i != row * col - 1)
        {
            ++elem1;
            ++elem2;
            ++elem3;
        }
    }

    return OK;
}

int main()
{
    vector<arrayElemType> matVec1, matVec2, matVec3;
    seqArray matBin1, matBin2, matBin3;
    int row, col, count, sign, i, size, whetherEmpty;
    arrayElemType temp;
    char c;

    cin >> row >> col;

    arrayInit(&matBin3, 2, row, col);

    getchar();
    temp = 0;
    sign = 1;
    whetherEmpty = 0;
    while ((c = getchar()) != EOF)
    {
        // cout << c - '0' << " aha!\n";
        if (isdigit(c))
        {
            whetherEmpty = 1;
            temp *= 10;
            temp += (c - '0');
            continue;
        }
        if (c == '-')
        {
            sign = -1;
            continue;
        }
        if (c == '\n' && whetherEmpty == 0)
            break;
        // cout << temp * sign << " OK please look at here!\n";
        matVec1.push_back(temp * sign);
        temp = 0;
        sign = 1;
        if (c == '\n')
            break;
        // if "c" is ' ', then continue
    }
    arrayInput(&matBin1, row, col);

    temp = 0;
    sign = 1;
    whetherEmpty = 0;
    c = getchar();
    // cout << "This is c: " << c << endl;
    while ((c = getchar()) != EOF)
    {
        if (isdigit(c))
        {
            whetherEmpty = 1;
            temp *= 10;
            temp += (c - '0');
            continue;
        }
        if (c == '-')
        {
            sign = -1;
            continue;
        }
        if (c == '\n' && whetherEmpty == 0)
            break;
        matVec2.push_back(temp * sign);
        temp = 0;
        sign = 1;
        if (c == '\n')
            break;
        // if "c" is ' ', then continue
    }
    arrayInput(&matBin2, row, col);

    arrayAdd_2DimSpe(matBin1, matVec1, matBin2, matVec2, &matBin3, &matVec3);

    size = matVec3.size();
    for (i = 0; i < size - 1; ++i)
        cout << matVec3[i] << ' ';
    if (size > 0)
        cout << matVec3[size - 1];
    cout << endl;

    arrayPrint_2Dim(&matBin3);

    return 0;
}
