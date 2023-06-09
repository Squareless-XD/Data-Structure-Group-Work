// solve Knight Tour problem using Backtracking, in a C++ program
#include <bits/stdc++.h>

#define BOARD_SIZE 8                                // size of the board
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
    stackElemType *base;
    stackElemType *top;
    int stacksize;
} sqStack;

// RowMove[] and ColMove[] define next move of Knight, recording its coordinate offset.
const int RowMove[DIRECTIONS] = {2, 1, -1, -2, -2, -1, 1, 2};
const int ColMove[DIRECTIONS] = {1, 2, 2, 1, -1, -2, -2, -1};

// function prototypes for stack operations
sqStack *stackAlloc(void);
status initStack_Sq(sqStack *stack);
status destroyStack_Sq(sqStack *stack);
status clearStack_Sq(sqStack *stack);
status stackEmpty_Sq(sqStack *stack);
status stackLength_Sq(sqStack *stack);
status getTop_Sq(sqStack *stack, stackElemType *e);
status push_Sq(sqStack *stack, stackElemType e);
status pop_Sq(sqStack *stack, stackElemType *e);
status stackTraverse(sqStack *stack, status (*visit)());

// create a new stack
sqStack *stackAlloc(void)
{
    sqStack *stack;
    stack = (sqStack *)malloc(sizeof(sqStack));
    if (!stack)
        return NULL;
    return stack;
}

// initialize a stack
status initStack_Sq(sqStack *stack)
{
    stack->base = (stackElemType *)malloc(STACK_INIT_SIZE * sizeof(stackElemType));
    if (!stack->base)
        exit(OVERFLOW_STACK);
    stack->top = stack->base;
    stack->stacksize = STACK_INIT_SIZE;
    return OK;
}

// destroy a stack
status destroyStack_Sq(sqStack *stack)
{
    free(stack->base);
    return OK;
}

// clear a stack
status clearStack_Sq(sqStack *stack)
{
    stack->top = stack->base;
    memset(stack, 0, sizeof(sqStack));
    return OK;
}

// check if a stack is empty
status stackEmpty_Sq(sqStack *stack)
{
    if (stack->top == stack->base)
        return true;
    return false;
}

// return the length of a stack
int stackLength_Sq(sqStack *stack)
{
    return stack->top - stack->base;
}

// if the stack is not empty, then return the top element of the stack using e and return OK; else return ERROR
status getTop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *(stack->top - 1);
    return OK;
}

// push an element into a stack
status push_Sq(sqStack *stack, stackElemType e)
{
    if (stack->top - stack->base >= stack->stacksize)
    {
        stack->base = (stackElemType *)realloc(stack->base, (stack->stacksize + STACK_INCREMENT) * sizeof(stackElemType));
        if (!stack->base)
            exit(OVERFLOW_STACK);
        stack->top = stack->base + stack->stacksize;
        stack->stacksize += STACK_INCREMENT;
    }
    *(stack->top++) = e;
    return OK;
}

// pop an element from a stack
status pop_Sq(sqStack *stack, stackElemType *e)
{
    if (stack->top == stack->base)
        return ERROR;
    *e = *--stack->top;
    return OK;
}

// traverse a stack
status stackTraverse(sqStack *stack, status (*visit)(stackElemType *e))
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

void stackToTour(sqStack *stack, std::vector<Cell> *tour)
{
    int i = 0;
    stackElemType *p = stack->base;
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
void addBkTrackStack(sqStack *bkTrackStack, int row, int col, bool isForward)
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
int solveKTStack(int board[BOARD_SIZE][BOARD_SIZE], int initial_row, int initial_col, sqStack *tourStack, sqStack *bkTrackStack, bool continue_last)
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
int solveKTMainStack(sqStack *tourStack, sqStack *bkTrackStack, std::vector<Cell> *tour, bool continue_last, int initial_row = 0, int initial_col = 0)
{
    stackElemType *p = tourStack->base;
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
int main()
{
    std::vector<Cell> tour; // record the tour of the Knight by tuple (row, col)
    double duration;
    sqStack *tourStack; // record the tour of the Knight by stack
    sqStack *bkTrackStack; // record the backtracking of the Knight by stack
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

    // solve the Knight Tour problem for all possible starting positions
    // for (int i = 0; i < BOARD_SIZE; ++i)
    //     for (int j = 0; j < BOARD_SIZE; ++j)
    //         solveKTMainStack(i, j);
/*
    // solve the Knight Tour problem for a specific starting position controlled by the user
    // solveKTMainRec(initial_row, initial_col);
    int sum = 1;
    solveKTMainStack(tourStack, bkTrackStack, &tour, false, initial_row, initial_col);
    std::cout << "Number of paths: " << std::setw(4) << sum << "  while_count: " << std::setw(10) << while_count << std::endl;
    while (solveKTMainStack(tourStack, bkTrackStack, &tour, true, initial_row, initial_col) == true)
    {
        ++sum;
        if (sum % 100000 == 0)
            std::cout << "Number of paths: " << std::setw(4) << sum << "  while_count: " << std::setw(10) << while_count << std::endl;
        // if (sum % 10000 == 0)
        //     std::cout << std::endl;
    }
    std::cout << "sum: " << sum << std::endl;
*/

    solveKTMainStack(tourStack, bkTrackStack, &tour, false, initial_row, initial_col);
    solveKTMainStack(tourStack, bkTrackStack, &tour, true, initial_row, initial_col);
    solveKTMainStack(tourStack, bkTrackStack, &tour, true, initial_row, initial_col);
    solveKTMainStack(tourStack, bkTrackStack, &tour, true, initial_row, initial_col);

    // solve the Knight Tour problem for a specific starting position generated randomly
    // solveKTMainStack(rand() % BOARD_SIZE, rand() % BOARD_SIZE);

    // stop measuring time and calculate the elapsed time
    stop = clock();
    // duration = timeElapsed(start, finish);
    printElapsedTime(start, stop);

    return 0;
}
