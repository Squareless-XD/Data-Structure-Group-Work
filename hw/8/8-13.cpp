// 8.13-边界标志法
// Description
// 试完成边界标志法和依"首次适配策略"进行分配相应的回收释放块的算法

// Input
// 输入分为两部分，第一部分是已有的空闲块，其中每一行有三个部分，分别表示标志位，起始地址以及块空间大小
// 第二部分是待回收的释放块，每行有三个部分，分别表示标志位，起始地址以及块空间大小

// Output
// 输出回收后的空闲块状态

// Sample Input 1       Sample Output 1
// 0 10000 15000        0 10000 15000
// 0 31000 8000         0 30000 9000
// 0 59000 41000        0 40000 2000
//                      0 59000 41000
// 1 30000 1000     
// 1 40000 2000

// Sample Input 2       Sample Output 2
// 0 0 100              0 0 100
// 0 3001 4000          0 2000 1000
//                      0 3001 4000
// 1 2000 1000      

#include <bits/stdc++.h>
#include <vector>

#define TRUE 1
#define FALSE 0
#define OK 0
#define ERROR 1
#define INFEASIBLE 2

typedef int status;

typedef struct space_chunk
{
    int start;
    int size;
} space_chunk;

int main()
{
    std::vector<space_chunk> space;
    int start, size, space_num, i, j, leftAdj, rightAdj;
    char flag, c;

    // input for available space
    while (1)
    {
        if ((flag = getchar()) == '\n')
            break;
        std::cin >> start >> size;
        space.push_back({start, size});
        c = getchar();
        if (c == EOF)
            break;
    }

    space_num = space.size();

    // input for space to be released
    while (1)
    {
        // input
        if ((flag = getchar()) == EOF)
            break;
        std::cin >> start >> size;

        // initialize
        leftAdj = rightAdj = -1;

        // find if left adjacent chunk exists
        for (i = 0; i < space_num; ++i)
            if (space[i].size + space[i].start == start)
                break;
        if (i != space_num)
            leftAdj = i;

        // find if right adjacent chunk exists
        for (i = 0; i < space_num; ++i)
            if (space[i].start == start + size)
                break;
        if (i != space_num)
            rightAdj = i;

        // check each condition, and merge the spaces together
        // if no adjacent available space is found
        if (leftAdj == -1 && rightAdj == -1)
        {
            for (auto it = space.begin(); it != space.end(); ++it)
            {
                if ((*it).start > start)
                {
                    space.insert(it, {start, size});
                    break;
                }
            }
            ++space_num;
        }

        //if left side of released space has available space
        else if (rightAdj == -1)
            space[leftAdj].size += size;

        //if right side of released space has available space
        else if (leftAdj == -1)
        {
            space[rightAdj].start -= size;
            space[rightAdj].size += size;
        }

        // if both side of released space have available spaces
        else
        {
            space[leftAdj].size += (size + space[rightAdj].size);
            space.erase(space.begin() + rightAdj);
            --space_num;
        }

        c = getchar();
        if (c == EOF)
            break;
    }

    // output
    for (i = 0; i < space_num; ++i)
        std::cout << "0 " << space[i].start << ' ' << space[i].size << '\n';

    return 0;
}