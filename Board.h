#ifndef BOARD
#define BOARD

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "gridManipul.h"

#define BOARD_COUNTER_DIMENSIONS 8
#define BOARD_MINIMUMCELLVALUE -3
#define BOARD_MAXIMUMCELLVALUE 4

struct Board
{
    int** grid;
    size_t shape[2];

    int counter[BOARD_COUNTER_DIMENSIONS];

    size_t iter;
    bool areBordersMovable;

    void(*countNeighbors)(struct Board* self, size_t i, size_t j);
    int(*_gameRule)(struct Board* self, size_t i, size_t j);

};


struct Board* Board_cons(size_t height, size_t width, size_t iter, bool areBordersMovable);


void Board_decons(struct Board** board);


struct Board* Board_copy(struct Board* board);


void Board_print(struct Board* board);


void Board_expand(struct Board* board, size_t n);


void Board_shrink(struct Board* b);


void Board_count(struct Board* self, size_t i, size_t j);


int Board_gameRule1(struct Board* self, size_t i, size_t j);

int Board_gameRule2(struct Board* self, size_t i, size_t j);


void Board_nextIteration(struct Board* self);

void Board_fill(struct Board* board, int whatToFill);

void Board_fillRandom(struct Board* board, int fromWhat, int toWhat);

#endif