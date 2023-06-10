#include <easyx.h>
#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <algorithm>
#include <iomanip>
#include <vector>
#include <cstdlib>
#define BACKGROUND RGB(15, 56, 115)
#define BTCOLOR RGB(255, 127, 39)
#define BOARD_UNIT 60
#define BOARD_SIZE 8
#define PAD 1
#define BOTTOM 3
#define MSG_X 60
#define MSG_Y 570
#define MSG_HEIGHT 40
#define BUF_SIZE 50
#define CHESS_REDIUS 10
// solve Knight Tour problem using Backtracking, in a C++ program


//#define BOARD_SIZE 8                                // size of the board
#define BOARD_SIZE_SQUARE (BOARD_SIZE * BOARD_SIZE) // number of cells in the board
#define EMPTY -1                                    // empty cell
#define DIRECTIONS 8                                // number of possible directions for the Knight

#define STACK_INIT_SIZE 200 // initial size of the stack, in terms of number of elements
#define STACK_INCREMENT 50  // increment of the stack, in terms of number of elements

#define OK 0
#define ERROR 1
#define INFEASIBLE -1
#define OVERFLOW_STACK -2

typedef struct
{
    int row, col;
    int degree;
} Cell;

typedef Cell stackElemType;
typedef int status;

typedef struct
{
    stackElemType* base;
    stackElemType* top;
    int stacksize;
} sqStack;

// Draw Function declarations
void PrintFirst();
void PrintOnce();
void EndPrint();
bool OnBoard(int x, int y);
bool OnSetButton(int x, int y);
void HighLight();
void PrintClickPosition(std::pair<int, int> node);
void Onclick();
void SetRun(std::pair<int, int> node);
void PrintChess(int cnt);
void DrawCircle(int x, int y, int style);
void DrawLine(int x, int y, int xx, int yy);
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y);

// function prototypes for stack operations
sqStack* stackAlloc(void);
status initStack_Sq(sqStack* stack);
status destroyStack_Sq(sqStack* stack);
status clearStack_Sq(sqStack* stack);
status stackEmpty_Sq(sqStack* stack);
status stackLength_Sq(sqStack* stack);
status getTop_Sq(sqStack* stack, stackElemType* e);
status push_Sq(sqStack* stack, stackElemType e);
status pop_Sq(sqStack* stack, stackElemType* e);
status stackTraverse(sqStack* stack, status(*visit)());


// RowMove[] and ColMove[] define next move of Knight, recording its coordinate offset.
const int RowMove[DIRECTIONS] = { 2, 1, -1, -2, -2, -1, 1, 2 };
const int ColMove[DIRECTIONS] = { 1, 2, 2, 1, -1, -2, -2, -1 };

std::vector<Cell> tour; // record the tour of the Knight by tuple (row, col)
std::vector<Cell> PreTour;

sqStack* tourStack; // record the tour of the Knight by stack
sqStack* bkTrackStack;

int SetX, SetY;

IMAGE horse;

const int offset = BOARD_UNIT * PAD;
const int ChessOffset = BOARD_UNIT / 2;

// Play Sleep Time
int SpeedOffset = 0;
int Speed = 200;

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
struct ButtonSpeedUp {
    int x = 60;
    int y = 650;
    int xx = 180;
    int yy = 700;
}SpeedUpButton;
struct MessageSpeedUp {
	int x = 70;
	int y = 655;
	int h = 20;
}SpeedUpMsg;
struct ButtonSpeedDown {
	int x = 220;
	int y = 650;
	int xx = 340;
	int yy = 700;
}SpeedDownButton;
struct MessageSpeedDown {
	int x = 230;
	int y = 655;
	int h = 20;
}SpeedDownMsg;
struct BkButtonPos {
    int x = 400;
    int y = 650;
    int xx = 520;
    int yy = 700;
}BkButton;
struct BkMsgPos {
	int x = 410;
	int y = 655;
	int h = 20;
}BkMsg;

//Print Graph of Horse Transparently
void drawAlpha(IMAGE* picture, int  picture_x, int picture_y) 
{
    DWORD* dst = GetImageBuffer();    
    DWORD* draw = GetImageBuffer();
    DWORD* src = GetImageBuffer(picture); 
    int picture_width = picture->getwidth(); 
    int picture_height = picture->getheight(); 
    int graphWidth = getwidth();      
    int graphHeight = getheight();    
    int dstX = 0;   

    // Cp=¦Áp*FP+(1-¦Áp)*BP
    for (int iy = 0; iy < picture_height; iy++)
    {
        for (int ix = 0; ix < picture_width; ix++)
        {
            int srcX = ix + iy * picture_width; 
            int sa = ((src[srcX] & 0xff000000) >> 24); 
            int sr = ((src[srcX] & 0xff0000) >> 16); //R
            int sg = ((src[srcX] & 0xff00) >> 8);   //G
            int sb = src[srcX] & 0xff;              //B
            if (ix >= 0 && ix <= graphWidth && iy >= 0 && iy <= graphHeight && dstX <= graphWidth * graphHeight)
            {
                dstX = (ix + picture_x) + (iy + picture_y) * graphWidth; 
                int dr = ((dst[dstX] & 0xff0000) >> 16);
                int dg = ((dst[dstX] & 0xff00) >> 8);
                int db = dst[dstX] & 0xff;
                draw[dstX] = ((sr * sa / 255 + dr * (255 - sa) / 255) << 16)  // Cp=¦Áp*FP+(1-¦Áp)*BP  £» ¦Áp=sa/255 , FP=sr , BP=dr
                    | ((sg * sa / 255 + dg * (255 - sa) / 255) << 8)         // ¦Áp=sa/255 , FP=sg , BP=dg
                    | (sb * sa / 255 + db * (255 - sa) / 255);              // ¦Áp=sa/255 , FP=sb , BP=db
            }
        }
    }
}

