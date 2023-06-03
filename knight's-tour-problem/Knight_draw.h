#pragma once

#include <easyx.h>
#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <conio.h>
//#include "Knight_algorithm.h"
#define BACKGROUND RGB(15, 56, 115)
#define BOARD_UNIT 60
#define BOARD_SIZE 8
#define PAD 1
#define BOTTOM 3
#define MSG_X 60
#define MSG_Y 570
#define MSG_HEIGHT 40
#define BUF_SIZE 50

// Function declarations
void PrintFirst();
void PrintOnce();
void EndPrint();
bool OnBoard(int x, int y);
bool OnSetButton(int x, int y);
void HighLight();
void PrintClickPosition(std::pair<int, int> node);
void Onclick();
void SetRun(std::pair<int, int> node);

// record the tour of the Knight by tuple (row, col)
//extern int tour[BOARD_SIZE_SQUARE][2];  
//extern int SetX, SetY;
int offset = BOARD_UNIT * PAD;
struct SetButtonPos {
	int x = 440;
	int y = 570;
	int xx = 520;
	int yy = 620;
}SetButton;
struct ButtonMsgPos {
	int x = 450;
	int y = 575;
	int h = 40;
}ButtonMsg;

// Print out the chessboard first time
void PrintFirst()
{
	// Set the window size
	initgraph((BOARD_SIZE + PAD * 2) * BOARD_UNIT, (BOARD_SIZE + PAD * 2 + BOTTOM) * BOARD_UNIT);
	// Set the background color
	setbkcolor(BACKGROUND);
	// Clear the screen
	cleardevice();
}

// Print out the chessboard once
void PrintOnce()
{
	// draw chessboard's background
	cleardevice();
	for (int i = 1; i <= BOARD_SIZE; ++i) {
		for (int j = 1; j <= BOARD_SIZE; ++j) {
			if ((i + j) % 2) {
				setfillcolor(BLACK);
			}
			else {
				setfillcolor(WHITE);
			}
			solidrectangle((j - 1) * BOARD_UNIT + offset, (i - 1) * BOARD_UNIT + offset,
				j * BOARD_UNIT + offset, i * BOARD_UNIT + offset);
		}
	}
}

// The end of the print
void EndPrint()
{
	// Pause the screen
	// _getch();
	// Close the window
	closegraph();
}

// Check if the mouse is on the board
bool OnBoard(int x, int y)
{
	if (x >= offset && x <= offset + BOARD_UNIT * BOARD_SIZE &&
		y >= offset && y <= offset + BOARD_UNIT * BOARD_SIZE) {
		return true;
	}
	return false;
}

bool OnSetButton(int x, int y)
{
	if (x >= SetButton.x && x <= SetButton.xx &&
		y >= SetButton.y && y <= SetButton.yy) {
		return true;
	}
}

// HighLight the unit if mouse moves on it
void HighLight()
{
	std::cout << "HighLight" << std::endl;
	ExMessage msg;
	bool isClick = false;
	std::pair<int, int> node;
	while (1) {
		if (peekmessage(&msg, EM_MOUSE)) {
			int x = msg.x;
			int y = msg.y;
			BeginBatchDraw();
			PrintOnce();
			if (msg.message == WM_LBUTTONDOWN) {
				node = std::make_pair(x, y);
				if (OnBoard(x, y)) PrintClickPosition(node);
				else if (OnSetButton(x, y)) SetRun(node);  // Set and Run
				isClick = true;
			}
			if (!OnBoard(x, y)) continue;
			if (isClick) PrintClickPosition(node);
			int ii = (y - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
			int jj = (x - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
			std::cout << "HighLighting " << ii << " " << jj << std::endl;
			int offset = BOARD_UNIT * PAD;
			// Print out Special color
			setfillcolor(RED);
			solidrectangle((jj - 1) * BOARD_UNIT + offset, (ii - 1) * BOARD_UNIT + offset,
				jj * BOARD_UNIT + offset, ii * BOARD_UNIT + offset);
			FlushBatchDraw();
		}
	}
}

// PrintOut the Clicked position
void PrintClickPosition(std::pair<int, int> node)
{
	std::cout << "Printing POS..." << std::endl;
	std::pair<int, int> OutPos;
	char OutPositionMsg[BUF_SIZE];
	OutPos.first = (node.first - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
	OutPos.second = (node.second - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
	sprintf_s(OutPositionMsg, "Begin Position ( %d , %d )", OutPos.second, OutPos.first);
	settextstyle(MSG_HEIGHT, 0, "Cambria");
	settextcolor(WHITE);
	outtextxy(MSG_X, MSG_Y, OutPositionMsg);
	// Out Button
	setfillcolor(RED);
	rectangle(SetButton.x, SetButton.y, SetButton.xx, SetButton.yy);
	settextstyle(ButtonMsg.h, 0, "Impact");
	settextcolor(RED);
	outtextxy(ButtonMsg.x, ButtonMsg.y, "Start");
	FlushBatchDraw();
}

// Actions On Clicking any unit
void Onclick()
{
	HighLight();
}

// Set the Program Run
void SetRun(std::pair<int, int> node)
{
	//SetX = node.first;
	//SetY = node.second;
	PrintOnce();
	_getch();
}

/* int main()
{
	PrintFirst();
	Onclick();
	EndPrint();
	_getch();
	return 0;
}*/
