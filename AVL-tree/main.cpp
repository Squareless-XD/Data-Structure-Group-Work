#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include"AVLTree.h"


int main()
{
	AVLTree T1;
	AVLTree T2;
	int t;
	int cho;
	int key;
	bool taller =0;
	bool shorter=0;
	T1 = (AVLTree)malloc(sizeof(AVLTreeNode));
	T1 = NULL;
	T2 = (AVLTree)malloc(sizeof(AVLTreeNode));
	T2 = NULL;
	while (1)
	{
		printf("**********平衡二叉树的操作**********\n");
		printf("*******平衡二叉树凹入表显示区*******\n");
		if (T1 == NULL)
			printf("T1现在是空树\n");
		else {
			printf("T1:\n");
			PrintAVL(T1, 1);
		}
		if (T2 == NULL)
			printf("T2现在是空树\n");
		else {
			printf("T2:\n");
			PrintAVL(T2, 1);
		}
			
		printf("\n**********输入你想要的操作**********\n");
		printf("|               1.查找             |\n");
		printf("|               2.插入             |\n");
		printf("|               3.删除             |\n");
		printf("|               4.合并             |\n");
		printf("|               5.分裂             |\n");
		printf("|               7.退出             |\n");
		printf("************************************\n");
		printf("输入你想要的操作：\n");
		scanf("%d", &cho);
		fflush(stdin);
		switch (cho) {
		case 1:
			printf("请输入想要查找的树：");
			scanf("%d", &t);
			fflush(stdin);
			printf("请输入要查找的关键字：");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1)
			{
				if (SearchAVL(T1, key))
				printf("查找成功\n");
			else
				printf("查找失败\n");
			}
			else
			{
				if (SearchAVL(T2, key))
					printf("查找成功\n");
				else
					printf("查找失败\n");
			}
			break;
		case 2:
			printf("请输入想要插入的树：");
			scanf("%d", &t);
			fflush(stdin);
			printf("请输入要插入的值：");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1) {
				if (InsertAVL(T1, key, taller) == 1)
				printf("成功插入了一个节点\n");
			else
				printf("插入失败\n");
			}
			else {
				if (InsertAVL(T2, key, taller) == 1)
					printf("成功插入了一个节点\n");
				else
					printf("插入失败\n");
			}
			break;
		case 3:
			printf("请输入想要删除的树：");
			scanf("%d", &t);
			fflush(stdin);
			printf("请输入要删除的关键字：");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1) {
if (DeleteAVL(T1, key, shorter))
				printf("删除成功！\n");
			else
				printf("删除失败！\n");
			}
			else {
				if (DeleteAVL(T2, key, shorter))
					printf("删除成功！\n");
				else
					printf("删除失败！\n");
			}
			break;
		case 4:
			MergeAVL(T1, T2);
			T2 = NULL;
			break;
		case 5:
			printf("请输入要分裂的树：");
			scanf("%d", &t);
			fflush(stdin);
			printf("请输入要分裂的中间值：");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1) {
				Split(T1, key, T1, T2);
			}
			else {
				Split(T2, key, T1, T2);
			}
			break;
		case 7:
			return 0;
		}
		printf("按任意键返回\n");
		getchar();
	}
}