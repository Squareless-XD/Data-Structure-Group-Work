#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量
#define stackElemType char
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW_STACK -2

typedef int status;

typedef struct
{
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

sqStack *stackAlloc(void);
status initStack_Sq(sqStack *stack);
status clearStack_Sq(sqStack *stack);
status stackEmpty_Sq(sqStack *stack);
int stackLength_Sq(sqStack *stack);
status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);
// status bracketPair(char open, char close);
status judgePlalindrome(sqStack *stackInput, sqStack *stackCmp);

sqStack *stackAlloc(void)
{
    sqStack *stack;
    stack = (sqStack *)malloc(sizeof(sqStack));
    if (!stack)
        return NULL;
    return stack;
}

status initStack_Sq(sqStack *stack)
{
    if (!stack)
        exit(ERROR);
    stack->base = (stackElemType *)malloc(STACK_INIT_SIZE * sizeof(stackElemType));
    if (!stack->base)
        exit(ERROR);
    stack->top = stack->base;
    stack->stacksize = STACK_INIT_SIZE;
    return OK;
}

status clearStack_Sq(sqStack *stack)
{
    stack->top = stack->base;
    memset(stack, 0, sizeof(sqStack));
    return OK;
}

status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return TRUE;
    return FALSE;
}

int stackLength_Sq(sqStack *stack)
{
    return stack->top - stack->base;
}

status getTop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *(stack->top - 1);
    return OK;
}

status push_Sq(sqStack *stack, stackElemType e)
{
    if (stack->top - stack->base >= stack->stacksize)
    {
        stack->base = (stackElemType *)realloc(stack->base, (stack->stacksize + STACK_INCREMENT) * sizeof(stackElemType));
        if (!stack->base)
            exit(OVERFLOW_STACK);
        stack->top = stack->base + stack->stacksize;
        stack->stacksize += STACK_INCREMENT;
    }
    *(stack->top++) = e;
    return OK;
}

status pop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *--stack->top;
    return OK;
}

int main()
{
    sqStack *stackInput, *stackCmp;
    char c = '0';
    status judgeSign;
    
    stackInput = stackAlloc();
    initStack_Sq(stackInput);
    stackCmp = stackAlloc();
    initStack_Sq(stackCmp);

    while (c != EOF)
    {
        while ((c = getchar()) != '@' && c != '\n' && c != EOF)
            push_Sq(stackInput, c);
        if (c == EOF)
            break;
        if (c == '\n')
        {
            clearStack_Sq(stackInput);
            clearStack_Sq(stackCmp);
            continue;
        }
        judgeSign = judgePlalindrome(stackInput, stackCmp);
        if (judgeSign == TRUE)
            printf("1\n");
        else
            printf("0\n");
    }

    return 0;
}

status judgePlalindrome(sqStack *stackInput, sqStack *stackCmp)
{
    char c;
    stackElemType topElem, cmpElem;
    int length, index;

    length = stackLength_Sq(stackInput);
    // cout << "1 " << length << endl;
    for (index = length / 2; index > 0; --index)
    {
        // cout << index << endl;
        pop_Sq(stackInput, &topElem);
        // cout << "OK" << endl;
        push_Sq(stackCmp, topElem);
    }
    // cout << '2' << endl;
    if (length % 2 == 1)
    {
        pop_Sq(stackInput, &topElem);
    }
    // cout << '3' << endl;
    for (index = length / 2; index > 0; --index)
    {
        pop_Sq(stackInput, &topElem);
        pop_Sq(stackCmp, &cmpElem);
        if (topElem != cmpElem)
            return FALSE;
    }
    // cout << '4' << endl;
    if (stackEmpty_Sq(stackInput) == TRUE)
    {
        return TRUE;
    }
    return FALSE;
}