// Print out the chessboard first time
void PrintFirst()
{
    // Set the window size
    initgraph((BOARD_SIZE + PAD * 2) * BOARD_UNIT, (BOARD_SIZE + PAD * 2 + BOTTOM) * BOARD_UNIT);
    // Set the background color
    setbkcolor(BACKGROUND);
    loadimage(&horse, "horse.png", BOARD_UNIT, BOARD_UNIT);
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
    if (x > offset && x < offset + BOARD_UNIT * BOARD_SIZE &&
        y > offset && y < offset + BOARD_UNIT * BOARD_SIZE) {
        return true;
    }
    return false;
}

bool OnSetButton(int x, int y)
{
    if (x > SetButton.x && x < SetButton.xx &&
        y > SetButton.y && y < SetButton.yy) {
        return true;
    }
    return false;
}

// Check if on th bkButton
bool OnBkButton(int x, int y)
{
    if (x > BkButton.x && x < BkButton.xx &&
        y > BkButton.y && y < BkButton.yy) {
		return true;
	}
	return false;
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
                //std::cout << "LLLLLLDDDDDD" << std::endl;
                if (OnBoard(x, y)) {
                    node = std::make_pair(x, y);
                    PrintClickPosition(node);
                }
                else if (OnSetButton(x, y)) {
                    std::cout << "ON SETBUTTON" << std::endl;
                    SetRun(node);  // Set and Run
                    // return;
                }
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

// Draw a chess in two style
void DrawCircle(int x, int y, int style)
{
    std::cout << "Drawing circle at : " << x << " , " << y << std::endl;
    if (style == 1) {
        /*setfillcolor(GREEN);
        solidcircle(x, y, CHESS_REDIUS);*/
        //putimage(x - ChessOffset, y - ChessOffset, &horse, SRCINVERT);
        drawAlpha(&horse, x - ChessOffset, y - ChessOffset);
    }
    else if(style == 2) {
        setfillcolor(LIGHTBLUE);
        solidcircle(x, y, CHESS_REDIUS);
    }
    else if (style == 3) {
        setfillcolor(BTCOLOR);
		solidcircle(x, y, CHESS_REDIUS);
    }
        if (style == 0) {
        setfillcolor(RED);
        solidcircle(x, y, CHESS_REDIUS);
    }
}

// Draw line between chess
void DrawLine(int x, int y,int xx, int yy)
{
    /*std::cout << "Drawing line : " << pre << " to " << cur << std::endl;
    int PreX = (tour[pre].row + PAD) * BOARD_UNIT + ChessOffset;
    int PreY = (tour[pre].col + PAD) * BOARD_UNIT + ChessOffset;
    int CurX = (tour[cur].row + PAD) * BOARD_UNIT + ChessOffset;
    int CurY = (tour[cur].col + PAD) * BOARD_UNIT + ChessOffset;*/
    /*if (style == 1) {
        setfillcolor(GREEN);
        setfillstyle(BS_SOLID);
        line(x, y, xx, yy);
    }
    else {
        setfillcolor(LIGHTBLUE);
        setfillstyle(BS_SOLID);
        li*/
    int lx = (x) * BOARD_UNIT + ChessOffset;
    int ly = (y) * BOARD_UNIT + ChessOffset;
    int lxx = (xx) * BOARD_UNIT + ChessOffset;
    int lyy = (yy) * BOARD_UNIT + ChessOffset;
    setfillcolor(LIGHTBLUE);
    setfillstyle(BS_SOLID);
    line(lx, ly, lxx, lyy);
}

// Printout current chess
void PrintChess(int cnt)
{
    BeginBatchDraw();
    PrintOnce();
    int PreX, PreY;
    PreX = PreY = 0;
    ExMessage msg;
    for (int i = 0; i < BOARD_SIZE_SQUARE; ++i) { // Current Step
        cleardevice();
        PrintOnce();
        // Draw the count of path cnt
        char cntMsg[BUF_SIZE];
        std::cout<<"Drawing Path No. "<<cnt<<std::endl;
        sprintf_s(cntMsg, "Drawing Path No. %d", cnt);
        settextstyle(MSG_HEIGHT, 0, "Cambria");
        settextcolor(WHITE);
        outtextxy(MSG_X, MSG_Y, cntMsg);
        // Draw the chess
        for (int j = 0; j <= i; ++j) {  // Traverse to current
            int ChessX = tour[j].row + 1;
            int ChessY = tour[j].col + 1;
            if (j == 0) DrawCircle(ChessX * BOARD_UNIT + ChessOffset, 
                ChessY * BOARD_UNIT + ChessOffset, 0);
            else if (j == i) DrawCircle(ChessX * BOARD_UNIT + ChessOffset,
                ChessY * BOARD_UNIT + ChessOffset, 1);
            else DrawCircle(ChessX * BOARD_UNIT  + ChessOffset, 
                ChessY * BOARD_UNIT + ChessOffset, 2);
            if(j) DrawLine(PreX, PreY, ChessX, ChessY);
            PreX = ChessX;
            PreY = ChessY;
        }
        // Draw SpeedUp and SpeedDown Button
        setfillcolor(RED);
        rectangle(SpeedUpButton.x, SpeedUpButton.y, SpeedUpButton.xx, SpeedUpButton.yy);
        rectangle(SpeedDownButton.x, SpeedDownButton.y, SpeedDownButton.xx, SpeedDownButton.yy);
        settextstyle(ButtonMsg.h, 0, "Impact");
        settextcolor(RED);
        outtextxy(SpeedUpMsg.x, SpeedUpMsg.y, "Speed +");
        outtextxy(SpeedDownMsg.x, SpeedDownMsg.y, "Speed -");
        // Check if click SpeedUp or SpeedDown
        while (1) {
            if (peekmessage(&msg, EM_MOUSE)) {
                if (msg.message == WM_LBUTTONDOWN) {
                    if (msg.x >= SpeedUpButton.x && msg.x <= SpeedUpButton.xx &&
                        msg.y >= SpeedUpButton.y && msg.y <= SpeedUpButton.yy) {
                        SpeedOffset -= 50;
                    }
                    else if (msg.x >= SpeedDownButton.x && msg.x <= SpeedDownButton.xx &&
                        msg.y >= SpeedDownButton.y && msg.y <= SpeedDownButton.yy) {
                        SpeedOffset += 50;
                    }
                }
            }
            else break;
        }
        FlushBatchDraw();
        int SleepTime = (Speed + SpeedOffset >= 0) ? (Speed + SpeedOffset) : 0;
        Sleep(SleepTime);
    }
}

// Print Deleted Chess
void PrintDeleted() 
{
    bool inBoard[BOARD_SIZE][BOARD_SIZE];
    // Init inBoard
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j)
           inBoard[i][j] = false;
    }
    for (std::vector<Cell>::iterator it = PreTour.begin(); it != PreTour.end(); ++it) {
		int x = it->row;
		int y = it->col;
		inBoard[x][y] = true;
	}
    // Draw the deleted chess
    for (int i = 0; i < BOARD_SIZE; ++i) {
        for (int j = 0; j < BOARD_SIZE; ++j)
            if (!inBoard[i][j]) DrawCircle((i+1) * BOARD_UNIT + ChessOffset, (j+1) * BOARD_UNIT + ChessOffset, 3);
	}
}

