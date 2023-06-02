// 6.58-线索二叉树插入
// Description
// 试写一个算法，在中序全线索二叉树的节点*p之下，插入一棵以节点*x为根，只有左子树的中序全线索二叉树，使*x为根的二叉树成为*p的左子树。若*p原来有左子树，则令它为*x的右子树。完成插入之后的二叉树应保持全线索化的特性。
// 例如，给定二叉树如下，*p=2
// 若以节点*x=7为根的二叉树如下，
// 则插入后的二叉树如下，

// Input
// 首先输入*p所在的二叉树，使用6.43中的表示法表示。
// 然后输入*p节点
// 最后输入以*x为根的二叉树，同样使用6.43中的表示法表示，注意此时的根节点不一定为1，此时数组中的下标和节点的对应关系如下所示。
// 例如，例子中的*x所在二叉树的表示为
// L=7 8 0
// R=7 0 0
// 此时，设偏移量o为根节号减1，即o=7-1=6，节点7的左右子节点对应L[7-6]=8，R[7-6]=8，节点8的左右子节点对应L[8-6]=0，R[8-6]=0。

// Output
// 输出插入之后树的中序遍历序列

// Sample Input 1           Sample Input 2
// 1 2 4 0 0 0              1 0 0
// 1 3 5 0 0 0              1 2 0
// 2                        1
// 7 8 0                    3 4 5 0 0
// 7 0 0                    3 0 6 0 0
// Sample Output 1          Sample Output 2
// 8 7 4 2 5 1 3            5 4 6 3 1 2

#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_TREE -2
#define OVERFLOW_STACK -2
#define LINK 0
#define THREAD 1

int firstPrintSign = TRUE;

typedef int biTElemType;

typedef struct _biTNode
{
    biTElemType data;
    int rTag; // rTag == 0 means it is storing a rchild, 1 means a succ
    int lTag;
    struct _biTNode *lchild;
    struct _biTNode *rchild;
} biTNode;

typedef int status;
typedef biTNode *stackElemType;

typedef struct
{
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

sqStack *stackAlloc(void);
status initStack_Sq(sqStack *stack);
status destroyStack_Sq(sqStack *stack);
status stackEmpty_Sq(sqStack *stack);
status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);

sqStack *stackAlloc(void)
{
    sqStack *stack;
    stack = (sqStack *)malloc(sizeof(sqStack));
    if (!stack)
        return NULL;
    return stack;
}

status initStack_Sq(sqStack *stack)
{
    stack->base = (stackElemType *)malloc(STACK_INIT_SIZE * sizeof(stackElemType));
    if (!stack->base)
        exit(OVERFLOW_STACK);
    stack->top = stack->base;
    stack->stacksize = STACK_INIT_SIZE;
    return OK;
}

status destroyStack_Sq(sqStack *stack)
{
    free(stack->base);
    return OK;
}

status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return TRUE;
    return FALSE;
}

status getTop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *(stack->top - 1);
    return OK;
}

status push_Sq(sqStack *stack, stackElemType e)
{
    if (stack->top - stack->base >= stack->stacksize)
    {
        stack->base = (stackElemType *)realloc(stack->base, (stack->stacksize + STACK_INCREMENT) * sizeof(stackElemType));
        if (!stack->base)
            exit(OVERFLOW_STACK);
        stack->top = stack->base + stack->stacksize;
        stack->stacksize += STACK_INCREMENT;
    }
    *(stack->top++) = e;
    return OK;
}

status pop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *--stack->top;
    return OK;
}

status biTreeInit(biTNode *biTree);
status biTreeThreaded(biTNode *biTree);
status addNewThread(biTNode *node_next, biTNode *node_prev);
status biTreeSetFlag(biTNode *biTree);
status biTreeFindElem(biTNode *biTree, biTElemType elemFind, biTNode **firstNode);
status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode)); // --
status printTreeOneStep(biTNode *tNode);

status biTreeInit(biTNode *biTree)
{
    if (biTree != NULL)
        return ERROR;
    biTree = (biTNode *)malloc(sizeof(biTNode));
    return OK;
}

// create thread in a binary tree
status biTreeThreaded(biTNode *biTree)
{
    biTreeSetFlag(biTree);

    sqStack *tNodeStack;
    biTNode *nodeTemp, *nodeLast;

    tNodeStack = stackAlloc();
    initStack_Sq(tNodeStack);
    push_Sq(tNodeStack, biTree);
    nodeLast = NULL;

    while (stackEmpty_Sq(tNodeStack) == FALSE)
    {
        while (getTop_Sq(tNodeStack, &nodeTemp) == OK && nodeTemp != NULL)
            push_Sq(tNodeStack, nodeTemp->lchild);
        pop_Sq(tNodeStack, &nodeTemp);
        if (stackEmpty_Sq(tNodeStack) == FALSE)
        {
            pop_Sq(tNodeStack, &nodeTemp);
            addNewThread(nodeTemp, nodeLast); // add new thread between two node
            nodeLast = nodeTemp;              // store the last visited node
            push_Sq(tNodeStack, nodeTemp->rchild);
        }
    }
    addNewThread(NULL, nodeLast);

    destroyStack_Sq(tNodeStack);
    free(tNodeStack);
    return OK;
}

