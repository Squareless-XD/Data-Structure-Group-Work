// // Description
// // 试编写算法，计算 i!\cdot 2^i (i=0,1,…,n−1)的值并分别存入数组a[arrsize]的各个分量中。
// // 假设计算机中允许的整数最大值为MAXINT=2^32−1, 则当n>arrsize 或对某个 k(0≤k≤n−1)使 k!⋅2^k>MAXINT 时，应按出错处理。

// // Input
// // 输入为两个数，用空格隔开，分别表示 n 和 arrsize。其中 0 < n, arrsize ≤ 20

// // Output
// // * 若没有发生出错，则依次输出 i!⋅2^i (i=0,1,…,n−1) 的每个分量，用空格隔开。
// // * 若发生出错，则直接输出-1。

// #include <iostream>
// #include <cmath>
// // #include <ctime>
// using namespace std;
// #define MAXINT 4294967295

// int main()
// {
//     long long a_temp;
//     long long i, n, arrsize;
//     long long a[20];

//     a_temp = 1;

//     cin >> n >> arrsize;
//     // arrsize = min(arrsize, 25);
//     // const clock_t begin_time = clock();
//     a[0] = 1;

//     if (n > arrsize || n < 1 || arrsize > 20)
//     {
//         cout << "-1" << endl;
//         // float seconds = float(clock() - begin_time) / 1000; // 最小精度到ms
//         // cout << seconds << " seconds" << endl;
//         return -1;
//     }
//     for (i = 1; i < n; ++i)
//     {
//         if (MAXINT / a_temp < 2 * i)
//         {
//             cout << "-1" << endl;
//             // float seconds = float(clock() - begin_time) / 1000; // 最小精度到ms
//             // cout << seconds << " seconds" << endl;
//             return -1;
//         }
//         a_temp *= (2 * i);
//         a[i] = a_temp;
//     }
//     for (i = 0; i < n - 1; ++i)
//         cout << a[i] << " ";
//     cout << a[n - 1] << endl;
//     // float seconds = float(clock() - begin_time) / 1000; // 最小精度到ms
//     // cout << seconds << " seconds" << endl;
//     return 0;
// }

#include <iostream>
#include <cmath>
using namespace std;
#define MAXINT 4294967295

int main()
{
    unsigned int a_temp;
    unsigned int i, n, arrsize;
    unsigned int a[20];

    a_temp = 1;

    cin >> n >> arrsize;
    a[0] = 1;

    if (n > arrsize || n < 1 || arrsize > 20)
    {
        cout << "-1" << endl;
        return 0;
    }
    for (i = 1; i < n; ++i)
    {
        if (((unsigned int)MAXINT / a_temp) < (2 * i))
        {
            cout << "-1" << endl;
            return 0;
        }
        a_temp *= (2 * i);
        a[i] = a_temp;
    }
    for (i = 0; i < n - 1; ++i)
        cout << a[i] << " ";
    cout << a[n - 1] << endl;
    return 0;
}

