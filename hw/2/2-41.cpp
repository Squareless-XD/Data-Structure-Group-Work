// Description
// 试以循环链表作稀疏多项式的存储结构，编写求其导函数的算法，要求利用原多项式中的节点空间存放其导函数（多项式），同时释放所有无用（被删）节点。
// 稀疏多项式中采用的循环链表存储结构LinkedPoly定义为
// typedef struct polyNode{
//   PolyTerm data;
//   Struct polyNode *next;
// } polyNode, *PolyLink;
// typedef PolyLink LinkedPoly;

// Input
// 输入为给定的多项式

// Output
// 输出为求导之后的结果，按照多项式中每一项的幂次由高到低排列
// 如果导数为0，则输出0

// Sample Input 1
// - 2x + 4x^2 + 2x^3 + 3x^3 + 2x + 3x^2 - 3x^10
// Sample Output 1
// - 30x^9 + 15x^2 + 14x

// Sample Input 2
// 3x^2 + 4
// Sample Output 2
// 6x

#include <bits/stdc++.h>
using namespace std;

struct polyNode
{
    int coeff;
    int power;
    char sign;
    struct polyNode *next;
};

struct polyNode *createListFromInput();
struct polyNode *createNodeInList(int coeff, int power, char sign);
struct polyNode *listMalloc(void);
int polySort(struct polyNode *header);
int polyPowCmp(struct polyNode *former, struct polyNode *latter);
int polyAdd(struct polyNode *adderPrev, struct polyNode *another);
int listInsertByCopy(struct polyNode *insert, struct polyNode *node);
int listDeleteNode(struct polyNode *prevNode);
int polyDeriv(struct polyNode *header);
int listPrint(struct polyNode *header);

int main()
{
    int data, i, length, count = 1;
    struct polyNode *header, *node;

    header = createListFromInput();
    // listPrint(header);

    polySort(header);
    polyDeriv(header);
    listPrint(header);

    return 0;
}

// create a new list from input, with a space between each element and a '\n' at the end
struct polyNode *createListFromInput()
{
    int coeff, power;
    char c, sign, x, powerSign;
    struct polyNode *header, *node, *newNode;

    header = listMalloc(); // get a header node

    if ((sign = getchar()) == '-')
    {
        scanf("%d", &coeff);
        if ((c = getchar()) != 'x')
            power = 0;
        else if ((c = getchar()) == '^')
            scanf("%d", &power);
        else
            power = 1;
    }
    else
    {
        coeff = sign - '0';
        sign = '+';
        while (isdigit(c = getchar()))
            coeff = coeff * 10 + c - '0';
        if (c != 'x')
            power = 0;
        else if ((c = getchar()) == '^')
            scanf("%d", &power);
        else
            power = 1;
    }
    node = createNodeInList(coeff, power, sign);
    header->next = node;

    while (c != EOF && c != '\n' && sign != EOF && sign != '\n')
    {
        while ((c = getchar()) == ' ')
            ;
        if (c == '\n' || c == EOF)
            break;
        sign = c;
        scanf("%d", &coeff);
        if ((c = getchar()) != 'x')
            power = 0;
        else if ((c = getchar()) == '^')
            scanf("%d", &power);
        else
            power = 1;

        newNode = createNodeInList(coeff, power, sign);
        node->next = newNode;
        node = newNode;
    }
    if (node == NULL)
        return NULL;
    return header;
}

// create a new list node using the data called "data"
struct polyNode *createNodeInList(int coeff, int power, char sign)
{
    struct polyNode *node = NULL;
    if ((node = listMalloc()) == NULL)
        return NULL;

    node->next = NULL;
    node->coeff = coeff;
    node->power = power;
    node->sign = sign;

    return node;
}

struct polyNode *listMalloc(void)
{
    struct polyNode *newNode;
    newNode = (struct polyNode *)malloc(sizeof(struct polyNode));
    if (newNode == NULL)
        return NULL;
    return newNode;
}

