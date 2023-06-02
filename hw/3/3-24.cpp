// 3.24-递归函数计算
// Description
// 试编写如下定义的递归函数的递归算法，并根据算法画出求ｇ(5, 2)时栈的变化过程。
// g(m, n) = 0 (m = 0, n>=0)   g(m, n) = g(m-1, 2n) + n (m>0, n>=0)

// Input
// 输入ｍ与n的数值，中间以逗号隔开

// Output
// 递归计算后，g(m, n)的值

// Sample Input 1
// 5,2
// Sample Output 1
// 62

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
status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);

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
    sqStack *stack;
    char c, topTemp;

    stack = stackAlloc();
    initStack_Sq(stack);

    while ((c = getchar()) != '&' && c != EOF && c != '\n')
        push_Sq(stack, c);
    while ((c = getchar()) != '@' && c != EOF && c != '\n')
    {
        getTop_Sq(stack, &topTemp);
        if (topTemp != c)
            break;
        pop_Sq(stack, &c);
    }
    if (c == '@')
        printf("1\n");
    else
        printf("0\n");
    return 0;
}
