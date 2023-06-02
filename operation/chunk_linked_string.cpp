#include <bits/stdc++.h>

#define CHUNKSIZE 80 // tip: the length is fixed

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_STRING -2

typedef int status;

typedef struct chunkForString
{
    char ch[CHUNKSIZE];          // the data in each chunk
    struct chunkForString *next; // the next chunk
} chunk;

typedef struct
{
    chunk *head;
    chunk *tail;
    int len;
} lString;

// initialize string in chunk link
void initString_L(lString *str)
{
    str->head = NULL;
    str->tail = NULL;
    str->len = 0;
}

status chunkAlloc(chunk *newChunk)
{
    newChunk = (chunk *)malloc(sizeof(chunk)); // alloc a new space
    if (!newChunk)
        return OVERFLOW_STRING;
    newChunk->next = NULL;
    return OK;
}

status strAssign_L(lString *str, char *chars)
{
    int len, chunkNums, chunkRemain, index, count;
    chunk *newChunk;

    initString_L(str);

    len = strlen(chars);
    if (!len)
        return ERROR;

    chunkNums = len / CHUNKSIZE;   // the number of chunks
    chunkRemain = len % CHUNKSIZE; // the remaining characters
    if (chunkRemain)
        chunkNums++;

    for (index = 1; index <= chunkNums; index++)
    {
        chunkAlloc(newChunk);
        if (index == 1)
            str->head = str->tail = newChunk;
        else
        {
            str->tail->next = newChunk;
            str->tail = newChunk;
        }

        for (count = 0; count < CHUNKSIZE && count + (index - 1) * CHUNKSIZE < len; count++)
            str->tail->ch[count] = chars[count + (index - 1) * CHUNKSIZE];
    }

    while (count < CHUNKSIZE) // give each chunk a '\0'
    {
        str->tail->ch[count] = '\0';
        count++;
    }
    str->len = len;
    return OK;
}

status strCopy_L(lString *des, lString src)
{
    int i;
    chunk *chunkInsert, *chunkSrc;

    initString_L(des);
    chunkSrc = src.head;
    while (chunkSrc != NULL)
    {
        chunkAlloc(chunkInsert);

        if (chunkSrc == src.head)
            des->head = des->tail = chunkInsert;
        else
        {
            des->tail->next = chunkInsert;
            des->tail = chunkInsert;
        }
        for (i = 0; i < CHUNKSIZE; i++)
            chunkInsert->ch[i] = chunkSrc->ch[i];

        chunkSrc = chunkSrc->next;
    }
    des->len = src.len;

    return OK;
}

status strEmpty_L(lString *str)
{
    if (str->head == NULL && str->tail == NULL && str->len == 0)
        return TRUE;
    else
        return ERROR;
}

int strCompare_L(lString *former, lString *latter)
{
    int i;
    chunk *fmNode = former->head;
    chunk *ltNode = latter->head;

    while (fmNode != NULL && ltNode != NULL)
    {
        for (i = 0; i < CHUNKSIZE; i++)
        {
            if (fmNode->ch[i] != ltNode->ch[i])
                return fmNode->ch[i] - ltNode->ch[i];
        }

        fmNode = fmNode->next;
        ltNode = ltNode->next;
    }

    return former->len - latter->len;
}

int strLength_L(lString str)
{
    return str.len;
}

void clearString_L(lString *str)
{
    chunk *strChunk, *strTemp;

    strChunk = str->head;
    while (strChunk)
    {
        strTemp = strChunk->next;
        free(strChunk);
        strChunk = strTemp;
    }
    str->head = NULL;
    str->tail = NULL;
    str->len = 0;
}

void concat_L(lString *des, lString strFormer, lString strLatter)
{
    int i = 0, j = 0, k = 0, count;
    chunk *desHead, *fmChunk, *ltChunk;

    initString_L(des);

    desHead = des->head;
    fmChunk = strFormer.head;
    ltChunk = strLatter.head;

    while (fmChunk != NULL || ltChunk != NULL)
    {
        if (desHead == NULL)
        {
            chunkAlloc(desHead);

            if (des->head == NULL)
                des->head = des->tail = desHead;
            else
            {
                des->tail->next = desHead;
                des->tail = desHead;
            }
        }

        if (fmChunk != NULL)
        {
            while (fmChunk && fmChunk->ch[j])
            {
                desHead->ch[i] = fmChunk->ch[j];
                i = (i + 1) % CHUNKSIZE;
                j = (j + 1) % CHUNKSIZE;

                if (!j || !(fmChunk->ch[j]))
                    fmChunk = fmChunk->next;
                if (!i)
                {
                    desHead = desHead->next;
                    break;
                }
            }
        }
        else
        {
            while (ltChunk && ltChunk->ch[k])
            {
                desHead->ch[i] = ltChunk->ch[k];
                i = (i + 1) % CHUNKSIZE;
                k = (k + 1) % CHUNKSIZE;

                if (!k || !(ltChunk->ch[k]))
                    ltChunk = ltChunk->next;
                if (!i)
                {
                    desHead = desHead->next;
                    break;
                }
            }
        }
    }

    des->len = strFormer.len + strLatter.len;

    count = (des->len - 1) % CHUNKSIZE + 1;
    while (count < CHUNKSIZE)
    {
        des->tail->ch[count] = '\0';
        count++;
    }
}

