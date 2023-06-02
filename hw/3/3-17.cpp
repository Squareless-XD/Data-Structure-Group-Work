// 3.17-逆序列
// Description
// 试写一算法，识别依次读入的一个以@为结束符的字符序列是否为形如“序列1&序列2”模式的字符序列。其中序列1和序列2中都不包含字符“&”，且序列2是序列1的逆序列。例如，“a+b&b+a”是属于该模式的字符序列，而“1+3&3-1”则不是。

// Input
// 输入字符序列

// Output
// 如果序列符合模式则输出1,不符合则输出0

// Sample Input 1 
// a+b&b+a@
// Sample Output 1
// 1


#include <bits/stdc++.h>
using namespace std;

int g(int m, int n);

int main()
{
    int m, n;

    scanf("%d,%d", &m, &n);
    printf("%d\n", g(m, n));

    return 0;
}

int g(int m, int n)
{
    if (n < 0)
        return -1;
    if (m == 0)
        return 0;
    return g(m - 1, 2 * n) + n;
}
