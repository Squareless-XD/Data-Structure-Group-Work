// 顺序表比较
// Input
// 输入为两行，分别代表A和B中的元素，以逗号分开。每个顺序表最多100个元素。

// Output
// 输出A和B的比较结果，0代表A=B，1代表A<B，2代表A>B

#include <bits/stdc++.h>
using namespace std;
int main()
{
    vector<char> a;
    vector<char> b;
    char c;
    int length_a, length_b, i, length;

    while ((c = getchar()) != '\n' && c != EOF)
        if (c != ',')
            a.push_back(c);

    while ((c = getchar()) != '\n' && c != EOF)
        if (c != ',')
            b.push_back(c);

    length_a = a.size();
    length_b = b.size();
    length = min(length_a, length_b);
    for (i = 0; i < length; ++i)
    {
        if (a[i] < b[i])
        {
            cout << '1' << endl;
            return 0;
        }
        else if (a[i] > b[i])
        {
            cout << '2' << endl;
            return 0;
        }
    }
    if (length_a == length_b)
        cout << '0' << endl;
    else if (length_a < length_b)
        cout << '1' << endl;
    else
        cout << '2' << endl;
    return 0;
}