#include <easyx.h>
#include <graphics.h>
#include <windows.h>
#include <iostream>
#include <cstdio>
#include <conio.h>
#include <algorithm>
#include <iomanip>
#include <cstdlib>
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
#define CHESS_REDIUS 15
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
void PrintChess();
void DrawCircle(int x, int y, int style);

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

// RowMove[] and ColMove[] define next move of Knight, recording its coordinate offset.
const int RowMove[DIRECTIONS] = { 2, 1, -1, -2, -2, -1, 1, 2 };
const int ColMove[DIRECTIONS] = { 1, 2, 2, 1, -1, -2, -2, -1 };

int tour[BOARD_SIZE_SQUARE][2];    // record the tour of the Knight by tuple (row, col)
int SetX, SetY;

const int offset = BOARD_UNIT * PAD;
const int ChessOffset = BOARD_UNIT / 2;

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
void DrawCicle(int x, int y, int style)
{
    if (style == 1) {
        setfillcolor(GREEN);
        solidcircle(x, y, CHESS_REDIUS);
    }
    else {
        setfillcolor(LIGHTBLUE);
        solidcircle(x, y, CHESS_REDIUS);
    }
}

// Draw line between chess
void DrawLine(int pre, int cur, int style)
{
    int PreX = (tour[pre][0] + PAD) * BOARD_UNIT + ChessOffset;
    int PreY = (tour[pre][1] + PAD) * BOARD_UNIT + ChessOffset;
    int CurX = (tour[cur][0] + PAD) * BOARD_UNIT + ChessOffset;
    int CurY = (tour[cur][1] + PAD) * BOARD_UNIT + ChessOffset;
    if (style == 1) {
        setfillcolor(GREEN);
        setfillstyle(BS_SOLID);
        line(PreX, PreY, CurX, CurY);
    }
    else {
        setfillcolor(LIGHTBLUE);
        setfillstyle(BS_SOLID);
        line(PreX, PreY, CurX, CurY);
    }
}

// Printout current chess
void PrintChess()
{
    BeginBatchDraw();
    PrintOnce();
    int Pre;
    Pre = 0;
    for (int i = 0; i < BOARD_SIZE_SQUARE; ++i) { // Current Step
        cleardevice();
        PrintOnce();
        for (int j = 0; j <= i; ++j) {  // Traverse to current
            int ChessX = tour[j][0] + 1;
            int ChessY = tour[j][1] + 1;
            if (j == i) DrawCicle(ChessX * BOARD_UNIT + ChessOffset,
                ChessY * BOARD_UNIT + ChessOffset, 1);
            else DrawCicle(ChessX * BOARD_UNIT  + ChessOffset, ChessY * BOARD_UNIT + ChessOffset, 2);
            if(j) DrawLine(Pre, j, (i == j));
            Pre = j;
        }
        FlushBatchDraw();
        Sleep(200);
    }
}

// Actions On Clicking any unit
void Onclick()
{
    HighLight();
}



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
void printTour(int board[BOARD_SIZE][BOARD_SIZE]);
bool isValid(int board[BOARD_SIZE][BOARD_SIZE], int row, int col);
int getDegree(int board[BOARD_SIZE][BOARD_SIZE], int row, int col);
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
void printTour(int board[BOARD_SIZE][BOARD_SIZE])
{
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++)
    {
        for (col = 0; col < BOARD_SIZE; col++)
            std::cout << " " << std::setw(2) << board[row][col] << " ";
        std::cout << std::endl;
    }
}

void stackToTour(sqStack* stack, int tour[BOARD_SIZE_SQUARE][2])
{
    int i = 0;
    stackElemType* p = stack->base;
    while (p != stack->top)
    {
        if (p->degree != -1)
        {
            ++p;
            continue;
        }
        tour[i][0] = p->row;
        tour[i][1] = p->col;
        ++i;
        ++p;
    }
}

// check if (row,col) in a provided BOARD_SIZE^2 chessboard is empty
bool isValid(int board[BOARD_SIZE][BOARD_SIZE], int row, int col)
{
    return (row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE && board[row][col] == EMPTY);
}

int getDegree(int board[BOARD_SIZE][BOARD_SIZE], int row, int col)
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

