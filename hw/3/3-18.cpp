// 3.18-括号配对
// Description
// 试写一个判别表达式中开,闭括号是否配对出现的算法。
// 比如:'(a+b(c-d))'是配对出现的,而'(d-e))'不是配对出现的。

// Input
// 表达式,如:'a+b(c-d)’

// Output
// 表达式中的括号是否配对出现,是则输出1,否则输出0

// Sample Input 1
// (a+b(c-d))
// Sample Output 1
// 1

// Sample Input 2
// (d-e))
// Sample Output 2
// 0

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
status stackEmpty_Sq(sqStack *stack);
// status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);
// status bracketPair(char open, char close);

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

status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return TRUE;
    return FALSE;
}

// status getTop_Sq(sqStack *stack, stackElemType *e)
// {
//     if (stack->top == stack->base)
//         return ERROR;
//     *e = *(stack->top - 1);
//     return OK;
// }

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

// status bracketPair(char open, char close)
// {
//     if (open == '(' && close == ')')
//         return TRUE;
//     if (open == '[' && close == ']')
//         return TRUE;
//     if (open == '{' && close == '}')
//         return TRUE;
//     return FALSE;
// }

int main()
{
    sqStack *stack;
    char c, topTemp;

    stack = stackAlloc();
    initStack_Sq(stack);

    while ((c = getchar()) != EOF && c != '\n')
    {
        if (c == '(')
            push_Sq(stack, c);
        else if (c == ')')
        {
            if (pop_Sq(stack, &topTemp) == ERROR)
            {
                printf("0\n");
                return 0;
            }
        }
    }
    if (stackEmpty_Sq(stack) == TRUE)
        printf("1\n");
    else
        printf("0\n");
    return 0;
}

// a proper algorithm for more kind of brackets
// int main()
// {
//     sqStack *stack;
//     char c, topTemp;

//     stack = stackAlloc();
//     initStack_Sq(stack);

//     while ((c = getchar()) != EOF && c != '\n')
//     {
//         if (c == '(' || c == '[' || c == '{')
//             push_Sq(stack, c);
//         else if (c == ')' || c == ']' || c == '}')
//         {
//             if (getTop_Sq(stack, &topTemp) == ERROR)
//             {
//                 printf("0\n");
//                 return 0;
//             }
//             if (bracketPair(topTemp, c) == TRUE)
//                 pop_Sq(stack, &topTemp);
//         }
//     }
//     if (stackEmpty_Sq(stack) == TRUE)
//         printf("1\n");
//     else
//         printf("0\n");
//     return 0;
// }
