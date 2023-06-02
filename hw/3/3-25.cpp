// 3.25-递归函数非递归法计算
// Description
// 试写出求递归函数Ｆ(n)的递归算法，并消除递归:
// F(n ) = n + 1 (n = 0)
// F(n) = n * F(n / 2) (n > 0)

// Input
// 输入非负整数ｎ

// Output
// 计算出的F(n)函数的值

// Sample Input 1 
// 10
// Sample Output 1
// 100


#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量
#define stackElemType int
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
    sqStack *stack;
    char c;
    int n, length, index, functionValue, topElem;
    
    stack = stackAlloc();
    initStack_Sq(stack);

    cin >> n;

    while (n)
    {
        push_Sq(stack, n);
        n /= 2;
    }
    length = stackLength_Sq(stack);

    functionValue = 1;
    for (index = 0; index < length; ++index)
    {
        pop_Sq(stack, &topElem);
        functionValue *= topElem;
    }
    cout << functionValue << endl;

    return 0;
}