// Print the new path showing backtrace
void PrintBt(int cnt)
{
    BeginBatchDraw();
    int PreX, PreY;
    PreX = PreY = 0;
    int BtInfoIndex = 0, len = 0;
    int ThrowNum = 0;
    ExMessage msg;
    char cntMsg[BUF_SIZE];

    // 1 - Print out the PreTour
    cleardevice();
    PrintOnce();

    // 2.1 - Draw SpeedUp and SpeedDown Button
    setfillcolor(RED);
    rectangle(SpeedUpButton.x, SpeedUpButton.y, SpeedUpButton.xx, SpeedUpButton.yy);
    rectangle(SpeedDownButton.x, SpeedDownButton.y, SpeedDownButton.xx, SpeedDownButton.yy);
    settextstyle(ButtonMsg.h, 0, "Impact");
    settextcolor(RED);
    outtextxy(SpeedUpMsg.x, SpeedUpMsg.y, "Speed +");
    outtextxy(SpeedDownMsg.x, SpeedDownMsg.y, "Speed -");
    // 2.2 - Check if click SpeedUp or SpeedDown
    while (1) {
        if (peekmessage(&msg, EM_MOUSE)) {
            if (msg.message == WM_LBUTTONDOWN) {
                if (msg.x >= SpeedUpButton.x && msg.x <= SpeedUpButton.xx &&
                    msg.y >= SpeedUpButton.y && msg.y <= SpeedUpButton.yy) {
                    SpeedOffset -= 50;
                }
                else if (msg.x >= SpeedDownButton.x && msg.x <= SpeedDownButton.xx &&
                    msg.y >= SpeedDownButton.y && msg.y <= SpeedDownButton.yy) {
                    SpeedOffset += 50;
                }
            }
        }
        else break;
    }

    // 3.1 Bt Step1- New path fetch and throw
    cleardevice();
    PrintOnce();
    std::cout << "Drawing Path No. " << cnt << "(BT)  New path Fetch" << std::endl;
    //char cntMsg[BUF_SIZE];
    sprintf_s(cntMsg, "Drawing Path No. %d (BT)", cnt);
    settextstyle(MSG_HEIGHT, 0, "Cambria");
    settextcolor(WHITE);
    outtextxy(MSG_X, MSG_Y, cntMsg);
    // int MaxThrowNum = ThrowNum;
    BtInfoIndex = 0;
    ThrowNum = 0;
    len = stackLength_Sq(bkTrackStack);
    while (BtInfoIndex <len) {
        // Get some point in bt process
        if (bkTrackStack->base[BtInfoIndex].degree == 1) {
            ThrowNum--;
            PreTour.push_back({bkTrackStack->base[BtInfoIndex].row, bkTrackStack->base[BtInfoIndex].col, bkTrackStack->base[BtInfoIndex].degree});
            std::cout << "THROWING NUM : " << ThrowNum << std::endl;
        }
        // Throw some point in bt process
        else if (bkTrackStack->base[BtInfoIndex].degree == 0) {
            ThrowNum++;
            PreTour.pop_back(); 
        }
        // Print once
        cleardevice();
        PrintOnce();
        sprintf_s(cntMsg, "Drawing Path No. %d (BT)", cnt);
        settextstyle(MSG_HEIGHT, 0, "Cambria");
        settextcolor(WHITE);
        outtextxy(MSG_X, MSG_Y, cntMsg);
        //PrintDeleted();
        for (int j = 0; j < BOARD_SIZE_SQUARE - ThrowNum; ++j) {  // Traverse to current
            int ChessX = PreTour[j].row + 1;
            int ChessY = PreTour[j].col + 1;
            if (j == 0) DrawCircle(ChessX * BOARD_UNIT + ChessOffset,
                ChessY * BOARD_UNIT + ChessOffset, 0);   // begin
            else if (j == BOARD_SIZE_SQUARE - ThrowNum - 1) DrawCircle(ChessX * BOARD_UNIT + ChessOffset,
                ChessY * BOARD_UNIT + ChessOffset, 1);   // Current ending
            else DrawCircle(ChessX * BOARD_UNIT + ChessOffset,
                ChessY * BOARD_UNIT + ChessOffset, 2);  // Common
            if (j) DrawLine(PreX, PreY, ChessX, ChessY);
            PreX = ChessX;
            PreY = ChessY;
        }

        // 2.1 - Draw SpeedUp and SpeedDown Button
        setfillcolor(RED);
        rectangle(SpeedUpButton.x, SpeedUpButton.y, SpeedUpButton.xx, SpeedUpButton.yy);
        rectangle(SpeedDownButton.x, SpeedDownButton.y, SpeedDownButton.xx, SpeedDownButton.yy);
        settextstyle(ButtonMsg.h, 0, "Impact");
        settextcolor(RED);
        outtextxy(SpeedUpMsg.x, SpeedUpMsg.y, "Speed +");
        outtextxy(SpeedDownMsg.x, SpeedDownMsg.y, "Speed -");
        // 2.2 - Check if click SpeedUp or SpeedDown
        while (1) {
            if (peekmessage(&msg, EM_MOUSE)) {
                if (msg.message == WM_LBUTTONDOWN) {
                    if (msg.x >= SpeedUpButton.x && msg.x <= SpeedUpButton.xx &&
                        msg.y >= SpeedUpButton.y && msg.y <= SpeedUpButton.yy) {
                        SpeedOffset -= 50;
                    }
                    else if (msg.x >= SpeedDownButton.x && msg.x <= SpeedDownButton.xx &&
                        msg.y >= SpeedDownButton.y && msg.y <= SpeedDownButton.yy) {
                        SpeedOffset += 50;
                    }
                }
            }
            else break;
        }
        FlushBatchDraw();
        int SleepTime = (Speed + SpeedOffset >= 0) ? (Speed + SpeedOffset) : 0;
        Sleep(SleepTime);
        BtInfoIndex++;
    }
}

