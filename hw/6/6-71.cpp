// Description
// 假设树上每个结点所含的数据元素为一个字母，并且以孩子-兄弟链表为树的存储结构，试写一个按凹入表方式打印一棵树的算法。如下例，

// Input
// 输入为n 行，表示n 个节点的信息。
// 第i，i∈[1,n] 行为节点i 的信息，包括一个字符c 和两个数字p 和q， 这表示第i 个节点的内容为字符c, 其一个子节点为p, **下一个兄弟**为q。空节点使用 -1 表示。

// Output
// 凹入表（具体格式如样例所示,用连接符表示层次关系）。

// Sample Input 1       Sample Input 2
// A 2 -1               A 2 -1
// B 5 3                B 3 -1
// C 7 4                C -1 -1
// D -1 -1
// E -1 6
// F -1 -1
// G -1 -1
// Sample Output 1      Sample Output 2
// A                    A
// -B                   -B
// --E                  --C
// --F
// -C
// --G
// -D

#include <bits/stdc++.h>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1

typedef int status;
typedef char biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    struct _biTNode *firstChild;
    struct _biTNode *nextsibling;
} biTNode;

typedef biTNode *biTree;

status biTreeInit(biTree *ptree);
status biTreePrint(biTNode *root, int depth);

status biTreeInit(biTree *ptree)
{
    if (ptree == NULL)
        return ERROR;
    *ptree = (biTree)malloc(sizeof(biTNode));
    (*ptree)->data = 0;
    (*ptree)->firstChild = NULL;
    (*ptree)->nextsibling = NULL;
    return OK;
}

status biTreePrint(biTNode *root, int depth)
{
    int num = depth;

    if (root == NULL)
        return OK;

    while (--num)
        putchar('-');
    cout << root->data << endl;

    if (root->firstChild != NULL)
        biTreePrint(root->firstChild, depth + 1);
    if (root->nextsibling != NULL)
        biTreePrint(root->nextsibling, depth);

    return OK;
}

int main()
{
    vector<int> firstChildren, nextsiblings;
    vector<char> treeData;
    vector<biTNode *> tNodeAssign;
    biTree tree;
    int cTemp, sTemp, d, elemNum, i, completeSign;
    char dataTemp, c;

    treeData.push_back('0');
    firstChildren.push_back(-1);
    nextsiblings.push_back(-1);
    while ((dataTemp = getchar()) != EOF)
    {
        scanf(" %d %d%c", &cTemp, &sTemp, &c);
        if (c == EOF)
            break;
        treeData.push_back(dataTemp);
        firstChildren.push_back(cTemp);
        nextsiblings.push_back(sTemp);
    }

    elemNum = treeData.size() - 1;
    tNodeAssign = vector<biTNode *>(elemNum + 1);
    for (i = 1; i <= elemNum; ++i)
    {
        tNodeAssign[i] = (biTNode *)malloc(sizeof(biTNode));
        tNodeAssign[i]->data = treeData[i];
    }

    biTreeInit(&tree);
    tree->firstChild = tNodeAssign[1];

    for (i = 1; i <= elemNum; ++i)
    {
        if (firstChildren[i] == -1)
            tNodeAssign[i]->firstChild = NULL;
        else
            tNodeAssign[i]->firstChild = tNodeAssign[firstChildren[i]];
        if (nextsiblings[i] == -1)
            tNodeAssign[i]->nextsibling = NULL;
        else
            tNodeAssign[i]->nextsibling = tNodeAssign[nextsiblings[i]];
    }

    // below we finished the establishment of the binary tree

    biTreePrint(tree->firstChild, 1);

    return 0;
}
