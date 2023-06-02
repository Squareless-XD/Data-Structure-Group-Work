#include <bits/stdc++.h>
using namespace std;

#define MAXSTRLEN 255

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_STRING -2

typedef int status;
typedef unsigned char sString[MAXSTRLEN + 1];

int main()
{
    unsigned long a, b;
    a = 532664516;
    b = 5287727;

    printf("%lu\n", a / b);
    printf("%u\n", (unsigned)(a / b));

    return 0;
}