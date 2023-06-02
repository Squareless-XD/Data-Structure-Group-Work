/*
Description

写一个算法，自大到小依次输出顺序读入的三个整数，x，y和z的值


Input
输入x，y和z


Output
输出自大到小的排序
*/

#include <iostream>
#include <algorithm>
using namespace std;

int main()
{
    long long a[3];
    cin >> a[0] >> a[1] >> a[2];
    sort(a, a + 3);
    cout << a[2] << " " << a[1] << " " << a[0] << endl;
    
    return 0;
}