// Print messages after confirm the starting point 
void PrintSecondMsg()
{
	// Next Button
	setfillcolor(RED);
	rectangle(SetButton.x, SetButton.y, SetButton.xx, SetButton.yy);
	settextstyle(ButtonMsg.h, 0, "Impact");
	settextcolor(RED);
	outtextxy(ButtonMsg.x, ButtonMsg.y, "Next");
	FlushBatchDraw();
}

// Print BkMsg
void PrintBkMsg()
{
    setfillcolor(BTCOLOR);
    rectangle(BkButton.x, BkButton.y, BkButton.xx, BkButton.yy);
    settextstyle(ButtonMsg.h, 0, "Impact");
    settextcolor(BTCOLOR);
    outtextxy(BkMsg.x, BkMsg.y, "Next(BT)");
    FlushBatchDraw();
}

// Actions On Clicking any unit
void Onclick()
{
    HighLight();
}

// create a new stack
sqStack* stackAlloc(void)
{
    sqStack* stack;
    stack = (sqStack*)malloc(sizeof(sqStack));
    if (!stack)
        return NULL;
    return stack;
}

// initialize a stack
status initStack_Sq(sqStack* stack)
{
    stack->base = (stackElemType*)malloc(STACK_INIT_SIZE * sizeof(stackElemType));
    if (!stack->base)
        exit(OVERFLOW_STACK);
    stack->top = stack->base;
    stack->stacksize = STACK_INIT_SIZE;
    return OK;
}

