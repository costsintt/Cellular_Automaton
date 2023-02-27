#ifndef BOARD
#define BOARD

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "gridManipul.h"

#define BOARD_COUNTER_DIMENSIONS 8

struct Board;
struct Cell
{
    uint8_t type;
    uint64_t livedIterations;

    uint32_t var1;
    uint32_t var2;
    //... add more if needed
};
struct Board
{
    struct Cell** grid;
    struct Cell** buffGrid;
    size_t shape[2];

    uint8_t counter[BOARD_COUNTER_DIMENSIONS];
    uint8_t cellMaxValue;
    uint8_t cellMinValue;

    size_t iter;
    bool areBordersMovable;

    void(*countNeighbors)(struct Board* self, size_t i, size_t j);
    void(*nextIteration)(struct Board* self);

};


struct Board* Board_cons(size_t height, size_t width, uint8_t cellMinValue, uint8_t cellMaxValue,
                         size_t iter, bool areBordersMovable);


void Board_decons(struct Board** board);


struct Board* Board_copy(struct Board* board);


void Board_print(struct Board* board);


void Board_expand(struct Board* board, size_t n);


void Board_shrink(struct Board* b);


void Board_countNeighbrs(struct Board* self, size_t i, size_t j);



void Board_nextIteration_preyAndPred(struct Board* self);

void Board_nextIteration_gameOfLife(struct Board* self);

void Board_fill(struct Board* board, int whatToFill);

void Board_fillRandom(struct Board* board, int fromWhat, int toWhat);

size_t Board_count(struct Board* board, int32_t cellType);

uint64_t* Board_countInRange(struct Board* board, size_t buffSize,
                       int32_t fromCellType, int32_t toCellType);

void Board_moveCellRandomly(struct Board* self, size_t i, size_t j);


#endif