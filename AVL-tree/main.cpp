#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "AVLTree.h"

int main()
{
	AVLTree *T1;
	AVLTree *T2;
	int t;
	int cho;
	int key;
	bool taller = 0;
	bool shorter = 0;
	T1 = (AVLTree *)malloc(sizeof(AVLTree));
	T1 = NULL;
	T2 = (AVLTree *)malloc(sizeof(AVLTree));
	T2 = NULL;
	while (1)
	{
		printf("**********ƽ��������Ĳ���**********\n");
		printf("*******ƽ��������������ʾ��*******\n");
		if (T1 == NULL)
			printf("T1�����ǿ���\n");
		else
		{
			printf("T1:\n");
			PrintAVL(T1, 1);
		}
		if (T2 == NULL)
			printf("T2�����ǿ���\n");
		else
		{
			printf("T2:\n");
			PrintAVL(T2, 1);
		}

		printf("\n**********��������Ҫ�Ĳ���**********\n");
		printf("|               1.����             |\n");
		printf("|               2.����             |\n");
		printf("|               3.ɾ��             |\n");
		printf("|               4.�ϲ�             |\n");
		printf("|               5.����             |\n");
		printf("|               7.�˳�             |\n");
		printf("************************************\n");
		printf("��������Ҫ�Ĳ�����\n");
		scanf("%d", &cho);
		fflush(stdin);
		switch (cho)
		{
		case 1:
			printf("��������Ҫ���ҵ�����");
			scanf("%d", &t);
			fflush(stdin);
			printf("������Ҫ���ҵĹؼ��֣�");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1)
			{
				if (SearchAVL(T1, key))
					printf("���ҳɹ�\n");
				else
					printf("����ʧ��\n");
			}
			else
			{
				if (SearchAVL(T2, key))
					printf("���ҳɹ�\n");
				else
					printf("����ʧ��\n");
			}
			break;
		case 2:
			printf("��������Ҫ���������");
			scanf("%d", &t);
			fflush(stdin);
			printf("������Ҫ�����ֵ��");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1)
			{
				if (InsertAVL(T1, key, taller) == 1)
					printf("�ɹ�������һ���ڵ�\n");
				else
					printf("����ʧ��\n");
			}
			else
			{
				if (InsertAVL(T2, key, taller) == 1)
					printf("�ɹ�������һ���ڵ�\n");
				else
					printf("����ʧ��\n");
			}
			break;
		case 3:
			printf("��������Ҫ���������");
			scanf("%d", &t);
			fflush(stdin);
			printf("������Ҫɾ���Ĺؼ��֣�");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1)
			{
				if (DeleteAVL(T1, key, shorter))
					printf("ɾ���ɹ���\n");
				else
					printf("ɾ��ʧ�ܣ�\n");
			}
			else
			{
				if (DeleteAVL(T2, key, shorter))
					printf("ɾ���ɹ���\n");
				else
					printf("ɾ��ʧ�ܣ�\n");
			}
			break;
		case 4:
			MergeAVL(T1, T2);
			T2 = NULL;
			break;
		case 5:
			printf("������Ҫ���ѵ�����");
			scanf("%d", &t);
			fflush(stdin);
			printf("������Ҫ���ѵ��м�ֵ��");
			scanf("%d", &key);
			fflush(stdin);
			if (t == 1)
			{
				Split(T1, key, T1, T2);
			}
			else
			{
				Split(T2, key, T1, T2);
			}
			break;
		case 7:
			return 0;
		}
		printf("�����������\n");
		getchar();
	}
}