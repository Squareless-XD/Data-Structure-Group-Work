// Description

// 已知k阶斐波那契序列的定义为
// ......
// 试编写求k阶裴波那契序列的第m项值的函数算法，k和m均以值调用的形式在函数参数表中出现。

// Input
// 输入为k和m（m从0开始，f_0对应m=0）

// Output
// 输出第m项的值

#include <bits/stdc++.h>
using namespace std;

vector<int> fibo_mem; // set a vector to memorize fibonacci terms from 1 to higher

int main(int argc, char *argv[])
{
    int m, k, i, j; // calculate the m-th term
    cin >> k >> m;
    fibo_mem = vector<int>(m + 1, 0); // initialize as a vector consisting of m+1 "-1"s
    fibo_mem[k - 1] = 1;              // the first two terms are 0
    for (i = k; i <= m; ++i)
        for (j = 1; j <= k; ++j)
            fibo_mem[i] += fibo_mem[i - j]; // for each term, add up k terms before it
    std::cout << fibo_mem[m] << endl;
    return 0;
}
