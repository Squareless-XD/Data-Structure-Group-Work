// Description
// 使用十字链表完成
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

// 复杂度: O(N_1 N_2 (m_1+n_1)), N_1, N_2为两个矩阵的元素个数.

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

typedef int matElemType;
typedef int status;

typedef struct _OLNode
{
    int rowIndex, colIndex;       // 非零元的下标
    matElemType elem;             // 结点存储的元素
    struct _OLNode *right, *down; // 非零元所在行表和列表的后继
} OLNode;

typedef struct
{
    OLNode **rowHead, **colHead;    // 行和列链表指针向量
    int rowNum, colNum, nonzeroNum; // 行数、列数和非零元个数
} crossList;

OLNode *sMatNodeAlloc(void);
status sMatrixInit(crossList *mat, int row, int col);
status sMatrixDestroy(crossList *mat);
status sMatrixClear(crossList *mat);
status sMatrixValue(crossList mat, matElemType *elem, int row, int col);
status sMatrixAssign(crossList *mat, matElemType elem, int row, int col);
status sMatrixDelete(crossList *mat, int row, int col);
status sMatrixInput(crossList *mat, int row, int col);
status sMatrixPrintBin(crossList mat);
status sMatrixPrintVec(crossList mat);
status printZero(int count);
status sMatrixAdd(crossList *matResult, crossList mat1, crossList mat2);

OLNode *sMatNodeAlloc(void)
{
    return (OLNode *)malloc(sizeof(OLNode));
}

status sMatrixInit(crossList *mat, int row, int col)
{
    int i;
    matElemType elem;

    if (mat == NULL)
        return ERROR;

    mat->rowNum = row;
    mat->colNum = col;
    mat->nonzeroNum = 0;

    if ((mat->rowHead = (OLNode **)malloc((row + 1) * sizeof(OLNode *))) == NULL)
        return ERROR;
    if ((mat->colHead = (OLNode **)malloc((col + 1) * sizeof(OLNode *))) == NULL)
        return ERROR;

    for (i = 1; i <= row; ++i)
        *(mat->rowHead + i) = NULL;
    for (i = 1; i <= col; ++i)
        (mat->colHead)[i] = NULL;

    return OK;
}

status sMatrixDestroy(crossList *mat)
{
    sMatrixClear(mat);
    if (mat->colHead != NULL)
    {
        free(mat->colHead);
        mat->colHead = NULL;
    }
    if (mat->rowHead != NULL)
    {
        free(mat->rowHead);
        mat->rowHead = NULL;
    }
    mat->rowNum = 0;
    mat->colNum = 0;
    mat->nonzeroNum = 0;

    return OK;
}

status sMatrixClear(crossList *mat)
{
    int i, row = mat->rowNum, col = mat->colNum;
    OLNode *matNode, *matNextNode;

    if (mat->rowHead == NULL || mat->colHead == NULL)
        return ERROR;

    for (i = 1; i <= row; ++i)
    {
        matNode = (mat->rowHead)[i];
        while (matNode != NULL)
        {
            matNextNode = matNode->right;
            free(matNode);
            matNode = matNextNode;
        }
        (mat->rowHead)[i] = NULL;
    }
    for (i = 1; i <= col; ++i)
        (mat->colHead)[i] = NULL;

    mat->nonzeroNum = 0;

    return OK;
}


// get an elem's value. first elem in each row/col is 1, rather than 0
status sMatrixValue(crossList mat, matElemType *elem, int row, int col)
{
    OLNode *matNode;

    matNode = mat.rowHead[row];
    while (matNode != NULL && matNode->colIndex < col)
        matNode = matNode->right;
    if (matNode == NULL || matNode->colIndex > col)
        *elem = 0;
    else
        *elem = matNode->elem;

    return OK;
}

