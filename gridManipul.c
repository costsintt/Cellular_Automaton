#include "gridManipul.h"


int** createGrid(size_t height, size_t width)
{
    int** newGrid = calloc(height, sizeof(int*));
    for(size_t i = 0; i < height; i++)
    {
        newGrid[i] = calloc(width, sizeof(int));
    }
    return newGrid;
}


void deleteGrid(int*** grid, size_t height, size_t width)
{
    for(size_t i = 0; i < height; i++)
        free((*grid)[i]);
    free(*grid);
    *grid = NULL;
}