// Description
// 设有一双向循环链表，每个节点中除有prior,data和next三个域外，还增设了一个访问频度域freq。在链表被启用之前，频度域freq的值均初始化为0，而每当对链表进行一次LOCATE(L,x)的操作后，被访问的节点（即元素值等于x的节点）中的频度域freq的值便增1，同时调整链表中节点之间的次序，使其按访问频度非递增的次序顺序排列，以便始终保持被频繁访问的节点总是靠近表头结点。编写符合上述要求的LOCATE操作的算法。

// Input
// 输入包含三行，第一行是链表中的元素个数，第二行是链表中的元素，第三行包含所有被访问的元素

// Output
// 顺序输出从表头节点开始的链表中的元素。
// 注意：如果有多个元素的访问次数相同，需要按照访问次序，将先访问到的元素放在前面

// Sample Input 1
// 10
// 1 2 3 4 5 6 7 8 9 10
// 1 2 2 2 3 4 5 5 4 10 9
// Sample Output 1
// 2 4 5 1 3 10 9 6 7 8

// 真不想用循环链表，没啥意义

#include <bits/stdc++.h>
using namespace std;

struct listNode
{
    struct listNode *prev;
    struct listNode *next;
    int data;
    int freq;
    int order;
};

struct listNode *createListFromInput(int length);
struct listNode *createNodeInList(int data);
struct listNode *listMalloc(void);
int listLocate(struct listNode *header, int data, int count);
int listSort(struct listNode *header, int length);
int listSwap(struct listNode *node1, struct listNode *node2);
int listCompare(struct listNode *node1, struct listNode *node2);
int listPrint(struct listNode *header);


int main()
{
    int data, i, length, count = 1;
    struct listNode *header, *node;

    cin >> length;
    header = createListFromInput(length);

    while (cin >> data)
        if(listLocate(header, data, count) == 1)
            ++count;

    listSort(header, length);

    listPrint(header);

    return 0;
}

// create a new list from input, with a space between each element and a '\n' at the end
struct listNode *createListFromInput(int length)
{
    int data;
    char c;
    int i;
    struct listNode *header, *node, *newNode;

    header = listMalloc(); // get a header node
    node = header;
    for (i = 0; i < length && node != NULL; ++i)
    {
        scanf("%d%c", &data, &c);
        newNode = createNodeInList(data); // add a new node with data
        newNode->prev = node;
        node->next = newNode;
        node = newNode;
    }
    if (node == NULL)
        return NULL;
    return header;
}

// create a new list node using the data called "data"
struct listNode *createNodeInList(int data)
{
    struct listNode *node = NULL;
    if ((node = listMalloc()) == NULL)
        return NULL;

    node->prev = NULL;
    node->next = NULL;
    node->data = data;
    node->freq = 0;
    node->order = 0;

    return node;
}

struct listNode *listMalloc(void)
{
    struct listNode *newNode;
    newNode = (struct listNode *)malloc(sizeof(struct listNode));
    if (newNode == NULL)
        return NULL;
    return newNode;
}

int listLocate(struct listNode *header, int data, int count)
{
    struct listNode *node;

    if (header == NULL)
        return -1;
    node = header->next;

    while (node != NULL && node->data != data)
        node = node->next;
    if (node == NULL)
        return -1;

    if (node->freq == 0)
    {
        node->order = count;
        ++node->freq;
        return 1;
    }
    ++node->freq;
    return 0;
}

int listSort(struct listNode *header, int length)
{
    struct listNode *node, *nextNode;
    int i, j;
    for (i = 0; i < length - 1; ++i)
    {
        node = header->next;
        for (j = 0; j < length - 1 - i; ++j)
        {
            nextNode = node->next;
            if (listCompare(node, nextNode))
                listSwap(node, nextNode);
            node = nextNode;
        }
    }
    return 0;
}

int listSwap(struct listNode *node1, struct listNode *node2)
{
    struct listNode temp = *node1;

    node1->data = node2->data;
    node1->freq = node2->freq;
    node1->order = node2->order;
    node2->data = temp.data;
    node2->freq = temp.freq;
    node2->order = temp.order;

    return 0;
}

// return 0 when node1 is shuold be first to node2, or just should not swap them
int listCompare(struct listNode *node1, struct listNode *node2)
{
    if (node2->freq == 0)
        return 0;

    if (node1->freq > node2->freq)
        return 0;
    if (node1->freq < node2->freq)
        return 1;
    // then node1 has the same frequency with node2
    if (node1->order < node2->order)
        return 0;
    return 1;

}

// print a linked list out, which has a header *that does not contain any data*.
int listPrint(struct listNode *header)
{
    struct listNode *node;
    if (header == NULL) // list doesn't exist'
        return -1;
    if (header->next == NULL) // list is empty
    {
        // cout << "NULL" << endl;
        return 0;
    }

    node = header->next;
    while (node->next != NULL) // node hasn't reached the end
    {
        cout << node->data << " ";
        node = node->next;
    }
    cout << node->data << endl;
    return 0;
}