// destroy a stack
status destroyStack_Sq(sqStack* stack)
{
    free(stack->base);
    return OK;
}

// clear a stack
status clearStack_Sq(sqStack* stack)
{
    stack->top = stack->base;
    memset(stack, 0, sizeof(sqStack));
    return OK;
}

// check if a stack is empty
status stackEmpty_Sq(sqStack* stack)
{
    if (stack->top == stack->base)
        return true;
    return false;
}

// return the length of a stack
int stackLength_Sq(sqStack* stack)
{
    return stack->top - stack->base;
}

// if the stack is not empty, then return the top element of the stack using e and return OK; else return ERROR
status getTop_Sq(sqStack* stack, stackElemType* e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *(stack->top - 1);
    return OK;
}

// push an element into a stack
status push_Sq(sqStack* stack, stackElemType e)
{
    if (stack->top - stack->base >= stack->stacksize)
    {
        stack->base = (stackElemType*)realloc(stack->base, (stack->stacksize + STACK_INCREMENT) * sizeof(stackElemType));
        if (!stack->base)
            exit(OVERFLOW_STACK);
        stack->top = stack->base + stack->stacksize;
        stack->stacksize += STACK_INCREMENT;
    }
    *(stack->top++) = e;
    return OK;
}

// pop an element from a stack
status pop_Sq(sqStack* stack, stackElemType* e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *--stack->top;
    return OK;
}

// traverse a stack
status stackTraverse(sqStack* stack, status(*visit)(stackElemType* e))
{
    int i;
    if (stack->top == stack->base)
        return ERROR;
    for (i = stack->top - stack->base - 1; i >= 0; --i)
        visit(&stack->base[i]);
    return OK;
}

// function prototypes for Knight Tour problem
bool compareDegreeAsc(Cell a, Cell b);
bool compareDegreeDes(Cell a, Cell b);
void printTour(const int board[BOARD_SIZE][BOARD_SIZE]);
bool inBoard(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col);
bool isValid(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int getDegree(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int solveKTRecur(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int moveNum);
int solveKTStack(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int moveNum);
int solveKTMain(int initial_row, int initial_col);
static inline double timeElapsed(clock_t time_start, clock_t time_stop);
static inline void printElapsedTime(clock_t time_start, clock_t time_stop);

// compare two cells by their degree in ascending order
bool compareDegreeAsc(Cell a, Cell b)
{
    return a.degree < b.degree;
}

// compare two cells by their degree in descending order
bool compareDegreeDes(Cell a, Cell b)
{
    return a.degree > b.degree;
}

// print solution matrix board[BOARD_SIZE][BOARD_SIZE].
// here, each number means the order of the cell visited by the Knight
void printTour(const int board[BOARD_SIZE][BOARD_SIZE])
{
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++)
    {
        for (col = 0; col < BOARD_SIZE; col++)
            std::cout << " " << std::setw(2) << board[row][col] << " ";
        std::cout << std::endl;
    }
}

void stackToTour(sqStack* stack, std::vector<Cell>* tour)
{
    int i = 0;
    stackElemType* p = stack->base;
    while (p != stack->top)
    {
        if (p->degree == -1)
        {
            (*tour)[i].row = p->row;
            (*tour)[i].col = p->col;
            ++i;
        }
        ++p;
    }
}

// check if (row,col) in a provided BOARD_SIZE^2 chessboard is empty
bool inBoard(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE);
}

// check if (row,col) in a provided BOARD_SIZE^2 chessboard is empty
bool isValid(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == EMPTY);
}

int getDegree(const int board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    int count = 0, i;
    if (!isValid(board, row, col))
        return -1;
    for (i = 0; i < BOARD_SIZE; ++i)
        if (isValid(board, row + RowMove[i], col + ColMove[i]))
            ++count;

    return count;
}

// A recursive utility function to solve Knight Tour problem
int solveKTRecur(int board[BOARD_SIZE][BOARD_SIZE], int row, int col, int moveNum)
{
    int k, k_offset;        // used for iteration over all possible moves
    int start;              // used for recording the starting index of the next move of the Knight
    int next_row, next_col; // used for recording next move of the Knight
    // int degree, min_degree; // used for recording the degree of the next move of the Knight
    // int idx, min_degree_idx;  // used for recording the index of the next move of the Knight
    Cell next_seq[DIRECTIONS]; // used for sort the next move of the Knight

    // if all moves are finished, then return true
    if (moveNum == BOARD_SIZE_SQUARE)
        return true;

    start = rand() % DIRECTIONS;

    for (int k = 0; k < DIRECTIONS; ++k)
    {
        k_offset = (start + k) % DIRECTIONS;
        next_seq[k].row = row + RowMove[k_offset];
        next_seq[k].col = col + ColMove[k_offset];
        next_seq[k].degree = getDegree(board, next_seq[k].row, next_seq[k].col);
    }
    std::sort(next_seq, next_seq + DIRECTIONS, compareDegreeAsc); // sort the next move of the Knight by the degree of the next move

    // try all next moves from the current coordinate (row, col)
    for (k = 0; k < DIRECTIONS; k++)
    {
        // check if the next move is valid
        if (next_seq[k].degree == -1)
            continue;

        // get the coordinate of the next move of the Knight
        next_row = next_seq[k].row;
        next_col = next_seq[k].col;

        // if the next move is valid, then mark the current coordinate as visited
        board[next_row][next_col] = moveNum;

        // recur for next turn of move
        if (solveKTRecur(board, next_row, next_col, moveNum + 1) == true)
            return true;

        // backtracking: if the next move is not valid, then mark the current coordinate as unvisited and try other moves
        else
            board[next_row][next_col] = EMPTY;
    }
    return false;
}

