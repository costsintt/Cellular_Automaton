#include "Board.h"


struct Board* Board_cons(size_t height, size_t width, uint8_t cellMinValue, uint8_t cellMaxValue,
                         size_t iter, bool areBordersMovable)
{
    struct Cell** grid = createGrid(height, width);

    struct Board *board = calloc(1, sizeof(struct Board));
    board->grid = grid;
    board->shape[0] = height;
    board->shape[1] = width;
    board->cellMaxValue = cellMaxValue;
    board->cellMinValue = cellMinValue;
    board->iter = iter;
    board->areBordersMovable = areBordersMovable;
    board->countNeighbors = &Board_countNeighbrs;
    board->nextIteration = &Board_nextIteration_preyAndPred;

    return board;
}


void Board_decons(struct Board** board)
{
    deleteGrid(&((*board)->grid), (*board)->shape[0], (*board)->shape[1]);
    (*board)->shape[0] = 0;
    (*board)->shape[1] = 0;
    free(*board);
    *board = NULL;
}


struct Board* Board_copy(struct Board *b)
{
    struct Board* newBoard = Board_cons(b->shape[0], b->shape[1], 0, 1,
                                        b->iter, b->areBordersMovable);
    for (size_t i = 0; i < newBoard->shape[0]; i++)
    {
        for (size_t j = 0; j < newBoard->shape[1]; j++)
        {
            Cell_copyPaste(&newBoard->grid[i][j], &b->grid[i][j]);
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
            printf("%d", board->grid[i][j].type);
        }
        printf("\n");
    }
    printf("\n");
}


/* too useless to spend time remaking it
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
*/

void Board_countNeighbrs(struct Board *self, size_t i, size_t j)
{
    for(uint8_t n = 0; n < self->cellMaxValue - self->cellMinValue + 1; n++)
        self->counter[n] = 0;

    for (size_t n = (i > 0 ? i - 1 : 0); n <= i + 1; n++)
    {
        for (size_t m = (j > 0 ? j - 1 : 0); m <= j + 1; m++)
        {
            if (n < self->shape[0] && m < self->shape[1] && (n != i || m != j))
            {
                self->counter[self->grid[n][m].type] += 1;
            }
        }
    }
}


void Board_nextIteration_preyAndPred(struct Board* self)
{
    uint8_t dir = randomUInt(0, 8);
    struct Cell** newGrid = createGrid(self->shape[0], self->shape[1]);
    copyGrid(newGrid, self->grid, self->shape[0], self->shape[1]);

    struct Cell emptyCell; 
    emptyCell.livedIterations = 0; emptyCell.type = 0; emptyCell.var1 = 0; emptyCell.var2 = 0;
    bool searchNewPosition;
    for(size_t i = 0; i < self->shape[0]; i++)
    {
        for(size_t j = 0; j < self->shape[1]; j++)
        {
            Board_countNeighbrs(self, i, j);
            if(i > 0 && j > 0 && i < self->shape[0] - 1 && j < self->shape[1] - 1)
            {
                if(self->counter[0] && self->grid[i][j].type)
                {
                    searchNewPosition = true;
                    while(searchNewPosition)
                    {
                        for(uint8_t n = 0; n < 3 && searchNewPosition; n++)
                        {
                            for(uint8_t m = 0; m < 3; m++)
                            {
                                if(n == 1 && m == 1) continue;
                                if(!newGrid[i+n-1][j+m-1].type &&
                                   !randomUInt(0, 9))
                                {
                                    newGrid[i+n-1][j+m-1] = self->grid[i][j];
                                    newGrid[i][j] = emptyCell;
                                    searchNewPosition = false;
                                    break;
                                }
                            }
                        }
                    }
                }
            }           
        }
    }



    deleteGrid(&(self->grid), self->shape[0], self->shape[1]);
    self->grid = newGrid;
    self->iter++;
}


void Board_nextIteration_gameOfLife(struct Board* self)
{
    struct Cell** newGrid = createGrid(self->shape[0], self->shape[1]);

    for(size_t i = 0; i < self->shape[0]; i++)
    {
        for(size_t j = 0; j < self->shape[1]; j++)
        {
            self->countNeighbors(self, i, j);
            if(self->counter[1] < 2 || self->counter[1] > 3) newGrid[i][j].type = 0;
            else if((self->grid[i][j].type && self->counter[1] == 2) ||
            self->counter[1] == 3) newGrid[i][j].type = 1;
            else newGrid[i][j].type = 0;
        }
    }

    deleteGrid(&(self->grid), self->shape[0], self->shape[1]);
    self->grid = newGrid;
    self->iter++;
}


void Board_fill(struct Board* board, int whatToFill)
{
    for(size_t i = 0; i < board->shape[0]; i++) for(size_t j = 0; j < board->shape[1]; j++)
            board->grid[i][j].type = whatToFill;
}

void Board_fillRandom(struct Board* board, int fromWhat, int toWhat)
{
    srand(board->shape[0] + board->shape[1]);
    for(size_t i = 0; i < board->shape[0]; i++) for(size_t j = 0; j < board->shape[1]; j++)
            board->grid[i][j].type = randomUInt(fromWhat, toWhat);
}

size_t Board_count(struct Board* board, int32_t cellType)
{
    size_t counter = 0;
    for (size_t i = 0; i < board->shape[0]; i++)
    {
        for (size_t j = 0; j < board->shape[1]; j++)
        {
            if(board->grid[i][j].type == cellType) counter++;
        }
    }
    return counter;
}

uint64_t* Board_countInRange(struct Board* board, size_t buffSize,
                       int32_t fromCellType, int32_t toCellType)
{   
    uint64_t* buff = calloc(buffSize, sizeof(uint64_t));
    size_t iOverBuff = 0;
    for(int32_t i = fromCellType; i <= toCellType; i++)
    {
        buff[iOverBuff++] = Board_count(board, i);
    }
    return buff;
}

void Cell_copyPaste(struct Cell* cellToPaste, struct Cell* cellToCopy)
{
    cellToPaste->livedIterations = cellToCopy->livedIterations;
    cellToPaste->type = cellToCopy->type;
    cellToPaste->var1 = cellToCopy->var1;
    cellToPaste->var2 = cellToCopy->var2;
}