// 6.65-建立二叉树链表
// Description
// 已知一棵二叉树的前序序列和中序序列分别存于两个一维数组中，试编写算法建立该二叉树的二叉链表，然后输出该二叉链表的后序序列。
// 比如一棵二叉树链表的示意图如下：
// 前序序列数组为：[1,2,4,5,3]
// 中序序列数组为：[4,2,5,1,3]

// Input
// 输入分为两行，
// 第一行输入为二叉树的先序序列；
// 第二行输入为二叉树的中序序列。

// Output
// 输出为二叉树的后序序列

// Sample Input 1       Sample Input 2
// 12453                ABDHEICFJKG
// 42513                DHBEIAJFKCG
// Sample Output 1      Sample Output 2
// 45231                HDIEBJKFGCA

#include <bits/stdc++.h>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR -1
#define INFEASIBLE -2
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
status biTreeTrvPost(biTNode *root, status (*visit)(biTNode *tNode));
status biTreeEstbFromInAndPre(biTNode *root, std::vector<char> *ppreOrderTree, std::vector<char> *pinOrderTree, int preBegin, int preEnd, int inBegin, int inEnd);
status printNode(biTNode *node);

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

status biTreeTrvPost(biTNode *root, status (*visit)(biTNode *tNode))
{
    if (root == NULL)
        return OK;
    if (biTreeTrvPost(root->lchild, visit) == OK && biTreeTrvPost(root->rchild, visit) == OK && visit(root) == OK)
        return OK;
    return ERROR;
}

status biTreeEstbFromInAndPre(biTNode *root, std::vector<char> *ppreOrderTree, std::vector<char> *pinOrderTree, int preBegin, int preEnd, int inBegin, int inEnd)
{
    int index = inBegin, inSplit = inBegin;

    while (inSplit < inEnd && (*pinOrderTree)[inSplit] != (*ppreOrderTree)[preBegin])
        ++inSplit;
    if (inSplit == inEnd)
        return ERROR;

    if (inSplit == inBegin)
        root->lchild = NULL;
    else
    {
        root->lchild = (biTNode *)malloc(sizeof(biTNode));
        root->lchild->data = (*ppreOrderTree)[preBegin + 1];
        biTreeEstbFromInAndPre(root->lchild, ppreOrderTree, pinOrderTree, preBegin + 1, preBegin + 1 + (inSplit - inBegin), inBegin, inSplit);
    }
    if (inSplit == inEnd - 1)
        root->rchild = NULL;
    else
    {
        root->rchild = (biTNode *)malloc(sizeof(biTNode));
        root->rchild->data = (*ppreOrderTree)[preEnd + (inSplit - inEnd) + 1];
        biTreeEstbFromInAndPre(root->rchild, ppreOrderTree, pinOrderTree, preEnd + (inSplit - inEnd) + 1, preEnd, inSplit + 1, inEnd);
    }

    return OK;
}

status printNode(biTNode *node)
{
    std::cout << node->data;
    return OK;
}

int main()
{
    std::vector<char> preOrderTree, inOrderTree;
    char c;
    int vecSize;
    biTree tree;

    biTreeInit(&tree);

    while ((c = getchar()) != '\n' && c != EOF)
        preOrderTree.push_back(c);
    while ((c = getchar()) != '\n' && c != EOF)
        inOrderTree.push_back(c);

    vecSize = preOrderTree.size();
    tree->lchild = (biTNode *)malloc(sizeof(biTNode));
    tree->lchild->data = preOrderTree[0];
    biTreeEstbFromInAndPre(tree->lchild, &preOrderTree, &inOrderTree, 0, vecSize, 0, vecSize);

    biTreeTrvPost(tree->lchild, printNode);
    putchar('\n');

    return 0;
}