#include "Board.h"


struct Board *Board_cons(size_t height, size_t width, size_t iter, bool areBordersMovable)
{
    int **grid = createGrid(height, width);

    struct Board *board = calloc(1, sizeof(struct Board));
    board->grid = grid;
    board->shape[0] = height;
    board->shape[1] = width;
    board->iter = iter;
    board->areBordersMovable = areBordersMovable;

    board->countNeighbors = &Board_count;
    board->_gameRule = &Board_gameRule1;

    return board;
}


void Board_decons(struct Board **board)
{
    deleteGrid(&((*board)->grid), (*board)->shape[0], (*board)->shape[1]);
    (*board)->shape[0] = 0;
    (*board)->shape[1] = 0;
    free(*board);
    *board = NULL;
}


struct Board *Board_copy(struct Board *b)
{
    struct Board *newBoard = Board_cons(b->shape[0], b->shape[1], b->iter, b->areBordersMovable);
    for (size_t i = 0; i < newBoard->shape[0]; i++)
    {
        for (size_t j = 0; j < newBoard->shape[1]; j++)
        {
            newBoard->grid[i][j] = b->grid[i][j];
        }
    }

    return newBoard;
}


void Board_print(struct Board *board)
{
    for (size_t i = 0; i < board->shape[0]; i++)
    {
        for (size_t j = 0; j < board->shape[1]; j++)
        {
            printf("%d", board->grid[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}


void Board_expand(struct Board *board, size_t n)
{
    int **newGrid = createGrid(board->shape[0] + 2 * n, board->shape[1] + 2 * n);

    for (size_t i = 0; i < board->shape[0]; i++)
        for (size_t j = 0; j < board->shape[1]; j++)
            newGrid[i + n][j + n] = board->grid[i][j];

    deleteGrid(&(board->grid), board->shape[0], board->shape[1]);

    board->shape[0] += 2 * n;
    board->shape[1] += 2 * n;
    board->grid = newGrid;
}


void Board_shrink(struct Board *b)
{
    int isUppermostRowEmpty = 1;
    int isDownmostRowEmpty = 1;
    int isLeftmostColEmpty = 1;
    int isRightmostColEmpty = 1;

    int rowShift = 0;
    int colShift = 0;

    int isNotEnded = 1;
    while (isNotEnded && colShift < b->shape[1]) // leftColumn
    {
        for (int i = 0; i < b->shape[0] && isLeftmostColEmpty; i++)
            if (b->grid[i][colShift])
                isLeftmostColEmpty = 0;

        if (isLeftmostColEmpty)
        {
            colShift += 1;
        }
        else
            isNotEnded = 0;
    }

    if (colShift == b->shape[1]) // if all zone is empty
    {
        deleteGrid(&(b->grid), b->shape[0], b->shape[1]);
        b->grid = createGrid(0, 0);
        b->shape[0] = 0;
        b->shape[1] = 0;
        return;
    }

    isNotEnded = 1;
    while (isNotEnded && colShift < b->shape[1]) // rightColumn
    {
        for (int i = 0; i < b->shape[0] && isRightmostColEmpty; i++)
            if (b->grid[i][b->shape[1] - 1])
                isRightmostColEmpty = 0;

        if (isRightmostColEmpty)
        {
            b->shape[1] -= 1;
        }
        else
            isNotEnded = 0;
    }

    isNotEnded = 1;
    while (isNotEnded && rowShift < b->shape[0]) // uperRow
    {
        for (int j = 0; j < b->shape[1] && isUppermostRowEmpty; j++)
            if (b->grid[rowShift][j])
                isUppermostRowEmpty = 0;

        if (isUppermostRowEmpty)
        {
            rowShift += 1;
        }
        else
            isNotEnded = 0;
    }

    isNotEnded = 1;
    while (isNotEnded && rowShift < b->shape[0]) // downRow
    {
        for (int j = 0; j < b->shape[1] && isDownmostRowEmpty; j++)
            if (b->grid[b->shape[0] - 1][j])
                isDownmostRowEmpty = 0;

        if (isDownmostRowEmpty)
        {
            b->shape[0] -= 1;
        }
        else
            isNotEnded = 0;
    }

    b->shape[0] -= rowShift;
    b->shape[1] -= colShift;

    int **newGrid = createGrid(b->shape[0], b->shape[1]);

    for (int i = 0; i < b->shape[0]; i++)
    {
        for (int j = 0; j < b->shape[1]; j++)
        {
            newGrid[i][j] = b->grid[i + rowShift][j + colShift];
        }
    }
    deleteGrid(&(b->grid), b->shape[0], b->shape[1]);
    b->grid = newGrid;
}


void Board_count(struct Board *self, size_t i, size_t j)
{
    for(unsigned n = 0; n < BOARD_MAXIMUMCELLVALUE - BOARD_MINIMUMCELLVALUE + 1; n++)
        self->counter[n] = 0;

    for (int n = i - 1; n <= i + 1; n++)
    {
        for (int m = j - 1; m <= j + 1; m++)
        {
            if (n > -1 && m > -1 &&
                n < self->shape[0] && m < self->shape[1] && (n != i || m != j))
            {
                self->counter[self->grid[n][m]] += 1;
            }
        }
    }
}


int Board_gameRule1(struct Board* self, size_t i, size_t j)
{
    if(self->counter[1] < 2 || self->counter[1] > 3) return 0;
    else if((self->grid[i][j] && self->counter[1] == 2) || self->counter[1] == 3) return 1;
    else return 0;
}

int Board_gameRule2(struct Board* self, size_t i, size_t j)
{
    return self->grid[i][j];
}


void Board_nextIteration(struct Board* self)
{
    int** newGrid = createGrid(self->shape[0], self->shape[1]);

    for(size_t i = 0; i < self->shape[0]; i++)
    {
        for(size_t j = 0; j < self->shape[1]; j++)
        {
            self->countNeighbors(self, i, j);
            newGrid[i][j] = self->_gameRule(self, i, j);
        }
    }

    deleteGrid(&(self->grid), self->shape[0], self->shape[1]);
    self->grid = newGrid;
}


void Board_fill(struct Board* board, int whatToFill)
{
    for(size_t i = 0; i < board->shape[0]; i++) for(size_t j = 0; j < board->shape[1]; j++)
            board->grid[i][j] = whatToFill;
}

void Board_fillRandom(struct Board* board, int fromWhat, int toWhat)
{
    srand(board->shape[0] + board->shape[1]);
    for(size_t i = 0; i < board->shape[0]; i++) for(size_t j = 0; j < board->shape[1]; j++)
    board->grid[i][j] = rand() % (toWhat + 1) - fromWhat;
}
