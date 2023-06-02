#include <bits/stdc++.h>
#include <vector>
using namespace std;

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE -1

typedef int status;

int main()
{
    vector<int> L, R, tempVec;
    int d, elemNum, i;
    char c;

    while (scanf("%d%c", &d, &c), c == ' ')
        L.push_back(d);
    L.push_back(d);

    while (scanf("%d%c", &d, &c), c == ' ')
        R.push_back(d);
    R.push_back(d);

    tempVec = R;
    R = L;
    L = tempVec;

    elemNum = R.size();

    for (i = 0; i < elemNum - 1; ++i)
        cout << L[i] << ' ';
    cout << L[i] << endl;

    for (i = 0; i < elemNum - 1; ++i)
        cout << R[i] << ' ';
    cout << R[i] << endl;


    return 0;
}