status subString_L(lString *substr, lString *str, int pos, int len)
{
    int i = 0, j = 0, remainder, count;
    chunk *subChunk, *strChunk;

    initString_L(substr);

    if (strEmpty_L(str))
        return ERROR;

    if (pos < 1 || pos > str->len || len < 0 || pos + len - 1 > str->len)
        return ERROR;

    // move to where the substring should begin
    for (count = 1, strChunk = str->head; pos > count * CHUNKSIZE; count++, strChunk = strChunk->next)
        ;

    subChunk = substr->head;
    remainder = (pos % CHUNKSIZE) - 1;

    while (i < len)
    {
        if (!subChunk)
        {
            chunkAlloc(subChunk);
            if (!substr->head)
                substr->head = substr->tail = subChunk;
            else
            {
                substr->tail->next = subChunk;
                substr->tail = subChunk;
            }
        }
        while (i < len)
        {
            subChunk->ch[j] = strChunk->ch[remainder];
            j = (j + 1) % CHUNKSIZE;
            remainder = (remainder + 1) % CHUNKSIZE;
            ++i;

            if (remainder == NULL)
                strChunk = strChunk->next;
            if (j == NULL)
            {
                subChunk = subChunk->next;
                break;
            }
        }
    }
    substr->len = len;

    count = (substr->len - 1) % CHUNKSIZE + 1;
    while (count < CHUNKSIZE)
    {
        substr->tail->ch[count] = '\0';
        count++;
    }

    return OK;
}

int index_L(lString *src, lString *pattern, int pos)
{
    int srcLen, patLen;
    lString sub;

    if (pos > 0 && pos <= src->len)
    {
        srcLen = src->len;
        patLen = pattern->len;

        while (pos + patLen - 1 <= srcLen)
        {
            subString_L(&sub, src, pos, patLen);

            if (strCompare_L(&sub, pattern) != 0)
                pos++;
            else
                return pos;
        }
    }

    return 0; // not found
}

status replace_L(lString *src, lString *pattern, lString *newStr)
{
    int index;

    if (strEmpty_L(pattern))
        return ERROR;

    index = index_L(src, pattern, 1);

    while (index)
    {
        strDelete_L(src, index, strLength_L(*pattern));
        strInsert_L(src, index, newStr);
        index += strLength_L(*newStr);
        index = index_L(src, pattern, index);
    }

    return OK;
}

status strInsert_L(lString *str, int pos, lString *newStr)
{
    chunk *tempChunk, *strChunk, *strChunkTemp, *newStrChunk;
    int i = 0, j = 0, k = 0, count = 1;
    lString tempStr;

    if (pos < 1 || pos > str->len + 1)
        return ERROR;

    initString_L(&tempStr);

    tempChunk = tempStr.head;
    strChunk = str->head;
    strChunkTemp = NULL;
    newStrChunk = newStr->head;

    while (strChunk || strChunkTemp || newStrChunk)
    {
        if (!tempChunk)
        {
            chunkAlloc(tempChunk);

            if (!tempStr.head)
                tempStr.head = tempStr.tail = tempChunk;
            else
            {
                tempStr.tail->next = tempChunk;
                tempStr.tail = tempChunk;
            }
        }

        if (strChunk)
        {
            while (strChunk && count < pos)
            {
                tempChunk->ch[i] = strChunk->ch[j];
                i = (i + 1) % CHUNKSIZE;
                j = (j + 1) % CHUNKSIZE;
                count++;

                if (!j || !(strChunk->ch[j]))
                    strChunk = strChunk->next;
                if (!i)
                {
                    tempChunk = tempChunk->next;
                    break;
                }
            }

            if (count == pos)
            {
                strChunkTemp = strChunk;
                strChunk = NULL;
            }
        }
        else if (newStrChunk)
        {
            while (newStrChunk && newStrChunk->ch[k])
            {
                tempChunk->ch[i] = newStrChunk->ch[k];
                i = (i + 1) % CHUNKSIZE;
                k = (k + 1) % CHUNKSIZE;

                if (!k || !(newStrChunk->ch[k]))
                    newStrChunk = newStrChunk->next;
                if (!i)
                {
                    tempChunk = tempChunk->next;
                    break;
                }
            }
        }
        else
        {
            while (strChunkTemp && strChunkTemp->ch[j])
            {
                tempChunk->ch[i] = strChunkTemp->ch[j];
                i = (i + 1) % CHUNKSIZE;
                j = (j + 1) % CHUNKSIZE;

                if (!j || !(strChunkTemp->ch[j]))
                    strChunkTemp = strChunkTemp->next;
                if (!i)
                {
                    tempChunk = tempChunk->next;
                    break;
                }
            }
        }
    }

    tempStr.len = str->len + newStr->len;

    count = (tempStr.len - 1) % CHUNKSIZE + 1;
    while (count < CHUNKSIZE)
    {
        tempStr.tail->ch[count] = '\0';
        count++;
    }

    clearString_L(str);

    str->len = tempStr.len;
    str->head = tempStr.head;
    str->tail = tempStr.tail;

    return OK;
}

