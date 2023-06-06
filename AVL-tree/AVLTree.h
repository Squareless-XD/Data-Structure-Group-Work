#include<stdio.h>
#include<stdlib.h>
#define LH +1
#define EH 0
#define RH -1
#define true 1
#define false 0
typedef struct AVLTreeNode {
	int data;
	int depth;
	int bf; //balance factor
	struct AVLTreeNode* lchild;
	struct AVLTreeNode* rchild;
}*AVLTree;

//create a AVLTree which is empty;
AVLTree CreateAVLTree()
{
	AVLTree a;
	a = (AVLTree)malloc(sizeof(AVLTreeNode));
	a->bf = EH;
	a->depth = 0;
	a->data = 0;
	a->lchild = NULL;
	a->rchild = NULL;
	return a;
}

void R_Rotate(AVLTree &p)
{
	AVLTree lc;
	lc= p->lchild;
	p->lchild = lc->rchild;
	lc->rchild = p;
	p = lc;
}

void L_Rotate(AVLTree &p)
{
	AVLTree rc;
	rc = p->rchild;
	p->rchild = rc->lchild;
	rc->lchild = p;
	p = rc;
}

void LeftBalance(AVLTree &T)
{
	AVLTree lc;
	AVLTree rd;
	lc = T->lchild;
	switch (lc->bf) {
	case LH:
		T->bf = lc->bf = EH;
		R_Rotate(T);
		break;
	case RH:
		rd = lc->rchild;
		switch (rd->bf) {
		case LH:
			T->bf = RH;
			lc->bf = EH;
			break;
		case EH:
			T->bf = lc->bf = EH;
			break;
		case RH:
			T->bf = EH;
			lc->bf = LH;
			break;
		}
		rd->bf = EH;
		L_Rotate(T->lchild);
		R_Rotate(T);
		break;
	}
}

void RightBalance(AVLTree &T)
{
	AVLTree rc;
	AVLTree ld;
	rc = T->rchild;
	switch (rc->bf) {
	case RH:
		T->bf = rc->bf = EH;
		L_Rotate(T);
		break;
	case LH:
		ld = rc->lchild;
		switch (ld->bf) {
		case LH:
			T->bf = EH;
			rc->bf = RH;
			break;
		case EH:
			T->bf = rc->bf = EH;
			break;
		case RH:
			T->bf = LH;
			rc->bf = EH;
			break;
		}
		ld->bf = EH;
		R_Rotate(T->rchild);
		L_Rotate(T);
	}
}

bool InsertAVL(AVLTree &T, int e, bool &taller)
{
	if (T == NULL) {
		T = (AVLTree)malloc(sizeof(AVLTreeNode));
		T->data = e;
		T->bf = EH;
		T->lchild = NULL;
		T->rchild = NULL;
		taller = true;
	}
	else if (e == T->data)
	{
		taller = false;
		return 0;
	}
	else if (e < T->data)
	{
		if (InsertAVL(T->lchild, e, taller)==false)
			return 0;
		if(taller==true)
			switch (T->bf) {
			case LH:
				LeftBalance(T);
				taller = false;
				break;
			case EH:
				T->bf = LH;
				taller = true;
				break;
			case RH:
				T->bf = EH;
				taller = false;
				break;
			}
	}
	else {
		if (InsertAVL(T->rchild, e, taller)==false)
			return false;
		if(taller==true)
			switch (T->bf) {
			case LH:
				T->bf = EH;
				taller = false;
				break;
			case EH:
				T->bf = RH;
				taller = true;
				break;
			case RH:
				RightBalance(T);
				taller = false;
				break;
			}
	}
	return true;
}

AVLTree SearchAVL(AVLTree &T, int key)
{
	if (T==NULL)
		return NULL;
	else if (key == T->data)
		return T;
	else if (key < T->data)
		return SearchAVL(T->lchild, key);
	else
		return SearchAVL(T->rchild, key);
}

bool DeleteAVL(AVLTree &T, int key, bool &shorter)
{
	if (T == NULL)
		return false;
	if (T->data == key) {
		AVLTree p;
		if (T->lchild == NULL) {
			p = T;
			T = T->rchild;
			free(p);
			shorter = true;
		}
		else if (T->rchild == NULL) {
			p = T;
			T = T->lchild;
			free(p);
			shorter = true;
		}
		else {
			p = T->lchild;
			while (p->rchild)
				p = p->rchild;
			T->data = p->data;
			DeleteAVL(T->lchild, p->data, shorter);
			if (shorter) {
				switch (T->bf) {
				case LH:
					T->bf = EH;
					shorter = true;
					break;
				case EH:
					T->bf = RH;
					shorter = false;
					break;
				case RH:
					RightBalance(T);
					break;
				}
			}
		}
	}
	else if (key < T->data) {
		if (DeleteAVL(T->lchild, key, shorter))
			return false;
		if (shorter) {
			switch (T->bf) {
			case LH:
				T->bf = EH;
				shorter = true;
				break;
			case EH:
				T->bf = RH;
				shorter = false;
				break;
			case RH:
				RightBalance(T);
				break;
			}
		}
	}
	else {
		if (DeleteAVL(T->rchild, key, shorter))
			return false;
		if (shorter) {
			switch (T->bf) {
			case LH:
				LeftBalance(T);
				break;
			case EH:
				T->bf = LH;
				shorter = false;
				break;
			case RH:
				T->bf = EH;
				shorter = true;
				break;
			}
		}
	}
	return true;
}

void PrintAVL(AVLTree &T, int lev)
{
	int i;
	if (T->rchild)
		PrintAVL(T->rchild,lev + 1);
	for (i = 0; i < lev; i++)
		printf("  ");
	printf("%d\n", T->data);
	if (T->lchild)
		PrintAVL(T->lchild, lev + 1);
}

void MergeAVL(AVLTree &T1, AVLTree &T2) {
	bool taller = 0;
	if (T2 == NULL)
		return;
	MergeAVL(T1, T2->lchild);
	InsertAVL(T1, T2->data, taller);
	MergeAVL(T1, T2->rchild);
}

void SplitAVL(AVLTree &T, int key, AVLTree &T1, AVLTree &T2)
{
	bool taller = 0;
	if (T == NULL)
		return;
	SplitAVL(T->lchild, key, T1, T2);
	if (T->data > key)
		InsertAVL(T2, T->data, taller);
	else
		InsertAVL(T1, T->data, taller);
	SplitAVL(T->rchild, key, T1, T2);
}

void DestroyAVL(AVLTree &T) {
	if (T == NULL)
		return;
	DestroyAVL(T->lchild);
	DestroyAVL(T->rchild);
	free(T);
}

void Split(AVLTree &T, int key, AVLTree &T1, AVLTree &T2)
{
	AVLTree t1=NULL;
	AVLTree t2=NULL;
	SplitAVL(T, key, t1, t2);
	DestroyAVL(T1);
	DestroyAVL(T2);
	T1 = t1;
	T2 = t2;
}

