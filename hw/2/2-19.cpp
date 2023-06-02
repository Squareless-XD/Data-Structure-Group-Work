// Description
// 已知线性表中的元素以递增有序排列，并以单链表为存储结构。试写一高效算法，删除表中所有值大于mink且小于maxk的元素（若表中存在这样的元素），同时释放被删节点的空间。
// 注意：mink和maxk是给定的两个参变量，他们的值可以和表中的元素相同，也可以不同。

// Input
// 输入包含两行，第一行是链表中的元素，以空格分隔。
// 第二行分别为mink和maxk两个元素，以空格分隔。

// Output
// 输出最后的链表中的元素

// Sample Input 1 
// 1 2 3 4 5 8 12 15 16 22
// 6 18
// Sample Output 1
// 1 2 3 4 5 22

#include <bits/stdc++.h>
using namespace std;
// claim a struct called "listNode"
struct listNode
{
    int data;
    struct listNode *next;
};

struct listNode *listMalloc();
struct listNode *createNodeInList(int data);
struct listNode *createListFromInput();
int listDeleteNode(struct listNode *prevNode);
int listPrint(struct listNode *header);
// int listDelete(struct listNode *header, int index);

int main()
{
    int mink, maxk;
    struct listNode *header, *node;

    header = createListFromInput();
    if (header == NULL)
    {
        cout << "ERROR: Invalid header!" << endl;
        return 0;
    }
    cin >> mink >> maxk;

    node = header;
    while (node->next != NULL && node->next->data <= mink)
        node = node->next;
    while (node->next != NULL && node->next->data < maxk)
        listDeleteNode(node);

    listPrint(header);
    return 0;
}

//create a new list from input, with a space between each element and a '\n' at the end
struct listNode *createListFromInput()
{
    int data;
    char c;
    struct listNode *header, *node, *newNode;

    header = listMalloc();
    node = header;
    do
    {
        scanf("%d%c", &data, &c);
        newNode = createNodeInList(data);
        node->next = newNode;
        node = newNode;
    } while (c != '\n' && node != NULL);// until the next line is coming
    if (node == NULL)
        return NULL;
    return header;
}

int listDeleteNode(struct listNode *prevNode)
{
    struct listNode *temp;
    if (prevNode == NULL || prevNode->next == NULL)
        return -1;
    temp = prevNode->next;
    prevNode->next = temp->next;
    free(temp);
    return 0;
}

// create a new list node using the data called "data"
struct listNode *createNodeInList(int data)
{
    struct listNode *node = NULL;
    if ((node = listMalloc()) == NULL) // assign the memory needed for the node, and judge whether something wrong happened.
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// allocate a new space to a list node
struct listNode *listMalloc()
{
    struct listNode *node = NULL;
    node = (struct listNode *)malloc(sizeof(struct listNode)); // call the "malloc()" function
    if (node == NULL)                                          // if "malloc()" fails, then give the warning and return 1
    {
        // this controls whether "NULL" should be printed when an empty list is printed here
        // cout << "Function malloc() error." << endl;
        return NULL;
    }
    return node; // if nothing happened, return 0
}

// print a linked list out, which has a header *that does not contain any data*.
int listPrint(struct listNode *header)
{
    struct listNode *node;
    if (header == NULL) // list doesn't exist'
        return -1;
    if (header->next == NULL) // list is empty
    {
        cout << "NULL" << endl;
        return 0;
    }

    node = header->next;
    while (node->next != NULL) // node hasn't reached the end
    {
        cout << (node->data) << " ";
        node = node->next;
    }
    cout << (node->data) << endl;
    return 0;
}

// int listDelete(struct listNode *header, int index)
// {
//     int i;
//     struct listNode *node, *temp;

//     if (header == NULL)
//         return -1;
//     node = header;
//     for (i = 0; i < index && node->next != NULL; ++i)
//         node = node->next;
//     if (i != index || node->next == NULL)
//     {
//         cout << "no " << i << "-th element in list" << endl;
//         return -1;
//     }

//     // i = 0;
//     // node = header;
//     // while (node != NULL && i < index - 1)
//     // {
//     //     node = node->next;
//     //     ++i;
//     // }
//     // if (node == NULL || node->next == NULL)
//     //     return -1;

//     temp = node->next;
//     node->next = temp->next;
//     free(temp);
//     return 0;
// }
