#ifndef BOARD
#define BOARD

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "gridManipul.h"

#define BOARD_COUNTER_DIMENSIONS 64

struct Board
{
    int** grid;
    size_t shape[2];

    int counter[BOARD_COUNTER_DIMENSIONS];
    int cellMaxValue;
    int cellMinValue;

    size_t iter;
    bool areBordersMovable;

    void(*countNeighbors)(struct Board* self, size_t i, size_t j);
    int(*_gameRule)(struct Board* self, size_t i, size_t j);

};


struct Board* Board_cons(size_t height, size_t width, int cellMinValue, int cellMaxValue,
                         size_t iter, bool areBordersMovable);


void Board_decons(struct Board** board);


struct Board* Board_copy(struct Board* board);


void Board_print(struct Board* board);


void Board_expand(struct Board* board, size_t n);


void Board_shrink(struct Board* b);


void Board_countNeighbrs(struct Board* self, size_t i, size_t j);


int Board_gameRule1(struct Board* self, size_t i, size_t j);

int Board_gameRule2(struct Board* self, size_t i, size_t j);


void Board_nextIteration(struct Board* self);

void Board_fill(struct Board* board, int whatToFill);

void Board_fillRandom(struct Board* board, int fromWhat, int toWhat);

size_t Board_count(struct Board* board, int32_t cellType);

uint64_t* Board_countInRange(struct Board* board, size_t buffSize,
                       int32_t fromCellType, int32_t toCellType);

#endif