status addNewThread(biTNode *node_next, biTNode *node_prev)
{
    if (node_next != NULL && node_next->lTag == THREAD)
        node_next->lchild = node_prev;
    if (node_prev != NULL && node_prev->rTag == THREAD)
        node_prev->rchild = node_next;
    return OK;
}

// set lTag and rTag
status biTreeSetFlag(biTNode *biTree)
{
    if (biTree == NULL)
        return ERROR;
    if (biTree->lchild == NULL)
        biTree->lTag = THREAD;
    else
    {
        biTree->lTag = LINK;
        biTreeSetFlag(biTree->lchild);
    }

    if (biTree->rchild == NULL)
        biTree->rTag = THREAD;
    else
    {
        biTree->rTag = LINK;
        biTreeSetFlag(biTree->rchild);
    }
    return OK;
}

status biTreeFindElem(biTNode *biTree, biTElemType elemFind, biTNode **firstNode)
{
    if (biTree == NULL)
    {
        *firstNode = NULL;
        return FALSE;
    }
    if (biTree->data == elemFind)
    {
        *firstNode = biTree;
        return TRUE;
    }
    if (biTreeFindElem(biTree->lchild, elemFind, firstNode) == TRUE || biTreeFindElem(biTree->rchild, elemFind, firstNode) == TRUE)
        return TRUE;
    return FALSE;
}

status biTreeTrvIn(biTNode *biTree, status (*visit)(biTNode *tNode))
{
    biTNode *temp;

    if (biTree == NULL)
        return OK;
    temp = biTree;
    while (temp != NULL)
    {
        while (temp->lTag == LINK)
            temp = temp->lchild;
        if (visit(temp) != OK) // visit after reaching the bottom
            return ERROR;
        while (temp->rTag == THREAD && temp->rchild != NULL)
        {
            temp = temp->rchild;
            visit(temp);
        }
        temp = temp->rchild;
    }

    return OK;
}

status printTreeOneStep(biTNode *tNode)
{
    if (firstPrintSign == TRUE)
    {
        firstPrintSign = FALSE;
        cout << tNode->data;
    }
    else
        cout << " " << tNode->data;
    return OK;
}

int main()
{
    biTNode *tree, *tree2, *nodeTemp, *nodeTemp2, *nodeTemp3;
    vector<int> L1, R1, L2, R2, tempVec, inOrderTrav;
    vector<biTNode *> newNode, newNode2;
    int d, elemNum1, elemNum2, i, insertNode, offset;
    char c;
    status (*func1)(biTNode *);

    func1 = printTreeOneStep;

    while (scanf("%d%c", &d, &c), c == ' ')
        L1.push_back(d);
    L1.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        R1.push_back(d);
    R1.push_back(d);

    elemNum1 = L1.size() - 1;

    newNode = vector<biTNode *>(elemNum1 + 1);
    for (i = 1; i <= elemNum1; ++i)
    {
        newNode[i] = (biTNode *)malloc(sizeof(biTNode));
        newNode[i]->data = i;
    }
    tree = newNode[1];
    for (i = 1; i <= elemNum1; ++i)
    {
        if (L1[i] == 0)
            newNode[i]->lchild = NULL;
        else
            newNode[i]->lchild = newNode[L1[i]];
        if (R1[i] == 0)
            newNode[i]->rchild = NULL;
        else
            newNode[i]->rchild = newNode[R1[i]];
    }
    biTreeThreaded(tree);

    cin >> insertNode;

    while (scanf("%d%c", &d, &c), c == ' ')
        L2.push_back(d);
    L2.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        R2.push_back(d);
    R2.push_back(d);

    elemNum2 = L2.size() - 1;
    offset = L2[0] - 1;

    newNode2 = vector<biTNode *>(elemNum2 + 1);
    for (i = 1; i <= elemNum2; ++i)
    {
        newNode2[i] = (biTNode *)malloc(sizeof(biTNode));
        newNode2[i]->data = i + offset;
    }
    tree2 = newNode2[1];
    for (i = 1; i <= elemNum2; ++i)
    {
        if (L2[i] == 0)
            newNode2[i]->lchild = NULL;
        else
            newNode2[i]->lchild = newNode2[L2[i] - offset];
        if (R2[i] == 0)
            newNode2[i]->rchild = NULL;
        else
            newNode2[i]->rchild = newNode2[R2[i] - offset];
    }
    biTreeThreaded(tree2);

    biTreeFindElem(tree, insertNode, &nodeTemp);
    if (nodeTemp->lTag == LINK)
    {
        tree2->rTag = LINK;
        tree2->rchild = nodeTemp->lchild;
        nodeTemp->lchild = tree2;

        nodeTemp2 = tree2;
        while (nodeTemp2->lTag == LINK)
            nodeTemp2 = nodeTemp2->lchild;
        nodeTemp3 = tree2->rchild;
        while (nodeTemp3->lTag == LINK)
            nodeTemp3 = nodeTemp3->lchild;

        nodeTemp2->lchild = nodeTemp3->lchild;
        nodeTemp3->lchild = tree2;
    }
    else
    {
        tree2->rchild = nodeTemp;
        nodeTemp2 = tree2;
        while (nodeTemp2->lTag == LINK)
            nodeTemp2 = nodeTemp2->lchild;
        nodeTemp2->lchild = nodeTemp->lchild;

        nodeTemp->lTag = LINK;
        nodeTemp->lchild = tree2;
    }

    biTreeTrvIn(tree, func1);

    putchar('\n');

    return 0;
}
