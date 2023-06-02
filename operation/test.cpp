#include <bits/stdc++.h>
using namespace std;

int main()
{
    int *a;
    a = (int *)malloc(sizeof(int));
    *a = 1;
    free(a);

    int *b;
    free(b);
    cout << '1' << endl;
    return 0;
}