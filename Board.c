#include "Board.h"


struct Board* Board_cons(size_t height, size_t width, uint8_t cellMinValue, uint8_t cellMaxValue,
                         uint64_t iter, bool areBordersMovable)
{
    struct Board *board = calloc(1, sizeof(struct Board));
    board->grid = createGrid(height, width);
    board->buffGrid = createGrid(height, width);
    board->shape[0] = height;
    board->shape[1] = width;
    board->cellMinValue = cellMinValue;
    board->cellMaxValue = cellMaxValue;
    board->iter = iter;
    board->areBordersMovable = areBordersMovable;
    board->countNeighbors = &Board_countNeighbrs;
    board->nextIteration = &Board_nextIteration_preyAndPred;

    return board;
}


void Board_decons(struct Board** board)
{
    deleteGrid(&((*board)->grid), (*board)->shape[0], (*board)->shape[1]);
    deleteGrid(&((*board)->buffGrid), (*board)->shape[0], (*board)->shape[1]);
    (*board)->shape[0] = 0;
    (*board)->shape[1] = 0;
    free(*board);
    *board = NULL;
}


struct Board* Board_copy(struct Board *b)
{
    struct Board* newBoard = Board_cons(b->shape[0], b->shape[1], 0, 1,
                                        b->iter, b->areBordersMovable);
    copyGrid(newBoard->grid, b->grid, b->shape[0], b->shape[1]);
    copyGrid(newBoard->buffGrid, b->buffGrid, b->shape[0], b->shape[1]);

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

    for(size_t i = 0; i < self->shape[0]; i++)
    {
        for(size_t j = 0; j < self->shape[1]; j++)
        {
            Board_countNeighbrs(self, i, j);
            if(self->counter[0] && self->grid[i][j].type)
            {
                Board_moveCellRandomly(self, i, j);
            }
        }
    }

    for(size_t i = 0; i < self->shape[0]; i++)
    {
        for(size_t j = 0; j < self->shape[1]; j++)
        {
            Board_countNeighbrs(self, i, j);

            if(self->grid[i][j].type == 1)
            {
                if(self->grid[i][j].var1 > 15)
                {
                    if(self->counter[0])
                    {
                        Board_createCellRandomly(self,
                        (struct Cell){self->grid[i][j].type, 0, 0, 0}, i, j);
                        self->grid[i][j].var1 = 0;
                    }
                }
                else
                {
                    self->grid[i][j].var1++;
                    self->grid[i][j].var1 -= self->counter[2];
                }
            }

            else if(self->grid[i][j].type == 2)
            {
                if(self->grid[i][j].var1 > 30)
                {
                    if(self->counter[0])
                    {
                        Board_createCellRandomly(self,
                        (struct Cell){self->grid[i][j].type, 0, 15, 0}, i, j);
                        self->grid[i][j].var1 -= 15;
                    }
                }
                else
                {
                    if(self->counter[1]) self->grid[i][j].var1 += self->counter[1]*2;
                    else self->grid[i][j].var1--;
                }
            }

            if(self->grid[i][j].var1 < 0) self->grid[i][j] = (struct Cell){0,0,0,0};
            
        }
    }

    self->iter++;
}

//remake
// void Board_nextIteration_gameOfLife(struct Board* self)
// {
//     struct Cell** newGrid = createGrid(self->shape[0], self->shape[1]);

//     for(size_t i = 0; i < self->shape[0]; i++)
//     {
//         for(size_t j = 0; j < self->shape[1]; j++)
//         {
//             self->countNeighbors(self, i, j);
//             if(self->counter[1] < 2 || self->counter[1] > 3) newGrid[i][j].type = 0;
//             else if((self->grid[i][j].type && self->counter[1] == 2) ||
//             self->counter[1] == 3) newGrid[i][j].type = 1;
//             else newGrid[i][j].type = 0;
//         }
//     }

//     deleteGrid(&(self->grid), self->shape[0], self->shape[1]);
//     self->grid = newGrid;
//     self->iter++;
// }


void Board_fill(struct Board* board, uint8_t whatToFill, size_t x0, size_t y0, size_t x1, size_t y1)
{
    for(size_t i = y0; i <= y1; i++)
        for(size_t j = x0; j <= x1; j++)
            board->grid[i][j].type = whatToFill;
}