/*
    This function is based on the recursive function solveKTRecur().

    Though the problem might have multiple solutions, this function prints one of the feasible solutions.
    If any solution exists, return true and prints the tour.
    If there is no feasible solution, then it prints "Solution does not exist".
    Actually, the function can be modified to print all solutions by using backtracking.

    This function use two initial position as input, with (0,0) as default values.
    For using other initial position, just change the default values of initial_row and initial_col.
*/
int solveKTMainRec(int initial_row = 0, int initial_col = 0)
{
    int board[BOARD_SIZE][BOARD_SIZE]; // solution matrix
    int row, col;

    // initialization of the board (solution matrix)
    for (row = 0; row < BOARD_SIZE; row++)
        for (col = 0; col < BOARD_SIZE; col++)
            board[row][col] = EMPTY;

    // mark the initial position as visited (for recursion)
    board[initial_row][initial_col] = 0;

    // start from (initial_row, initial_col) and explore all tours using solveKTRecur()
    if (solveKTRecur(board, initial_row, initial_col, 1) == false)
    {
        std::cout << "Solution does not exist" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Solution exists for (" << initial_row << ", " << initial_col << "):" << std::endl;
        printTour(board);
        std::cout << std::endl;
    }

    return true;
}

unsigned long long int while_count = 0;

// add a new position into the bkTrackStack.
void addBkTrackStack(sqStack* bkTrackStack, int row, int col, bool isForward)
{
    stackElemType bkTrackHead;
    bkTrackHead.row = row;
    bkTrackHead.col = col;
    bkTrackHead.degree = isForward;
    push_Sq(bkTrackStack, bkTrackHead);
}

