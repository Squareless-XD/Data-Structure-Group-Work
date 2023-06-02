// Description

// 假设有A,B,C,D,E 五个高等院校进行田径对抗赛，各院校的单项成绩均已存入计算机，并构成一张表，表中每一行的形式为
// 项目名称 - 性别 - 校名 - 成绩
// 编写算法，处理上述表格，以统计各院校的男、女总分和团体总分，并输出

// Input
// 输入包含多行，每行代表表格中的一行，每个字段以空格分隔
// M代表男生，F代表女生

// Output
// 输出每个学校的男，女总分和团体总分，按如下顺序
// 第一行输出学校A的男生总分
// 第二行输出学校A的女生总分
// 第三行输出学校A的团体总分
// 第四行输出学校B的男生总分
// ...
// 如果某个学校没有男生或女生，则跳过对应的行数（如学校A没有女生，则跳过第二行）

// Sample Input 1 
// X M A 3
// Z M A 4
// Y F B 30
// Sample Output 1
// A M 7
// A 7
// B F 30
// B 30


#include <bits/stdc++.h>
using namespace std;

int main()
{
    string game = "", game_former;
    char gender, school;
    int grade;
    int grade_total[5][2]; // [5] represents 5 schools, [2] represents 2 genders
    int school_choose, gender_choose; // two indexes of grade_total
    int i, j;
    memset(grade_total, 0, sizeof(int [5][2]));

    while(cin >> game)
    {
        cin >> gender >> school >> grade;
        if (game == game_former || game == "")
            break;
        school_choose = school - 'A';
        gender_choose = (gender == 'M') ? 0 : 1;
        grade_total[school_choose][gender_choose] += grade; // add the grade to the corresponding variable
    }

    for (i = 0; i < 5; ++i)
    {
        for (j = 0; j < 2; ++j)
        {
            if (grade_total[i][j] != 0)
                cout << (char)(i + 'A') << " " << ((j == 0) ? 'M' : 'F') << " " << grade_total[i][j] << endl;
        }
        if ((grade_total[i][0] + grade_total[i][1]) != 0)
        cout << (char)(i + 'A') << " " << (grade_total[i][0] + grade_total[i][1]) << endl;
    }

    return 0;
}