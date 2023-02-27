#include "gridManipul.h"


struct Cell** createGrid(size_t height, size_t width)
{
    struct Cell** newGrid = calloc(height, sizeof(struct Cell*));
    for(size_t i = 0; i < height; i++)
    {
        newGrid[i] = calloc(width, sizeof(struct Cell));
    }
    return newGrid;
}


void deleteGrid(struct Cell*** grid, size_t height, size_t width)
{
    for(size_t i = 0; i < height; i++)
        free((*grid)[i]);
    free(*grid);
    *grid = NULL;
}


void copyGrid(struct Cell** gridToPaste, struct Cell** gridToCopy, size_t height, size_t width)
{
    for(size_t i = 0; i < height; i++)
    {
        for(size_t j = 0; j < width; j++)
        {
            gridToPaste[i][j] = gridToCopy[i][j];
        }
    }
}


uint8_t randomUInt(uint8_t from, uint8_t to)
{
    return rand() % (to + 1) - from;
}

void Grid_swapCells(struct Cell** grid, size_t i, size_t j, size_t to_i, size_t to_j)
{
    struct Cell buff = grid[to_i][to_j];
    grid[to_i][to_j] = grid[i][j];
    grid[i][j] = buff;
}