// Description
// 试写一算法，实现单链表的就地逆置

// Input
// 输入为给定链表的所有元素，以逗号分隔

// Output
// 输出为链表逆置后的结果，以逗号分隔

// Sample Input 1
// s,a,s,f,w,s,a,d,a,S
// Sample Output 1
// S,a,d,a,s,w,f,s,a,s

#include <bits/stdc++.h>
using namespace std;
struct listnode *list_malloc();
struct listnode
{
    char data;
    struct listnode *next;
};

int main()
{
    struct listnode *header, *p, *q, *r;
    char c;

    header = list_malloc();
    p = header;
    while ((c = getchar()) != '\n' && c != EOF)
    {
        if (c != ',')
        {
            q = list_malloc();
            q->data = c;
            p->next = q;
            p = q;
        }
    }
    p->next = NULL;

    p = header->next;
    if (p == NULL)
        return 0;
    q = p->next;
    p->next = NULL;
    while (q != NULL)
    {
        r = q->next;
        q->next = p;
        p = q;
        q = r;
    }
    header->next = p;

    while (p->next != NULL)
    {
        cout << p->data << ',';
        p = p->next;
    }
    cout << p->data << endl;
    return 0;
}

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
