#include <bits/stdc++.h>
using namespace std;

#define MAXSTRLEN 255

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW_STRING -2

typedef int status;
typedef unsigned char sString[MAXSTRLEN + 1];

status strInitAssign(sString str);
status strCopy(sString strSrc, sString strDst);
int strLength(sString str);
status strEmpty(sString str);
status strCompare(sString str1, sString str2);
int charCmp(char former, char latter);
status subString(sString str, sString sub, int pos, int len);
void get_nextval(sString pattern, int *nextval);
int strIndexKMP(sString str, sString pattern, int pos);
status strAssign(sString str, char *charStr);
status strCat(sString str, sString substr1, sString substr2);
status clearStr(sString str);
status repalceStr(sString str, sString pattern, sString replace);
status strDelete(sString str, int pos, int len);
status strInsert(sString str, sString sub, int pos);
status strAppend(sString str, char newChar);

status strInitAssign(sString str)
{
    str[0] = 0;
    return OK;
}

status strCopy(sString strSrc, sString strDst)
{
    if (strSrc == strDst)
        return ERROR;
    memcpy(strDst, strSrc, sizeof(sString));
    return OK;
}

int strLength(sString str)
{
    return (int)str[0];
}

status strEmpty(sString str)
{
    if (str[0] == 0)
        return TRUE;
    return FALSE;
}

// return    1<2: -?   1=2: 0   1>2: ?   Error: -2
status strCompare(sString str1, sString str2)
{
    if (str1 == str2)
        return 0;
    int strLen1 = strLength(str1), strLen2 = strLength(str2);
    int minStrLen = min(strLen1, strLen2);
    int index = 1;
    while (minStrLen--)
    {
        if (charCmp(str1[index], str2[index]) != 0)
            return charCmp(str1[index], str2[index]);
        ++index;
    }
    if (!minStrLen && strLen1 == strLen2)
        return 0;
    return ERROR; // Error
}

// self defined compare function of 'char's
int charCmp(char former, char latter)
{
    if (former < latter)
        return -1;
    else if (former > latter)
        return 1;
    return 0;
}

// note: the first char in string is called the 0th
status subString(sString str, sString sub, int pos, int len)
{
    int index;

    if (strLength(str) < pos + len - 1 || len < 1 || pos < 1)
        return ERROR;
    sub[0] = len;
    for (index = 0; index < len; ++index)
        sub[1 + index] = str[pos + index];
    return OK;
}

// a better algorithm, quickening the calculation
void get_nextval(sString pattern, int *nextval)
{
    int posNow = 1, posBack = 0; // "now" is where we are dealing with. "back" is which "now" matches
    int patternLen = strLength(pattern);

    int count = 0;

    nextval[1] = 0;

    // run for every char in the array "nextval"
    while (posNow < patternLen)
    {
        if (posBack == 0 || pattern[posNow] == pattern[posBack]) // if 0, go 1; if the same, go to the next
        {
            ++posNow;
            ++posBack;
            if (pattern[posNow] != pattern[posBack])
                nextval[posNow] = posBack;
            else
                nextval[posNow] = nextval[posBack]; // change is here. if the smae, we do not need to match it for another time
        }
        else
            posBack = nextval[posBack]; // if not the same, let the "back" go backward to match another sequence
    }
    while (count++ < patternLen)
        cout << nextval[count] << ' ';
    cout << endl;
}

// use KMP algorithm to find a substring in "str", beginning at "str[pos]"
int strIndexKMP(sString str, sString pattern, int pos)
{
    if (str == pattern)
        return 1;

    int nextval[pattern[0] + 1];
    int indexStr = pos;
    int indexPat = 1;

    get_nextval(pattern, nextval);

    while (indexStr <= str[0] && indexPat <= pattern[0])
    {
        if (indexPat == 0 || str[indexStr] == pattern[indexPat])
        {
            ++indexStr;
            ++indexPat;
        }
        else
            indexPat = nextval[indexPat];
    }
    if (indexPat > pattern[0])
        return indexStr - pattern[0];
    return 0;
}

status strAssign(sString str, char *charStr)
{
    int len = 0;
    char *ptrCharStr = charStr;

    while (*ptrCharStr != '\0' && len < MAXSTRLEN)
        str[++len] = *(ptrCharStr++);
    if (*ptrCharStr == '\0')
    {
        str[0] = len;
        return OK;
    }
    return ERROR;
}

status strCat(sString str, sString substr1, sString substr2)
{
    int len1 = strLength(substr1);
    int len2 = strLength(substr2);
    int index;

    if (len1 + len2 > MAXSTRLEN)
        return ERROR;
    str[0] = index = len1 + len2;
    while (index > len1)
    {
        str[index] = substr2[index - len1];
        --index;
    }
    while (index)
    {
        str[index] = substr1[index];
        --index;
    }
    return OK;
}

status clearStr(sString str)
{
    str[0] = 0;
    return OK;
}

status repalceStr(sString str, sString pattern, sString replace)
{
    int index;

    while ((index = strIndexKMP(str, pattern, index)) != 0)
    {
        strDelete(str, index, pattern[0]);
        if (strInsert(str, replace, index) == ERROR)
            return ERROR;
        index += replace[0];
    }

    return OK;
}

status strDelete(sString str, int pos, int len)
{
    int index;

    if (str[0] < pos + len - 1 || len < 1 || pos < 1)
        return ERROR;

    str[0] -= len;
    for (index = pos; index <= str[0]; ++index)
        str[index] = str[index + len];

    return OK;
}

status strInsert(sString str, sString sub, int pos)
{
    if (str == sub)
    {
        sString temp;
        strCopy(sub, temp);
        return strInsert(str, temp, pos);
    }

    int index, strLen = strLength(str), subLen = strLength(sub);

    if (strLen + subLen > MAXSTRLEN || subLen < 1 || pos < 1 || pos > strLen + 1)
        return ERROR;

    str[0] += subLen;
    for (index = strLen; index >= pos; --index)
        str[index + subLen] = str[index];
    while (subLen--)
        str[pos + subLen] = sub[1 + subLen];

    return OK;
}

status strAppend(sString str, char newChar)
{
    if (str[0] == MAXSTRLEN)
        return ERROR;

    ++(str[0]);
    str[str[0]] = newChar;
    return OK;
}

int main()
{
    sString str, pattern;
    int result;
    char c;

    strInitAssign(str);
    strInitAssign(pattern);

    while ((c = getchar()) != '\n' && c != EOF)
        strAppend(str, c);

    while ((c = getchar()) != '\n' && c != EOF)
        strAppend(pattern, c);

    cout << strLength(str) << ' ' << strLength(pattern) << endl;
    result = strIndexKMP(str, pattern, 1);

    if (result > 0)
        cout << "OK! pos:" << result << endl;
    else
        cout << "NOT FOUND!" << endl;


    // halfLen = len / 2;
    // for (i = 0; i < halfLen; ++i)
    // {
    //     if (str[i + 1] != str[len - i])
    //     {
    //         cout << '0' << endl;
    //         return 0;
    //     }
    // }
    // cout << '1' << endl;


    return 0;
}