// A recursive utility function to solve Knight Tour problem
int solveKTStack(int board[BOARD_SIZE][BOARD_SIZE], int initial_row, int initial_col, sqStack* tourStack)
{
    Cell next_seq[DIRECTIONS]; // sort the next move of the Knight
    stackElemType pathHead;    // used for recording the top element of the stack
    int k, k_offset;           // used for iteration over all possible moves
    int start;                 // starting index of the next move of the Knight
    int row, col;              // current coordinate of the Knight
    int next_row, next_col;    // next move of the Knight
    int moveNum;               // current move number of the Knight. begin with 0

    // Attention: the stack is initialized in the main function, not here.

    // The degree of a Cell may have 3 meanings:
    // 1. the degree of the next move of the Knight.
    // 2. the move number of the Knight at current position. begin with 0.
    // 3. if it appears to be -1, then it means that all next moves is not valid. then we pop the it and try other moves.

    // check if the initial position is valid
    if (!isValid(board, initial_row, initial_col))
        return false;

    // initialization of the stack, and push the initial position into the stack
    pathHead.row = initial_row;
    pathHead.col = initial_col;
    pathHead.degree = 0;
    push_Sq(tourStack, pathHead);

    // use a while loop to explore all possible tours of the Knight, until the stack is empty
    while (stackEmpty_Sq(tourStack) == false)
    {
        // get the top element of the stack
        pop_Sq(tourStack, &pathHead);
        moveNum = pathHead.degree + 1;

        // if any tour is finished, then modify degree into meaning 2, and push it back and return true
        if (moveNum == BOARD_SIZE_SQUARE)
        {
            board[pathHead.row][pathHead.col] = pathHead.degree;
            pathHead.degree = -1;
            push_Sq(tourStack, pathHead);
            return true;
        }
        // if the next move is not valid (pathHead.degree == -1), then pop the top element of the stack and try other moves
        else if (!moveNum)
        {
            board[pathHead.row][pathHead.col] = EMPTY;
            continue;
        }

        // set the current coordinate of the Knight, and mark the current coordinate as visited
        row = pathHead.row;
        col = pathHead.col;
        board[row][col] = pathHead.degree;
        // modify degree into meaning 2, and push the current coordinate of the Knight back into the stack
        pathHead.degree = -1;
        push_Sq(tourStack, pathHead);

        // randomize the starting index of the next move of the Knight
        start = rand() % DIRECTIONS;

        // get the next move of the Knight
        for (int k = 0; k < DIRECTIONS; ++k)
        {
            k_offset = (start + k) % DIRECTIONS;
            next_seq[k].row = row + RowMove[k_offset];
            next_seq[k].col = col + ColMove[k_offset];
            next_seq[k].degree = getDegree(board, next_seq[k].row, next_seq[k].col);
        }

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

int solveKTMainStack(sqStack* tourStack, int initial_row = 0, int initial_col = 0)
{
    int board[BOARD_SIZE][BOARD_SIZE]; // solution matrix
    int row, col;

    // Attention: the stack is initialized in the main function, not here.

    // initialization of the board (solution matrix)
    for (row = 0; row < BOARD_SIZE; row++)
        for (col = 0; col < BOARD_SIZE; col++)
            board[row][col] = EMPTY;

    // start from (initial_row, initial_col) and explore all tours using solveKTRecur()
    if (solveKTStack(board, initial_row, initial_col, tourStack) == false)
    {
        std::cout << "Solution does not exist" << std::endl;
        return false;
    }
    else
    {
        std::cout << "Solution exists for (" << initial_row << ", " << initial_col << "):" << std::endl;
        printTour(board);
        stackToTour(tourStack, tour);
        for (int i = 0; i < BOARD_SIZE_SQUARE; ++i)
        {
            std::cout << "(" << tour[i][0] << ", " << tour[i][1] << ") ";
            if ((i + 1) % 8 == 0)
                std::cout << std::endl;
        }
    }

    return true;
}

// calculate the elapsed time
static inline double timeElapsed(clock_t time_start, clock_t time_stop)
{
    return (double)(time_stop - time_start) / CLOCKS_PER_SEC;
}

// print the elapsed time
static inline void printElapsedTime(clock_t time_start, clock_t time_stop)
{
    std::cout << "Time: " << timeElapsed(time_start, time_stop) << " seconds" << std::endl;
}

// main function, used to test the program
int Algorithm_op()
{
    sqStack* tourStack; // record the tour of the Knight by stack
    double duration;
    clock_t start, stop;
    int initial_row, initial_col;

    // user's control of the initial position of the Knight
    std::cin >> initial_row >> initial_col;

    // initialize random seed
    srand(time(NULL));

    // create a new stack
    tourStack = stackAlloc();
    initStack_Sq(tourStack);

    // start measuring time
    start = clock();

    // solve the Knight Tour problem for all possible starting positions
    // for (int i = 0; i < BOARD_SIZE; ++i)
    //     for (int j = 0; j < BOARD_SIZE; ++j)
    //         solveKTMainStack(i, j);

    // solve the Knight Tour problem for a specific starting position controlled by the user
    // solveKTMainRec(initial_row, initial_col);
    solveKTMainStack(tourStack, initial_row, initial_col);

    // solve the Knight Tour problem for a specific starting position generated randomly
    // solveKTMainStack(rand() % BOARD_SIZE, rand() % BOARD_SIZE);

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
    SetX = (node.first - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
    SetY = (node.second - BOARD_UNIT * PAD) / BOARD_UNIT + 1;
    sqStack* tourStack; // record the tour of the Knight by stack
    // create a new stack
    tourStack = stackAlloc();
    initStack_Sq(tourStack);
    std::cout << "SetX = " << SetX << " SetY = " << SetY << std::endl;
    solveKTMainStack(tourStack, SetX - 1, SetY - 1);
    PrintOnce();
    PrintChess();
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
