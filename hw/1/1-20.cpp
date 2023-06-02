// 求一元多项式的值

#include <iostream>
#include <bits/stdc++.h>
using namespace std;

int main()
{
    int x0, n, i, temp;
    vector<int> a(n, 0);
    cin >> x0 >> n;
    for (i = 0; i <= n; ++i)
    {
        cin >> a[i];
    }

    temp = a[n];
    while (n > 0)
    {
        temp *= x0;
        temp += a[--n];
    }
    cout << temp << endl;
}