status strDelete_L(lString *S, int pos, int len)
{
    chunk *p, *q, *r;
    int count, first, last, m, n;

    if (pos < 1 || pos > S->len || len < 0 || pos + len - 1 > S->len)
        return ERROR;

    if (pos == 1 && len == S->len)
        clearString_L(S);

    first = pos;
    last = pos + len - 1;

    for (count = 1, p = S->head; first > count * CHUNKSIZE; count++, p = p->next)
        ;

    for (q = p; last > count * CHUNKSIZE; count++, q = q->next)
        ;

    m = (first - 1) % CHUNKSIZE;
    n = (last - 1) % CHUNKSIZE;

    n = (n + 1) % CHUNKSIZE;
    if (!n)
        q = q->next;

    while (q && q->ch[n])
    {
        p->ch[m] = q->ch[n];
        m = (m + 1) % CHUNKSIZE;
        n = (n + 1) % CHUNKSIZE;

        if (!m)
            p = p->next;
        if (!n)
            q = q->next;
    }

    S->len -= len;

    for (count = 1, S->tail = S->head; S->len > count * CHUNKSIZE; count++, S->tail = S->tail->next)
        ;

    count = (S->len - 1) % CHUNKSIZE + 1;
    while (count < CHUNKSIZE)
    {
        S->tail->ch[count] = '\0';
        count++;
    }

    r = S->tail->next;
    while (r)
    {
        S->tail->next = r->next;
        free(r);
        r = S->tail->next;
    }

    return OK;
}

// void destroyString_L(lString *S)
// {
//
// }

void strPrint_L(lString S)
{
    int i = 0;
    chunk *p = S.head;

    if (S.head == NULL && S.tail == NULL && S.len == 0)
        printf("Empty String");

    while (p)
    {
        if (p->ch[i])
            printf("%c", p->ch[i]);
        i = (i + 1) % CHUNKSIZE;
        if (!i)
            p = p->next;
    }
}

status getChar_L(lString S, char *c, int i)
{
    int m, n, count;
    chunk *p;

    if (strEmpty_L(&S))
        return ERROR;

    if (i < 1 || i > S.len)
        return ERROR;

    m = i / CHUNKSIZE;
    n = i % CHUNKSIZE;
    if (n)
        m++;

    for (count = 1, p = S.head; count < m; count++)
        p = p->next;
    if (n)
        *c = p->ch[n - 1];
    else
        *c = p->ch[CHUNKSIZE - 1];

    return OK;
}

// #################################

// // create a new List node using the data called "data"
// int *create_list_node(listnode **pheader, ElemType data)
// {
//     if (list_malloc(pheader) == -1) // assign the memory needed for the node, and judge whether something wrong happened.
//         return -1;
//     memset(*pheader, 0, sizeof(listnode)); // clear the of memory
//     // node->data = 100;
//     // node->next = NULL;

//     return 0;
// }

// // allocate a new space to a list node
// int list_malloc(listnode **pnode)
// {
//     listnode *node = NULL;
//     node = (listnode *)malloc(sizeof(listnode)); // call the "malloc()" function
//     if (node == NULL)                            // if "malloc()" fails, then give the warning and return 1
//     {
//         printf("Function malloc() error.\n");
//         *pnode = NULL;
//         return -1;
//     }
//     *pnode = node;
//     return 0; // if nothing happened, return 0
// }

// // free a list node
// int free_list_node(listnode *node)
// {
//     free(node);
//     return 0;
// }

// // destroy a whloe list
// int destroy_list(listnode *header)
// {
//     listnode *node = header, *prev;
//     if (header == NULL)
//     {
//         printf("This list is empty.\n");
//         return -1;
//     }
//     while (node != NULL)
//     {
//         prev = node;
//         node = node->next;
//         free_list_node(prev);
//     }
//     return 0;
// }