void Board_fillRandom(struct Board* board, size_t argCount, ...)
{
    size_t buffSize = 16;
    if(argCount > buffSize) return;

    va_list args;
    va_start(args, argCount);
    struct Cell* buff = calloc(buffSize, sizeof(struct Cell));
    for(size_t i = 0; i < argCount; i++)
    {
        buff[i] = va_arg(args, struct Cell);
    }
    for(size_t i = 0; i < board->shape[0]; i++)
    {
        for(size_t j = 0; j < board->shape[1]; j++)
        {
            board->grid[i][j] = buff[randomUInt(0, argCount - 1)];
        }
    }

}

uint64_t Board_count(struct Board* board, int8_t cellType)
{
    uint64_t counter = 0;
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
    for(uint8_t i = fromCellType; i <= toCellType; i++)
    {
        buff[iOverBuff++] = Board_count(board, i);
    }
    return buff;
}

uint32_t Board_createCellRandomly(struct Board* self, struct Cell cellToCreate,
                                  size_t i0, size_t j0)
{
    //012 <-dir meaning
    //7 3
    //654
    uint8_t dir = randomUInt(0, 7);
    if(dir == 0) return 0;
    else if(dir == 1 && i0 < self->shape[0] - 1 && j0 != 0 && self->grid[i0+1][j0-1].type == 0) self->grid[i0+1][j0-1] = cellToCreate;
    else if(dir == 2 && i0 < self->shape[0] - 1 && self->grid[i0+1][j0].type == 0) self->grid[i0+1][j0] = cellToCreate;
    else if(dir == 3 && i0 < self->shape[0] - 1 && j0 < self->shape[1] - 1 && self->grid[i0+1][j0+1].type == 0) self->grid[i0+1][j0+1] = cellToCreate;
    else if(dir == 4 && j0 < self->shape[1] - 1 && self->grid[i0][j0+1].type == 0) self->grid[i0][j0+1] = cellToCreate;
    else if(dir == 5 && i0 != 0 && j0 < self->shape[1] - 1 && self->grid[i0-1][j0+1].type == 0) self->grid[i0-1][j0+1] = cellToCreate;
    else if(dir == 6 && i0 != 0 && self->grid[i0-1][j0].type == 0) self->grid[i0-1][j0] = cellToCreate;
    else if(dir == 7 && i0 != 0 && j0 != 0 && self->grid[i0-1][j0-1].type == 0) self->grid[i0-1][j0-1] = cellToCreate;
    else if(dir == 8 && j0 != 0 && self->grid[i0][j0-1].type == 0) self->grid[i0][j0-1] = cellToCreate;
    else return 1;
    return 0;
}

uint32_t Board_moveCellRandomly(struct Board* self, size_t i, size_t j)
{   //123 <-dir meaning
    //804
    //765
    uint8_t dir = randomUInt(0, 8);
    if(dir == 0) return 0;
    else if(dir == 1 && i < self->shape[0] - 1 && j != 0 && self->grid[i+1][j-1].type == 0) Grid_swapCells(self->grid, i, j, i+1, j-1);
    else if(dir == 2 && i < self->shape[0] - 1 && self->grid[i+1][j].type == 0) Grid_swapCells(self->grid, i, j, i+1, j);
    else if(dir == 3 && i < self->shape[0] - 1 && j < self->shape[1] - 1 && self->grid[i+1][j+1].type == 0) Grid_swapCells(self->grid, i, j, i+1, j+1);
    else if(dir == 4 && j < self->shape[1] - 1 && self->grid[i][j+1].type == 0) Grid_swapCells(self->grid, i, j, i, j+1);
    else if(dir == 5 && i != 0 && j < self->shape[1] - 1 && self->grid[i-1][j+1].type == 0) Grid_swapCells(self->grid, i, j, i-1, j+1);
    else if(dir == 6 && i != 0 && self->grid[i-1][j].type == 0) Grid_swapCells(self->grid, i, j, i-1, j);
    else if(dir == 7 && i != 0 && j != 0 && self->grid[i-1][j-1].type == 0) Grid_swapCells(self->grid, i, j, i-1, j-1);
    else if(dir == 8 && j != 0 && self->grid[i][j-1].type == 0) Grid_swapCells(self->grid, i, j, i, j-1);
    else return 1;
    return 0;
}