// add a new elem. if elem==0, jump to delete. first elem in each row/col is 1, rather than 0
status sMatrixAssign(crossList *mat, matElemType elem, int row, int col)
{
    OLNode *matNode, *newNode;

    // this function does not consider adding an zero (which actually means deleting a node)
    if (elem == 0)
    {
        sMatrixDelete(mat, row, col);
        return OK;
    }

    // create a new node
    newNode = sMatNodeAlloc();
    if (newNode == NULL)
        return ERROR;
    newNode->rowIndex = row;
    newNode->colIndex = col;
    newNode->elem = elem;

    // consider a row, check whther the corresponding node exists
    matNode = mat->rowHead[row];
    if (matNode == NULL || matNode->colIndex > col)
    {
        newNode->right = matNode;
        mat->rowHead[row] = newNode;
    }
    else if (matNode->colIndex == col)
    {
        matNode->elem = elem;
        free(newNode);
        return OK; // end the function
    }
    else
    {
        while (matNode->right != NULL && matNode->right->colIndex < col)
            matNode = matNode->right;
        if (matNode->right != NULL && matNode->right->colIndex == col)
        {
            matNode->right->elem = elem;
            free(newNode);
            return OK;
        }
        newNode->right = matNode->right;
        matNode->right = newNode;
    }

    // consider a column, check whther the corresponding node exists
    matNode = mat->colHead[col];
    if (matNode == NULL || matNode->rowIndex > row)
    {
        newNode->down = matNode;
        mat->colHead[col] = newNode;
    }
    // else if (matNode->rowIndex == row)
    // {
    //     matNode->elem = elem;
    //     free(newNode);
    //     return OK; // end the function
    // }
    else
    {
        while (matNode->down != NULL && matNode->down->rowIndex < row)
            matNode = matNode->down;
        // if (matNode->down != NULL && matNode->down->rowIndex == row)
        // {
        //     matNode->down->elem = elem;
        //     free(newNode);
        //     return OK;
        // }
        newNode->down = matNode->down;
        matNode->down = newNode;
    }

    // if the function runs to this position, a new non-zero element is added.
    ++(mat->nonzeroNum);

    return OK;
}

// delete a new elem. first elem in each row/col is 1, rather than 0
status sMatrixDelete(crossList *mat, int row, int col)
{
    OLNode *matNode;

    // consider a row, check whther the corresponding node exists
    matNode = mat->rowHead[row];
    if (matNode == NULL || matNode->colIndex > col)
        return OK;
    else if (matNode->colIndex == col)
        // free this node? wait until checking col
        mat->rowHead[row] = matNode->right;
    else
    {
        while (matNode->right != NULL && matNode->right->colIndex < col)
            matNode = matNode->right;
        if (matNode->right == NULL || matNode->right->colIndex > col)
            return OK;
        matNode->right = matNode->right->right;
        // wait to free "matNode"
    }

    // consider a col, check whther the corresponding node exists
    matNode = mat->colHead[col];
    if (matNode == NULL || matNode->rowIndex > row)
        return OK;
    else if (matNode->rowIndex == row)
    {
        free(matNode);
        // free this node? wait until checking row
        mat->colHead[col] = matNode->down;
    }
    else
    {
        while (matNode->down != NULL && matNode->down->rowIndex < row)
            matNode = matNode->down;
        if (matNode->down == NULL || matNode->down->rowIndex > row)
            return OK;
        free(matNode->down);
        matNode->down = matNode->down->down;
        // wait to free "matNode"
    }

    // if the function runs to this position, a existing non-zero element is deleted.
    --(mat->nonzeroNum);

    return OK;
}

status sMatrixInput(crossList *mat, int row, int col)
{
    int i, j, sign, whetherEmpty;
    char c;
    matElemType temp;
    vector<matElemType> matVec;
    vector<matElemType>::iterator it;

    sMatrixInit(mat, row, col);

    temp = 0;
    sign = 1;
    whetherEmpty = TRUE;
    while ((c = getchar()) != EOF)
    {
        // cout << c - '0' << " aha!\n";
        if (isdigit(c))
        {
            whetherEmpty = FALSE;
            temp *= 10;
            temp += (c - '0');
            continue;
        }
        if (c == '-')
        {
            sign = -1;
            continue;
        }
        if (c == '\n' && whetherEmpty == TRUE)
            break;
        // cout << temp * sign << " OK please look at here!\n";
        matVec.push_back(temp * sign);
        temp = 0;
        sign = 1;
        if (c == '\n')
            break;
        // if "c" is ' ', then continue
    }

    it = matVec.begin();
    for (i = 1; i <= row; ++i)
    {
        for (j = 1; j <= col; ++j)
        {
            cin >> temp;
            if (temp == 1)
                sMatrixAssign(mat, *(it++), i, j);
        }
    }

    // ATTENTION: here the function clear up '\n'
    getchar();

    return OK;
}