// // delete a list node
// int delete_list_node(listnode *node_del, listnode *header)
// {
//     listnode *prev;
//     if (header == NULL) // when the list is empty, node won't ever be detected
//     {
//         printf("This list is empty.\n");
//         return -1;
//     }
//     prev = header;                                 // begin the search of "node_del" from "header"
//     while (prev != NULL && prev->next != node_del) // if "prev" hasn't reached the end, or the next node is not "node_del", then go on
//         prev = prev->next;
//     if (prev != NULL) // if the next node is "node_del", delete it
//     {
//         prev->next = node_del->next;
//         free(node_del);
//         return 0;
//     }
//     printf("The node to be deleted wasn't found in the list.\n");
//     return -1;
// }

// // check if a list is empty or not
// int whether_list_empty(listnode *header)
// {
//     if (header == NULL)
//         return 1; // if the list is empty, return 1
//     return 0;     // if not, return 0
// }

// // return the number of elements in a list
// int list_length(listnode *header)
// {
//     listnode *node = header;
//     int length = 0;
//     while (node != NULL) // if the node hasn't reached the end, count the length
//     {
//         node = node->next;
//         ++length;
//     }
//     return length;
// }

// // return the index-th element in a list of length bigger than index
// int list_index(listnode *header, int index)
// {
//     listnode *node;
//     // --index; // if you want to let the header be the 1st element, this line is required
//     if (header == NULL)
//     {
//         printf("This list is empty.\n");
//         return -1; // if the list is empty, return 1
//     }
//     if (index < 0)
//     {
//         printf("Index is not valid.\n");
//         return -2; // if the list is empty, return 1
//     }
//     node = header;
//     while (index && node->next != NULL) // if the node hasn't reached the end and hasn't reach the index
//     {
//         node = node->next;
//         --index;
//     }
//     if (!index) // hasn't reached the index
//     {
//         printf("Length of the list fewer than index.\n");
//         return -1;
//     }
//     return node->data;
// }

// // use function "compare()" to give the first element in list with the same data, returning the index and address
// int list_locate_elem(listnode *header, listnode **pobject, ElemType data, int *compare(ElemType a, ElemType b))
// {
//     int count = 0;
//     listnode *node;     // begin the search of "node_del" from "header"
//     if (header == NULL) // when the list is empty, node won't ever be detected
//     {
//         printf("This list is empty.\n");
//         return -1;
//     }
//     node = header;
//     while (node != NULL && compare(node->data, data) != 0) // if "prev" hasn't reached the end, or compare() gives a "unequal", then go on
//     {
//         ++count;
//         node = node->next;
//     }
//     if (node != NULL) // if compare() gives a "equal", find its address
//     {
//         *pobject = node;
//         return count;
//     }
//     printf("The node to be deleted wasn't found in the list.\n");
//     return -1;
// }

// // return the node before in the list
// int list_prev_node(listnode *header, listnode *node, listnode **pprev)
// {
//     listnode *prev;
//     if (header == NULL) // when the list is empty, node won't ever be detected
//     {
//         printf("This list is empty.\n");
//         return -1;
//     }
//     if (header == node)
//     {
//         *pprev = NULL;
//         printf("The first ndoe is that node. No previous one before it.\n");
//         return -2;
//     }
//     prev = header;
//     while (prev->next != NULL && prev->next != node) // if "prev" hasn't reached the end, and its "next" is not our target, then go on
//     {
//         prev = prev->next;
//     }
//     if (prev->next != NULL) // if hasn't reached the end
//     {
//         *pprev = prev;
//         return 0;
//     }
//     printf("No valid node found.\n");
//     return -1;
// }

// // add a new node to the list after certain node
// int list_insert_after(listnode *node, ElemType data)
// {
//     listnode *new_node;
//     if (create_list_node(new_node, data) == -1)
//     {
//         printf("Insert failed: cannot create new node\n");
//         return -1;
//     }
//     new_node->next = node->next;
//     node->next = new_node;
//     return 0;
// }

// // add a new node to the list before certain node
// int list_insert_before(listnode **pheader, listnode *node, ElemType data)
// {
//     int prev_judge;
//     listnode *prev, *new_node;
//     if ((prev_judge = list_prev_node(*pheader, node, &prev)) == -1)
//     {
//         printf("Insert failed: cannot found previous one\n");
//         return -1;
//     }
//     if (prev_judge == -2)
//     {
//         if (create_list_node(new_node, data) == -1)
//         {
//             printf("Insert failed: cannot create new node\n");
//             return -1;
//         }
//         new_node->next = node; // link 2 nodes together
//         *pheader = new_node; // change the header
//     }
//     else if (prev_judge == 0)
//         list_insert_after(prev, data);
//     return 0;
// }