// A recursive utility function to solve Knight Tour problem
// tourStack is used to record the tour of the Knight by stack, and bkTrackStack is used to record the backtracking of the Knight by stack
// Attention: tourStack should be given as used to be, if continue_last is false; else, tourStack should be clear.
//           bkTrackStack should be clear.
int solveKTStack(int board[BOARD_SIZE][BOARD_SIZE], int initial_row, int initial_col, sqStack* tourStack, sqStack* bkTrackStack, bool continue_last)
{
    Cell next_seq[DIRECTIONS]; // sort the next move of the Knight
    stackElemType tourHead, bkTrackHead;    // used for recording the top element of the stack
    int k, k_offset;           // used for iteration over all possible moves
    int start;                 // starting index of the next move of the Knight
    int row, col;              // current coordinate of the Knight
    // int next_row, next_col;    // next move of the Knight
    int moveNum;               // current move number of the Knight. begin with 0

    // Attention: the stack is initialized in the main function, not here.

    // The degree of a Cell may have 3 meanings:
    // 1. the degree of the next move of the Knight.
    // 2. the move number of the Knight at current position. begin with 0.
    // 3. if it appears to be -1, then it means that all next moves is not valid. then we pop the it and try other moves.

    // the legality of the initial position is checked in the function calling this function

    // initialization of the stack, and push the initial position into the stack
    if (continue_last == false)
    {
        // add the initial position into the stack
        tourHead.row = initial_row;
        tourHead.col = initial_col;
        tourHead.degree = 0;
        push_Sq(tourStack, tourHead);
    }
    // default: continue the last tour, where the top element of the stack is the last position of the Knight, with degree==-1
    else if (getTop_Sq(tourStack, &tourHead) == OK && tourHead.degree == -1)
    {
        // pop the top element of the stack and try other moves
        pop_Sq(tourStack, &tourHead);
        board[tourHead.row][tourHead.col] = EMPTY;

        // add the current position into the backtracking stack, backtracking
        addBkTrackStack(bkTrackStack, tourHead.row, tourHead.col, false);
        // Here we come over another meaning of the degree of a Cell: if it appears to be true, then it means that the Knight is moving forward. else, it is backtracking.
    }
    else
    {
        std::cout << "Error: continue_last is true, but the top element of the stack is not the last position of the Knight." << std::endl;
        return false;
    }

    // use a while loop to explore all possible tours of the Knight, until the stack is empty
    while (stackEmpty_Sq(tourStack) == false)
    {
        // get the top element of the stack
        pop_Sq(tourStack, &tourHead);
        moveNum = tourHead.degree + 1;

        // for printing the progress of the program. can be commented out.
        while_count++;
        if (while_count % 100000000 == 0)
            std::cout << "while_count: " << while_count << std::endl;

        // if any tour is finished, then modify degree into meaning 2, and push it back and return true
        if (moveNum == BOARD_SIZE_SQUARE)
        {
            board[tourHead.row][tourHead.col] = tourHead.degree;
            tourHead.degree = -1;
            push_Sq(tourStack, tourHead);

            // add the current position into the backtracking stack, forwarding
            addBkTrackStack(bkTrackStack, tourHead.row, tourHead.col, true);

            return true;
        }
        // if the next move is not valid (tourHead.degree == -1), then pop the top element of the stack and try other moves
        else if (!moveNum)
        {
            // add the current position into the backtracking stack, backtracking
            addBkTrackStack(bkTrackStack, tourHead.row, tourHead.col, false);

            board[tourHead.row][tourHead.col] = EMPTY;
            continue;
        }

        // set the current coordinate of the Knight, and mark the current coordinate as visited
        row = tourHead.row;
        col = tourHead.col;
        board[row][col] = tourHead.degree;

        // randomize the starting index of the next move of the Knight
        start = rand() % DIRECTIONS;

        // get the next move of the Knight
        for (k = 0; k < DIRECTIONS; ++k)
        {
            k_offset = (start + k) % DIRECTIONS;
            // k_offset = k;
            next_seq[k].row = row + RowMove[k_offset];
            next_seq[k].col = col + ColMove[k_offset];
            next_seq[k].degree = getDegree(board, next_seq[k].row, next_seq[k].col);
            if (next_seq[k].degree == 0 && moveNum != BOARD_SIZE_SQUARE - 1)
                break;
        }
        // if the degree of one position is 0, then it means this position is not accessible, so we won't consider the following positions.
        // note that the degree of the last position of the Knight is 0, but it is accessible.
        if (k != DIRECTIONS)
        {
            // add 0-degree position forwarding, and back track these 2 positions
            // addBkTrackStack(bkTrackStack, next_seq[k].row, next_seq[k].col, true);
            // addBkTrackStack(bkTrackStack, next_seq[k].row, next_seq[k].col, false);
            // addBkTrackStack(bkTrackStack, row, col, false);
            board[row][col] = EMPTY;
            continue;
        }
        // modify degree into meaning 2, and push the current coordinate of the Knight back into the stack
        tourHead.degree = -1;
        push_Sq(tourStack, tourHead);

        // add the current position into the backtracking stack, forwarding
        addBkTrackStack(bkTrackStack, row, col, true);

        // sort the next move of the Knight by the degree of the next move.
        // for stack is LIFO, we sort the next move of the Knight in descending order (from large to small)
        // for recursion is FIFO, we sort the next move of the Knight in ascending order (from small to large) in solveKTRecur()
        std::sort(next_seq, next_seq + DIRECTIONS, compareDegreeDes);

        // try all next moves from the current coordinate (row, col)
        for (k = 0; k < DIRECTIONS; k++)
        {
            // check if the next move is valid. for we have sorted the next move of the Knight, if the next move is not valid, then all the following moves are not valid
            if (next_seq[k].degree == -1)
                break;

            // push the next move of the Knight into the stack
            next_seq[k].degree = moveNum;
            push_Sq(tourStack, next_seq[k]);
        }
    }

    // if the stack is empty, then return false
    std::cout << "Error: No tour is found" << std::endl;
    return false;
}

