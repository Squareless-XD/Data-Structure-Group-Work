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
struct listNode
{
    struct listNode *prev;
    struct listNode *next;
    int data;
    int freq;
};

int *locateSeq;
int countSeq = 0;
int length;

struct listNode *createListFromInput(int length);
struct listNode *createNodeInList(char data, struct listNode *prev);
struct listNode *listMalloc(void);
int listLocate(struct listNode *header, int data);
int listSort(struct listNode *header, struct listNode *node, int index);
int seqCheck(int data);
int listPrint(struct listNode *header);


int main()
{
    int data, i;
    struct listNode *header, *node;
    char c;


    cin >> length;
    locateSeq = new int[length];
    header = createListFromInput(length);
    while (cin >> data)
        listLocate(header, data);

    node = header->next;
    printf("%d\n", length);
    for (i = 0; i < length; ++i)
    {
        node = node->next;
        listSort(header, node->prev, i);
    }
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
struct listNode *createNodeInList(char data, struct listNode *prev)
{
    struct listNode *node = NULL;
    if ((node = listMalloc()) == NULL)
        return NULL;
    node->prev = prev;
    node->data = data;
    node->freq = 0;
    return node;
}

struct listNode *listMalloc(void)
{
    struct listNode *newNode;
    newNode = (struct listNode *)malloc(sizeof(struct listNode));
    if (newNode == NULL)
        return NULL;
    memset(newNode, 0, sizeof(struct listNode *));
    return newNode;
}

int listLocate(struct listNode *header, int data)
{
    struct listNode *node, *lastNode, *prevNode;
    if (header == NULL)
        return -1;
    node = header->next;
    lastNode = node->prev;

    while (node->data != data && node != lastNode)
        node = node->next;

    if (node->data == data)
    {
        if (node->freq == 0)
            locateSeq[countSeq++] = data;
        node->freq = node->freq + 1;
    }
    return 0;
}

int listSort(struct listNode *header, struct listNode *node, int index)
{
    struct listNode *lastNode, *prevNode;
    int dataTemp, nodeSeqOrder;

    nodeSeqOrder = seqCheck(node->data);
    while (index)
    {
        if (node->freq = 0 || node->freq < node->prev->freq || node->freq == node->prev->freq && nodeSeqOrder > seqCheck(node->prev->data))
        {
            break;
        }
        dataTemp = prevNode->data;
        prevNode->data = node->data;
        node->data = dataTemp;
        --index;
    }

    // while (prevNode != lastNode && (nodeFreq != 0) && (nodeFreq > prevNode->freq || nodeFreq == prevNode->freq && (find(iterBegin, iter, prevNode->data) == iter)))
    // {
    //     dataTemp = prevNode->data;
    //     prevNode->data = node->data;
    //     node->data = dataTemp;
    //     // a series of switching two nodes
    //     // prevNode->prev->next = node;
    //     // node->prev = prevNode->prev;
    //     // node->next->prev = prevNode;
    //     // prevNode->next = node->next;
    //     // prevNode->prev = node;
    //     // node->next = prevNode;

    //     prevNode = node->prev;
    // }

    return 0;
}


int seqCheck(int data)
{
    int num = 0;
    while (num < length && locateSeq[num] != data)
        ++num;
    if (num == length)
        return -1;
    return length;
}

// print a linked list out, which has a header *that does not contain any data*.
int listPrint(struct listNode *header)
{
    struct listNode *node, *firstNode;
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
        //debug
        // cout << node->freq << ",";
        cout << node->data << " ";
        node = node->next;
    }
    cout << (node->data) << endl;

    return 0;
}
