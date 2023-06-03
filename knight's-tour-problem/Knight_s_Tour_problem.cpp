// solve Knight Tour problem using Backtracking, in a C++ program
#include <bits/stdc++.h>

#define BOARD_SIZE 8 // size of the board
#define EMPTY -1     // empty cell
#define DIRECTIONS 8 // number of possible directions for the Knight

typedef struct
{
    int row, col;
    int degree;
} Cell;

// RowMove[] and ColMove[] define next move of Knight, recording its coordinate offset.
const int RowMove[DIRECTIONS] = {2, 1, -1, -2, -2, -1, 1, 2};
const int ColMove[DIRECTIONS] = {1, 2, 2, 1, -1, -2, -2, -1};

// compare two cells by their degree. if the degree is -1, then the cell is not valid.
bool compareDegree(Cell a, Cell b)
{
    return a.degree < b.degree;
}

// print solution matrix board[BOARD_SIZE][BOARD_SIZE].
// here, each number means the order of the cell visited by the Knight
void printRoute(int board[BOARD_SIZE][BOARD_SIZE])
{
    int row, col;
    for (row = 0; row < BOARD_SIZE; row++)
    {
        for (col = 0; col < BOARD_SIZE; col++)
            std::cout << " " << std::setw(2) << board[row][col] << " ";
        std::cout << std::endl;
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
    if (moveNum == BOARD_SIZE * BOARD_SIZE)
        return true;

	start = rand() % DIRECTIONS;

	for (int k = 0; k < DIRECTIONS; ++k)
	{
        k_offset = (start + k) % DIRECTIONS;
        next_seq[k].row = row + RowMove[k_offset];
        next_seq[k].col = col + ColMove[k_offset];
        next_seq[k].degree = getDegree(board, next_seq[k].row, next_seq[k].col);
	}
    std::sort(next_seq, next_seq + DIRECTIONS, compareDegree); // sort the next move of the Knight by the degree of the next move

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
    If any solution exists, return true and prints the route of Knight's tour.
    If there is no feasible solution, then it prints "Solution does not exist".
    Actually, the function can be modified to print all solutions by using backtracking.

    This function use two initial position as input, with (0,0) as default values.
    For using other initial position, just change the default values of initial_row and initial_col.
*/
int solveKTMain(int initial_row = 0, int initial_col = 0)
{
    int board[BOARD_SIZE][BOARD_SIZE];
    int row, col;

    // Initialization of the board (solution matrix)
    for (row = 0; row < BOARD_SIZE; row++)
        for (col = 0; col < BOARD_SIZE; col++)
            board[row][col] = EMPTY;

    // Since the Knight is initially at the first block
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
        printRoute(board);
        std::cout << std::endl;
    }

    return true;
}

static inline double timeElapsed(clock_t time_start, clock_t time_stop)
{
    return (double)(time_stop - time_start) / CLOCKS_PER_SEC;
}

static inline void printElapsedTime(clock_t time_start, clock_t time_stop)
{
    std::cout << "Time: " << timeElapsed(time_start, time_stop) << " seconds" << std::endl;
}

// Driver Code
int main()
{
    double duration;
    clock_t start, stop;
    int initial_row, initial_col;

    // user's control of the initial position of the Knight
    std::cin >> initial_row >> initial_col;

    // initialize random seed
	srand(time(NULL));

    // start measuring time
    start = clock();

    // solve the Knight Tour problem for all possible starting positions
    // for (int i = 0; i < BOARD_SIZE; ++i)
    //     for (int j = 0; j < BOARD_SIZE; ++j)
    //         solveKTMain(i, j);

    // solve the Knight Tour problem for a specific starting position controlled by the user
    solveKTMain(initial_row, initial_col);

    // solve the Knight Tour problem for a specific starting position generated randomly
    // solveKTMain(rand() % BOARD_SIZE, rand() % BOARD_SIZE);

    // stop measuring time and calculate the elapsed time
    stop = clock();
    // duration = timeElapsed(start, finish);
    printElapsedTime(start, stop);

    // getchar();
    // getchar();

    return 0;
}
