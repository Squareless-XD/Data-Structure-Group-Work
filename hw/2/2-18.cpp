// Description
// 试写一算法，在无头节点的动态单链表上实现线性表操作DELETE(L,i)。

// Input
// 输入包含两行，第一行是链表中的元素，第二行表示需要删除的第i个元素，i从0开始计数
// 若删除后，链表不包含元素，则输出“NULL”。

// Output
// 输出删除后的链表元素

// Sample Input 1
// 1 2 3 4 5
// 4
// Sample Output 1
// 1 2 3 4

#include <bits/stdc++.h>
using namespace std;
// claim a variable called "struct listnode"
struct listnode
{
    int data;
    struct listnode *next;
};

struct listnode *list_malloc();
struct listnode *create_listnode(int data);
int list_print(struct listnode *header);
int list_delete(struct listnode *header, int index);

int main()
{
    int data, index;
    char c;
    struct listnode *header, *node, *newnode;

    header = list_malloc();
    node = header;
    do
    {
        scanf("%d%c", &data, &c);
        newnode = create_listnode(data);
        node->next = newnode;
        node = newnode;
    } while (c != '\n'); // until the next line is comming

    cin >> index;

    list_delete(header, index);
    list_print(header);
    return 0;
}

// create a new List node using the data called "data"
struct listnode *create_listnode(int data)
{
    struct listnode *node = NULL;
    if ((node = list_malloc()) == NULL) // assign the memory needed for the node, and judge whether something wrong happened.
        return NULL;
    node->data = data;
    node->next = NULL;
    return node;
}

// allocate a new space to a list node
struct listnode *list_malloc()
{
    struct listnode *node = NULL;
    node = (struct listnode *)malloc(sizeof(struct listnode)); // call the "malloc()" function
    if (node == NULL)                                          // if "malloc()" fails, then give the warning and return 1
    {
        cout << "Function malloc() error." << endl;
        return NULL;
    }
    return node; // if nothing happened, return 0
}

// print a linked list out, which has a header *that does not contain any data*.
int list_print(struct listnode *header)
{
    struct listnode *node;
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

int list_delete(struct listnode *header, int index)
{
    int i;
    struct listnode *node, *temp;

    if (header == NULL)
        return -1;
    node = header;
    for (i = 0; i < index && node->next != NULL; ++i)
        node = node->next;
    if (i != index || node->next == NULL)
    {
        cout << "no " << i << "-th element in list" << endl;
        return -1;
    }

    // i = 0;
    // node = header;
    // while (node != NULL && i < index - 1)
    // {
    //     node = node->next;
    //     ++i;
    // }
    // if (node == NULL || node->next == NULL)
    //     return -1;

    temp = node->next;
    node->next = temp->next;
    free(temp);
    return 0;
}
