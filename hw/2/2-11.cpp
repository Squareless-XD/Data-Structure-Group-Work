// Description
// 设顺序表va中的数据为递增有序。写一个算法，将x插入到顺序表的适当位置，保证有序性。

// Input
// 输入分为两行，第一行是va顺序表，每个元素按空格分隔，第二行是x的值。
// 顺序表中的元素最多为100个，所有元素的值均大于0，元素为整型。

// Output
// 输出插入x后，va的结果

// Sample Input 1
// 1 3 4 6 45 56
// 20

// Sample Output 1
// 1 3 4 6 20 45 56

#include <bits/stdc++.h>
using namespace std;

int main()
{
    int a, va[110], count = 0, i, j, x;
    char c;

    do
    {
        scanf("%d%c", &a, &c);
        va[count] = a;
        ++count;
    } while (c != '\n'); // until the next line is comming

    scanf("%d", &x);

    for (i = 0; i < count; ++i)
    {
        if (x <= va[i])
        {
            for (j = count; j > i; --j)
                va[j] = va[j - 1];
            va[i] = x;
            break;
        }
    }
    if(i == count)
        va[count] = x;

    for (i = 0; i < count; ++i)
        cout << va[i] << " ";
    cout << va[count] << endl;

    return 0;
}
