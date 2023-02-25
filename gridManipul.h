#ifndef GRIDMANIPUL
#define GRIDMANIPUL

#include <stdlib.h>
#include <stdint.h>

#include "Board.h"


struct Cell** createGrid(size_t height, size_t width);


void deleteGrid(struct Cell*** grid, size_t height, size_t width);


void copyGrid(struct Cell** gridToPaste, struct Cell** gridToCopy, size_t height, size_t width);


uint8_t randomUInt(uint8_t from, uint8_t to);

#endif