// print mat
status sMatrixPrintBin(crossList mat)
{
    int row = mat.rowNum, col = mat.colNum, i, colIndexTemp;
    OLNode *matNode;

    for (i = 1; i <= row; ++i)
    {
        matNode = mat.rowHead[i];
        // if (matNode == NULL)
        // {
        //     printZero(col - 1);
        //     cout << '0' << endl;
        //     continue;
        // }
        colIndexTemp = 0;
        while (matNode != NULL)
        {
            printZero(matNode->colIndex - colIndexTemp - 1);
            colIndexTemp = matNode->colIndex;
            if (colIndexTemp != col)
                cout << '1' << ' ';
            else
                cout << '1';
            matNode = matNode->right;
        }
        if (colIndexTemp != col)
        {
            printZero(col - colIndexTemp - 1);
            cout << '0';
        }
        cout << endl;
    }

    return OK;
}

// print mat
status sMatrixPrintVec(crossList mat)
{
    int row = mat.rowNum, col = mat.colNum, i, colIndexTemp, firstPrtSign;
    OLNode *matNode;

    firstPrtSign = TRUE;
    for (i = 1; i <= row; ++i)
    {
        matNode = mat.rowHead[i];
        while (matNode != NULL)
        {
            if (firstPrtSign == FALSE)
                cout << ' ';
            else
                firstPrtSign = FALSE;
            cout << matNode->elem;
            matNode = matNode->right;
        }
    }
    cout << endl;

    return OK;
}

status printZero(int count)
{
    if (count <= 0)
        return OK;
    while (count--)
        cout << "0 ";

    return OK;
}

status sMatrixAdd(crossList *matResult, crossList mat1, crossList mat2)
{
    int row, col, i, j;
    matElemType valueTemp, colTemp1, colTemp2;
    OLNode *matNode1, *matNode2;

    if (mat1.rowNum != mat2.rowNum || mat1.colNum != mat2.colNum)
        return ERROR;

    row = mat1.rowNum;
    col = mat1.colNum;
    sMatrixClear(matResult);

    for (i = 1; i <= row; ++i)
    {
        matNode1 = mat1.rowHead[i];
        matNode2 = mat2.rowHead[i];
        while (matNode1 != NULL || matNode2 != NULL)
        {
            // classify all cases into 3 class, in each situation, add them together

            // matNode2.colIndex is smaller
            if (matNode1 == NULL || matNode2 != NULL && matNode1->colIndex > matNode2->colIndex)
            {
                sMatrixAssign(matResult, matNode2->elem, i, matNode2->colIndex);
                matNode2 = matNode2->right;
            }

            // matNode1.colIndex is smaller
            else if (matNode2 == NULL || matNode1->colIndex < matNode2->colIndex)
            {
                sMatrixAssign(matResult, matNode1->elem, i, matNode1->colIndex);
                matNode1 = matNode1->right;
            }

            // both have element at the same position
            else
            {
                if ((valueTemp = matNode1->elem + matNode2->elem) != 0)
                    sMatrixAssign(matResult, valueTemp, i, matNode1->colIndex);
                matNode1 = matNode1->right;
                matNode2 = matNode2->right;
            }
        }
    }

    return OK;
}

int main()
{
    crossList mat1, mat2, matResult;
    int row, col;

    cin >> row >> col;
    getchar();

    // sMatrixInit(&mat1, row, col);
    // sMatrixInit(&mat2, row, col);
    sMatrixInit(&matResult, row, col);

    sMatrixInput(&mat1, row, col);
    sMatrixInput(&mat2, row, col);

    sMatrixAdd(&matResult, mat1, mat2);

    sMatrixPrintVec(matResult);
    sMatrixPrintBin(matResult);

    return 0;
}

