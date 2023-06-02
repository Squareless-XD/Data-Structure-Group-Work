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

#include <bits/stdc++.h>
using namespace std;
struct listNodeDC
{
    struct listNodeDC *prev;
    struct listNodeDC *next;
    int data;
    int freq;
};
vector<int> locateSeq;

struct listNodeDC *createListFromInput(int length);
struct listNodeDC *createNodeInList(char data, struct listNodeDC *prev);
struct listNodeDC *listMalloc(void);
int listLocate(struct listNodeDC *header, int data);
int listPrint(struct listNodeDC *header);


int main()
{
    int data, length;
    struct listNodeDC *header, *node;
    char c;

    cin >> length;
    header = createListFromInput(length);
    while (cin >> data)
    {
        listLocate(header, data);
    }
    listPrint(header);

    return 0;
}

// create a new list from input, with a space between each element and a '\n' at the end
struct listNodeDC *createListFromInput(int length)
{
    int data;
    char c;
    int i;
    struct listNodeDC *header, *node, *newNode;

    header = listMalloc();
    node = header;
    for (i = 0; i < length && node != NULL; ++i)
    {
        scanf("%d%c", &data, &c);
        newNode = createNodeInList(data, node);
        node->next = newNode;
        node = newNode;
    }
    if (node == NULL)
        return NULL;
    node->next = header->next;
    header->next->prev = node;
    return header;
}

// create a new list node using the data called "data"
struct listNodeDC *createNodeInList(char data, struct listNodeDC *prev)
{
    struct listNodeDC *node = NULL;
    if ((node = listMalloc()) == NULL)
        return NULL;
    node->prev = prev;
    node->data = data;
    node->freq = 0;
    return node;
}

struct listNodeDC *listMalloc(void)
{
    struct listNodeDC *newNode;
    newNode = (struct listNodeDC *)malloc(sizeof(struct listNodeDC));
    if (newNode == NULL)
        return NULL;
    memset(newNode, 0, sizeof(struct listNodeDC *));
    return newNode;
}

int listLocate(struct listNodeDC *header, int data)
{
    struct listNodeDC *node, *lastNode, *prevNode;
    vector<int>::iterator iter, iterBegin;
    if (header == NULL)
        return -1;
    node = header->next;
    lastNode = node->prev;

    while (node->data != data && node != lastNode)
        node = node->next;

    if (node->data == data)
    {
        if (node->freq == 0)
            locateSeq.push_back(data);
        node->freq = node->freq + 1;
        iterBegin = locateSeq.begin();
        iter = find(iterBegin, locateSeq.end(), data);

        prevNode = node->prev;
        while (prevNode != lastNode && (node->freq > prevNode->freq || node->freq == prevNode->freq && !count(iterBegin, iter, prevNode->data)))
        {
            // a series of switching two nodes
            prevNode->prev->next = node;
            node->prev = prevNode->prev;
            node->next->prev = prevNode;
            prevNode->next = node->next;
            prevNode->prev = node;
            node->next = prevNode;

            prevNode = node->prev;
        }
        if (prevNode == lastNode)
            header->next = node;
    }
    return 0;
}

// print a linked list out, which has a header *that does not contain any data*.
int listPrint(struct listNodeDC *header)
{
    struct listNodeDC *node, *firstNode;
    if (header == NULL) // list doesn't exist'
        return -1;
    if ((firstNode = header->next) == NULL) // list is empty
    {
        cout << "NULL" << endl;
        return 0;
    }

    node = firstNode;
    while (node->next != firstNode) // node hasn't reached the end
    {
        cout << (node->data) << " ";
        node = node->next;
    }
    cout << (node->data) << endl;
    return 0;
}