// initialize, call solveKTStack(), and print the solution
// tourStack is used to record the tour of the Knight by stack, and bkTrackStack is used to record the backtracking of the Knight by stack
// Attention: tourStack should be given as used to be, if continue_last is false; else, tourStack should be clear.
int solveKTMainStack(sqStack* tourStack, sqStack* bkTrackStack, std::vector<Cell>* tour, bool continue_last, int initial_row = 0, int initial_col = 0)
{
    stackElemType* p = tourStack->base;
    int board[BOARD_SIZE][BOARD_SIZE]; // solution matrix
    int row, col, len, i;

    // Attention: the stack is initialized in the main function, not here.
    //             the path is passed by the main function, not here.

    clearStack_Sq(bkTrackStack);

    // check the legality of the initial position
    if (!inBoard(board, initial_row, initial_col))
        return false;

    // initialization of the board (solution matrix)
    if (continue_last == false)
    {
        for (row = 0; row < BOARD_SIZE; row++)
            for (col = 0; col < BOARD_SIZE; col++)
                board[row][col] = EMPTY;
    }
    else
    {
        i = 0;
        while (p != tourStack->top)
        {
            if (p->degree == -1)
            {
                board[p->row][p->col] = i;
                ++i;
            }
            ++p;
        }
    }
    // start from (initial_row, initial_col) and explore all tours using solveKTRecur()
    if (solveKTStack(board, initial_row, initial_col, tourStack, bkTrackStack, continue_last) == true)
    {
        // if (while_count % 500000 == 0)
        {
            std::cout << "Solution exists for (" << initial_row << ", " << initial_col << "):" << std::endl;
            printTour(board);
            stackToTour(tourStack, tour);
            for (i = 0; i < BOARD_SIZE_SQUARE; ++i)
            {
                std::cout << "(" << (*tour)[i].row << ", " << (*tour)[i].col << ") ";
                if ((i + 1) % 8 == 0)
                    std::cout << std::endl;
            }
            len = stackLength_Sq(bkTrackStack);
            for (i = 0; i < len; ++i)
            {
                std::cout << "(" << bkTrackStack->base[i].row << ", " << bkTrackStack->base[i].col << ", " << bkTrackStack->base[i].degree << ") ";
                if ((i + 1) % 10 == 0)
                    std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        return true;
    }
    // std::cout << "Solution does not exist" << std::endl;
    return false;
}

// calculate the elapsed time
double timeElapsed(clock_t time_start, clock_t time_stop)
{
    return (double)(time_stop - time_start) / CLOCKS_PER_SEC;
}

// print the elapsed time
void printElapsedTime(clock_t time_start, clock_t time_stop)
{
    std::cout << "Time: " << timeElapsed(time_start, time_stop) << " seconds" << std::endl;
}


// main function, used to test the program
int Algorithm_op()
{
    std::vector<Cell> tour; // record the tour of the Knight by tuple (row, col)
    double duration;
    sqStack* tourStack; // record the tour of the Knight by stack
    sqStack* bkTrackStack; // record the backtracking of the Knight by stack
    clock_t start, stop;
    int initial_row, initial_col;

    tour.resize(BOARD_SIZE_SQUARE);

    // user's control of the initial position of the Knight
    std::cin >> initial_row >> initial_col;

    // initialize random seed
    srand(time(NULL));

    // create a new stack
    tourStack = stackAlloc();
    initStack_Sq(tourStack);
    bkTrackStack = stackAlloc();
    initStack_Sq(bkTrackStack);

    // start measuring time
    start = clock();

    // stop measuring time and calculate the elapsed time
    stop = clock();
    // duration = timeElapsed(start, finish);
    printElapsedTime(start, stop);

    return 0;
}

// Set the Program Run
void SetRun(std::pair<int, int> node)
{
    std::cout << "IN setRUN....." << std::endl;
    tour.resize(BOARD_SIZE * BOARD_SIZE);
    PreTour.resize(BOARD_SIZE * BOARD_SIZE);
    SetX = (node.first - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
    SetY = (node.second - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
    ExMessage msg;

    // create a new stack
    tourStack = stackAlloc();
    bkTrackStack = stackAlloc();
    initStack_Sq(tourStack);
    initStack_Sq(bkTrackStack);
    std::cout << "SetX = " << SetX << " SetY = " << SetY << std::endl;
    
    // Enable mutiple paths
    int sum = 1;
    solveKTMainStack(tourStack, bkTrackStack, &tour, false, SetX - 1, SetY - 1);
    std::cout << "Number of paths: " << std::setw(4) << sum << "  while_count: " << std::setw(10) << while_count << std::endl;
    PrintOnce();
    PrintChess(1);
    PrintSecondMsg();
    PrintBkMsg();
    std::cout << "VECTOR COPY" << std::endl;
    int i = 0;
    for (std::vector<Cell>::iterator it = tour.begin(); it != tour.end(); ++it) {
		PreTour[i++] = *it;
	}
    //PreTour.assign(tour.begin(), tour.end());
    while (solveKTMainStack(tourStack, bkTrackStack, &tour, true, SetX - 1, SetY - 1) == true)
    {
        ++sum;
        std::cout << "Number of paths: " << std::setw(4) << sum << "  while_count: " << std::setw(10) << while_count << std::endl;
        while (1) {
            if (peekmessage(&msg, EM_MOUSE) && msg.message == WM_LBUTTONDOWN) {
                std::cout << "Click for Next path..." << std::endl;
                int x = msg.x;
                int y = msg.y;
                if (OnSetButton(x, y)) {
                    std::cout << "ON SETBUTTON" << std::endl;
                    cleardevice();
                    PrintOnce();
                    PrintChess(sum);
                    PrintBkMsg();
                    PrintSecondMsg();
                    break;
                }
                else if (OnBkButton(x, y)) {
					std::cout << "ON BKBUTTON" << std::endl;
					cleardevice();
					PrintOnce();
					PrintBt(sum);
                    PrintSecondMsg();
					PrintBkMsg();
					break;
                }
            }
        }
        std::cout << "VECTOR COPY" << std::endl;
        //PreTour.assign(tour.begin(), tour.end());
        int i = 0;
        for (std::vector<Cell>::iterator it = tour.begin(); it != tour.end(); ++it) {
            PreTour[i++] = *it;
        }
    }
    std::cout << "sum: " << sum << std::endl;
    // DESTROY STACK
    destroyStack_Sq(tourStack);
    _getch();
}

int main()
{
    PrintFirst();
    Onclick();
    //Algorithm_op();
    EndPrint();
    _getch();
    return 0;
}
