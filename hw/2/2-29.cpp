// Description
// 已知A，B和C为三个递增有序的线性表，现要求对A表做如下操作：删去那些既在B表中出现又在C表中出现的元素。试对顺序表编写实现上述操作的算法。

// Input
// 输入包含三行，分别为ABC三个线性表中的元素，以逗号分隔

// Output
// 输出操作之后的A表中的元素

// Sample Input 1 
// a,b,c,d,e,f,g
// b,e,g,h,h
// c,e,g,h
// Sample Output 1
// a,b,c,d,f

#include <bits/stdc++.h>
using namespace std;
// claim a struct called "listNode"
struct listNode
{
    char data;
    struct listNode *next;
};

struct listNode *listMalloc();
struct listNode *createNodeInList(char data);
struct listNode *createListFromInput();
int listDeleteNode(struct listNode *prevNode);
int listFindElement(struct listNode *header, char checkData);
int listPrint(struct listNode *header);

int main()
{
    struct listNode *headerA, *headerB, *headerC, *node;

    headerA = createListFromInput();
    headerB = createListFromInput();
    headerC = createListFromInput();
    if (headerA == NULL || headerB == NULL || headerC == NULL)
    {
        cout << "ERROR: Invalid header!" << endl;
        return 0;
    }
    
    node = headerA;
    while (node->next != NULL)
    {
        if (listFindElement(headerB, node->next->data) == 1 && listFindElement(headerC, node->next->data) == 1)
            listDeleteNode(node);
        else
            node = node->next;
    }


    listPrint(headerA);
    return 0;
}

//create a new list from input, with a space between each element and a '\n' at the end
struct listNode *createListFromInput()
{
    char data;
    char c;
    struct listNode *header, *node, *newNode;

    header = listMalloc();
    node = header;
    do
    {
        scanf("%c%c", &data, &c);
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

// if find, return 1; if not, return 0
int listFindElement(struct listNode *header, char checkData)
{
    struct listNode *node;
    if (header == NULL)
        return -1;
    node = header->next;
    while (node != NULL)
    {
        if (node->data == checkData)
            return 1;
        node = node->next;
    }
    if (node == NULL)
        return 0;
    return -1;
}

// create a new list node using the data called "data"
struct listNode *createNodeInList(char data)
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
        cout << (node->data) << ",";
        node = node->next;
    }
    cout << (node->data) << endl;
    return 0;
}
