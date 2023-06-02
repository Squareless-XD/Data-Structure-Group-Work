// 4-去除C程序中的注释
// Description
// C程序的注释用/*...*/来表示。请写一个程序，将输入的C程序源代码中的注释去掉，输出去掉注释之后的源代码。
// 注意：
// [1] 用于测试的C代码保证符合语法，不使用C++的//注释语法。
// [2] C语言不允许出现嵌套注释。具体来说，对于/*/**/"*/"，如果不允许嵌套注释，那么它表示字符串"*/"；如果允许嵌套注释，它表示一个引号"。
// [3] 字符串中出现的注释符/*属于字符串的一部分，注释中出现的双引号"属于注释的一部分。

// Input
// 符合语法的C代码文本文件。代码每行不超过200个字符。

// Output
// 去掉注释后的C代码。要求只能去掉注释，不可以做其他的修改，比如调整缩进，去除注释之外的换行符等。

#include <bits/stdc++.h>

#define CONTENT 0
#define COMMENT 1
#define STRING 2

int main()
{
    char c;
    int state = CONTENT;
    // state: 0 for content, 1 for comment, 2 for string
    
    // deal with the input in a while loop
    while ((c = getchar()) != EOF)
    {
        // output'\n' directly
        // if (c == '\n')
            // putchar(c);
        // if in content state
        if (state == CONTENT)
        {
            // if meet a comment, check whether to turn into comment state
            if (c == '/')
            {
                if ((c = getchar()) == '*')
                {
                    // putchar('/');
                    // putchar('*');
                    state = COMMENT;
                }
                else
                {
                    putchar('/');
                    putchar(c);
                }
            }
            // if meet a string, turn into string state
            else if (c == '"')
            {
                putchar(c);
                state = STRING;
            }
            // else output directly
            else
                putchar(c);
        }
        // if in comment state
        else if (state == COMMENT)
        {
            // if meet a comment end, check whether to turn into content state
            if (c == '*')
            {
                // putchar('*');
                if ((c = getchar()) == '/')
                {
                    // putchar(c);
                    state = CONTENT;
                }
                else
                {
                    // putchar(c);
                    ungetc(c, stdin);
                }
            }
        }
        else if (state == STRING)
        {
            if (c == '\\')
            {
                putchar('\\');
                putchar(getchar());
            }
            else if (c == '"')
            {
                putchar('"');
                state = CONTENT;
            }
            else
                putchar(c);
        }
    }

    return 0;
}

