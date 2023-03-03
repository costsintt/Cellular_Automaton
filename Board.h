#ifndef BOARD
#define BOARD

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

#include "gridManipul.h"

#define BOARD_COUNTER_DIMENSIONS 8

struct Board;
struct Cell
{
    uint8_t type;
    uint64_t livedIterations;

    int32_t var1;
    int32_t var2;
    //... add more if needed
};
struct Board
{
    struct Cell** grid;
    struct Cell** buffGrid;
    size_t shape[2];

    uint8_t counter[BOARD_COUNTER_DIMENSIONS];
    uint8_t cellMinValue;
    uint8_t cellMaxValue;
    
    uint64_t iter;
    bool areBordersMovable;

    void(*countNeighbors)(struct Board* self, size_t i, size_t j);
    void(*nextIteration)(struct Board* self);

};


struct Board* Board_cons(size_t height, size_t width, uint8_t cellMinValue, uint8_t cellMaxValue,
                         uint64_t iter, bool areBordersMovable);


void Board_decons(struct Board** board);


struct Board* Board_copy(struct Board* board);


void Board_print(struct Board* board);


// void Board_expand(struct Board* board, size_t n);


// void Board_shrink(struct Board* b);


void Board_countNeighbrs(struct Board* self, size_t i, size_t j);



void Board_nextIteration_preyAndPred(struct Board* self);

//void Board_nextIteration_gameOfLife(struct Board* self);

void Board_fill(struct Board* board, struct Cell whomToFill,
                size_t x0, size_t y0, size_t x1, size_t y1);

void Board_fillRandom(struct Board* board, size_t argCount, ...);

uint64_t Board_count(struct Board* board, int8_t cellType);

uint64_t* Board_countInRange(struct Board* board, size_t buffSize,
                       int32_t fromCellType, int32_t toCellType);

uint32_t Board_createCellRandomly(struct Board* self, struct Cell cellToCreate,
                                  size_t i0, size_t j0);

uint32_t Board_moveCellRandomly(struct Board* self, size_t i, size_t j);


#endif