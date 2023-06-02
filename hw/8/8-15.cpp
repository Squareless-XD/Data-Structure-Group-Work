// 8.15-可利用空间表
// Description
// 设被管理空间的上下界地址分别由变量highbound和lowbound给出，形成一个由同样大小的块组成的"堆"。试写一个算法，将所有tag域为0的块按始址递增顺序链接成一个可利用空间表。

// Input
// 输入分为两行，第一行为管理空间的上下界变量的highbound，lowbound以及每个块的大小，第二行为每个块的tag

// Output
// 输出为可利用空间表，每一行代表每个元素，每个元素包含三个数字，分别代表tag，起始地址和空间大小

// Sample Input 1           Sample Output 1
// 0 20 2                   0 0 4
// 0 0 1 1 0 0 1 0 1 1      0 8 4
//                          0 14 2

// Sample Input 2           Sample Output 2
// 30 40 1                  0 0 0
// 1 1 1 1 1 1 1 1 1 1

// Sample Input 3           Sample Output 3
// 10 25 3                  0 10 12
// 0 0 0 0 1

#include <bits/stdc++.h>
#include <vector>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2

#define DEFAULT_SIZE 10
#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef int status;

typedef struct
{
    int size;
    int start;
} spaceSys;


int main()
{
    std::vector<spaceSys> buddySys;
    int begin, end, size, num, flag, lastFlag, i, j;
    char c;

    // input for available space
    std::cin >> begin >> end >> size;
    num = (end - begin) / size;

    j = 0;
    lastFlag = 1;
    for (i = 0; i < num; ++i)
    {
        std::cin >> flag;
        if (flag == 0)
        {
            if (lastFlag == 1)
            {
                buddySys.push_back({size, begin});
                lastFlag = 0;
            }
            else
                buddySys[j].size += size;
        }
        else
        {
            if (lastFlag == 0)
            {
                lastFlag = 1;
                ++j;
            }
        }
        begin += size;
    }


    // output
    for (int i = 0; i < buddySys.size(); i++)
        std::cout << "0 " << buddySys[i].start << ' ' << buddySys[i].size << std::endl;
    if (buddySys.size() == 0)
        std::cout << "0 0 0" << std::endl;
    return 0;
}