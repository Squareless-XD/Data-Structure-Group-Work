// 3.21-逆波兰式
// Description
// 假设表达式由单字母变量和双目四则运算符构成.试写一个算法,将一个通常书写形式且书写正确的表达式转换为逆波兰式.

// Input
// 常规运算表达式,比如 a+b

// Output
// 逆波兰式 ab+

// Sample Input 1 
// A+B*(C-D)-E*F
// Sample Output 1
// ABCD-*+EF*-

// Sample Input 2 
// b*c/d
// Sample Output 2
// bc*d/

#include <bits/stdc++.h>
using namespace std;

#define STACK_INIT_SIZE 100 // 存储空间初始分配量 单位：stackElemType
#define STACK_INCREMENT 10  // 存储空间分配增量
#define stackElemType opStack
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW_STACK -2

typedef int status;

typedef struct
{
    char type;
    int priority;
} opStack;

typedef struct
{
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

sqStack *stackAlloc(void);
status initStack_Sq(sqStack *stack);
status stackEmpty_Sq(sqStack *stack);
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

status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return TRUE;
    return FALSE;
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
    sqStack *stackOp;
    char c;
    stackElemType top, newTop, popElem;
    int inBracket;

    stackOp = stackAlloc();
    initStack_Sq(stackOp);
    inBracket = 0;

    while ((c = getchar()) != EOF && c != '\n')
    {
        if (isalpha(c))
            putchar(c);
        else if (c == '(')
            ++inBracket;
        else if (c == ')')
        {
            --inBracket;
            while (getTop_Sq(stackOp, &top) == OK && top.priority > inBracket)
            {
                pop_Sq(stackOp, &popElem);
                putchar(popElem.type);
            }
            if (stackEmpty_Sq(stackOp) == FALSE && top.priority == inBracket && (top.type == '*' || top.type == '/'))
            {
                pop_Sq(stackOp, &popElem);
                putchar(popElem.type);
            }
        }
        else if (c == '*' || c == '/')
        {
            if (getTop_Sq(stackOp, &top) == OK && top.priority == inBracket && (top.type == '*' || top.type == '/'))
            {
                pop_Sq(stackOp, &popElem);
                putchar(popElem.type);
            }
            newTop.priority = inBracket;
            newTop.type = c;
            push_Sq(stackOp, newTop);
        }
        else if (c == '+' || c == '-')
        {
            if (getTop_Sq(stackOp, &top) == OK && top.priority == inBracket && (top.type == '+' || top.type == '-'))
            {
                pop_Sq(stackOp, &popElem);
                putchar(popElem.type);
            }
            newTop.priority = inBracket;
            newTop.type = c;
            push_Sq(stackOp, newTop);
        }
        else
            printf("Error: unknown input character.\n");
    }
    
    while (stackEmpty_Sq(stackOp) == FALSE)
    {
        pop_Sq(stackOp, &popElem);
        putchar(popElem.type);
    }

    putchar('\n');

    return 0;
}