int polySort(struct polyNode *header)
{
    struct polyNode *node, *insert;

    if (header == NULL) // list doesn't exist'
    {
        // cout << "Error: polySort(1)" << endl;
        return -1;
    }

    if (header->next == NULL)
        return 0;
    node = header->next;
    while (node->next != NULL)
    {
        insert = header;
        while (insert->next != NULL && polyPowCmp(insert->next, node->next) == 2) // here cmp==2 means the former is larger
            insert = insert->next;
        if (insert == node)
        {
            node = node->next;
            continue;
        }
        if (insert->next == NULL) // unexpected, but need to be dealt with
        {
            // cout << "Error: polySort(2)" << endl;
            return -1;
        }
        if (polyPowCmp(insert->next, node->next) == 1) // here cmp==1 means equal
        {
            if (polyAdd(insert, node->next) == 0)
                listDeleteNode(node);
            else
            {
                node = insert;
                listDeleteNode(insert);
                listDeleteNode(insert);
            }
        }
        else
        {
            listInsertByCopy(insert, node->next);
            listDeleteNode(node);
        }
    }

    return 0;
}

// 2 means the former is larger, 1 means equal, 0 means the latter is larger
int polyPowCmp(struct polyNode *former, struct polyNode *latter)
{
    if (former->power < latter->power)
        return 0;
    if (former->power > latter->power)
        return 2;
    return 1;
}

int polyAdd(struct polyNode *adderPrev, struct polyNode *another)
{
    struct polyNode *adder;
    adder = adderPrev->next;
    if (adder == NULL || another == NULL) // list doesn't exist'
    {
        // cout << "Error: polyAdd" << endl;
        return -1;
    }

    if (adder->sign == another->sign)
        adder->coeff += another->coeff;
    else if (adder->coeff < another->coeff)
    {
        adder->coeff = another->coeff - adder->coeff;
        adder->sign = (adder->sign == '-' ? '+' : '-');
    }
    else if (adder->coeff > another->coeff)
        adder->coeff -= another->coeff;
    else if (adder->next != another)
        listDeleteNode(adderPrev);
    else
        return 1;
    return 0;
}

int listInsertByCopy(struct polyNode *insert, struct polyNode *node)
{
    struct polyNode *newNode;

    if (insert == NULL || node == NULL) // list doesn't exist'
    {
        // cout << "Error: listInsertByCopy(1)" << endl;
        return -1;
    }
    newNode = createNodeInList(node->coeff, node->power, node->sign);
    if (newNode == NULL)
    {
        // cout << "Error: listInsertByCopy(2)" << endl;
        return -1;
    }
    newNode->next = insert->next;
    insert->next = newNode;

    return 0;
}

int listDeleteNode(struct polyNode *prevNode)
{
    struct polyNode *temp;

    if (prevNode == NULL)
    {
        // cout << "Error: listDeleteNode" << endl;
        return -1;
    }
    if (prevNode->next == NULL)
        return 0;

    temp = prevNode->next;
    prevNode->next = temp->next;
    free(temp);
    return 0;
}

int polyDeriv(struct polyNode *header)
{
    struct polyNode *node, *prevNode;

    if (header == NULL) // list doesn't exist'
    {
        // cout << "Error: polyDeriv" << endl;
        return -1;
    }

    if (header->next == NULL)
        return 0;

    prevNode = header;
    node = header->next;
    while (node->next != NULL)
    {
        node->coeff *= node->power;
        --node->power;
        prevNode = node;
        node = node->next;
    }
    if (node->power == 0)
        listDeleteNode(prevNode);
    else
    {
        node->coeff *= node->power;
        --node->power;
    }
    return 0;
}

// print a linked list out, which has a header *that does not contain any data*.
int listPrint(struct polyNode *header)
{
    struct polyNode *node;
    if (header == NULL) // list doesn't exist'
    {
        // cout << "Error: listPrint" << endl;
        return -1;
    }
    if (header->next == NULL) // list is empty
    {
        printf("0\n");
        return 0;
    }

    node = header->next;
    if (node->sign == '-')
        printf("- %dx", node->coeff);
    else
        printf("%dx", node->coeff);
    if (node->power != 1)
        printf("^%d", node->power);
    printf(" ");
    node = node->next;

    while (node->next != NULL) // node hasn't reached the end
    {
        printf("%c %dx", node->sign, node->coeff);
        if (node->power != 1)
            printf("^%d", node->power);
        printf(" ");
        node = node->next;
    }
    printf("%c %d", node->sign, node->coeff);
    if (node->power >= 1)
        printf("x");
    if (node->power > 1)
        printf("^%d", node->power);
    printf("\n");
    return 0;
}
