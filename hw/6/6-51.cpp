// 6.51-二叉树与算术表达式
// Description
// 编写一个算法，输出以二叉树表示的算术表达式，若该表达式中含有括号，则在输出时应添上。

// Input
// 输入对应表达式二叉树的先序遍历序列，其中#表示空节点。（如a##表示a的左右节点都为空，即a为叶子节点）

// Output
// 输出正常的表达式序列（即表达式二叉树的中序遍历序列）

// Sample Input 1       Sample Input 2      Sample Input 3
// *+a##b##c##          -a##*b##c##         -+a##b##-c##d##
// Sample Output 1      Sample Output 2     Sample Output 3
// (a+b)*c              a-b*c               a+b-(c-d)

#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -1
#define OVERFLOW_TREE -3

typedef int status;
typedef char biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    struct _biTNode *lchild;
    struct _biTNode *rchild;
} biTNode;

typedef biTNode *biTree;

status biTreeInit(biTree *tree);
status biTreeCreate(biTree tree); // --
status judgeDataClass(char data);
status biTreePrintExprs(biTNode *root, status bracketSign);

// alloc a node, with the lchild storing "root" and rchild storing "NULL". data is 0 since we don't need it
status biTreeInit(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    *ptree = (biTree)malloc(sizeof(biTNode));
    (*ptree)->data = 0;
    (*ptree)->lchild = NULL;
    (*ptree)->rchild = NULL;
    return OK;
}

// create a binary tree from input in pre-order.
status biTreeCreate(biTNode **proot)
{
    biTNode *tNode;
    char chdata;

    if (isspace(chdata = getchar()) || chdata == '#' || chdata == EOF)
        tNode = NULL;
    else
    {
        if ((tNode = (biTNode *)malloc(sizeof(biTNode))) == NULL)
            return OVERFLOW_TREE;
        tNode->data = chdata;
        biTreeCreate(&tNode->lchild);
        biTreeCreate(&tNode->rchild);
    }
    *proot = tNode;
    return OK;
}

status judgeDataClass(char data)
{
    if (data == '*' || data == '/')
        return 2;
    if (data == '+' || data == '-')
        return 1;
    if (isdigit(data))
        return 0;
    return -1;
}

status biTreePrintExprs(biTNode *root, status bracketSign)
{
    if (root == NULL)
        return OK;

    if (bracketSign == TRUE && judgeDataClass(root->data) >= 1)
        putchar('(');

    // left child
    if (root->lchild != NULL && judgeDataClass(root->data) == 2 && judgeDataClass(root->lchild->data) == 1)
        biTreePrintExprs(root->lchild, TRUE);
    else if (root->lchild != NULL)
        biTreePrintExprs(root->lchild, FALSE);
    // itself
    putchar(root->data);
    // right child
    if (root->rchild != NULL &&
        (root->data == '/' || root->data == '*' && judgeDataClass(root->rchild->data) == 1 || root->data == '-' && judgeDataClass(root->rchild->data) == 1))
        biTreePrintExprs(root->rchild, TRUE);
    else if (root->rchild != NULL)
        biTreePrintExprs(root->rchild, FALSE);

    if (bracketSign == TRUE && judgeDataClass(root->data) >= 1)
        putchar(')');

    return OK;
}

int main()
{
    biTree tree;

    biTreeInit(&tree);
    biTreeCreate(&tree->lchild);
    getchar();

    biTreePrintExprs(tree->lchild, FALSE);
    putchar('\n');

    return 0;
}