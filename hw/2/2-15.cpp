// Description
// 已知指针ha和hb分别指向两个单链表的头结点，并且已知两个链表的长度分别为m和n。试写一算法将这两个链表连接在一起（即令其中一个表的首元节点连在另一个表的最后一个节点之后），假设指针hc指向连接后的链表的头结点，并要求算法以尽可能短的时间完成链接运算。

// Input
// 输入包括三行，第一行是两个链表的长度m和n，第二行和第三行分别为链表ha和hb中的元素，以空格分隔。

// Output
// 输出合并后的链表hc，元素以空格分隔

// Sample Input 1
// 5 3
// 1 2 3 4 5
// 1 2 3
// Sample Output 1
// 1 2 3 1 2 3 4 5

#include <bits/stdc++.h>
using namespace std;

struct listnode
{
    int data;
    struct listnode *next;
};

struct listnode *list_malloc();
struct listnode *create_listnode(int data);
int print_list(struct listnode *header);

int main()
{
    int m, n, i, data;
    struct listnode *ha, *hb, *lnode, *newnode;
    
    ha = list_malloc();
    hb = list_malloc();
    cin >> m >> n;

    lnode = ha;
    for (i = 0; i < m; ++i)
    {
        cin >> data;
        newnode = create_listnode(data);
        lnode->next = newnode;
        lnode = newnode;
    }

    lnode = hb;
    for (i = 0; i < n; ++i)
    {
        cin >> data;
        newnode = create_listnode(data);
        lnode->next = newnode;
        lnode = newnode;
    }

    lnode->next = ha->next;
    print_list(hb);

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
int print_list(struct listnode *header)
{
    struct listnode *node;
    if (header == NULL) // list doesn't exist'
        return -1;
    if (header->next == NULL) // list is empty
        return 0;

    node = header->next;
    while (node->next != NULL) // node hasn't reached the end
    {
        cout << (node->data) << " ";
        node = node->next;
    }
    cout << (node->data) << endl;
    return 0;
}
