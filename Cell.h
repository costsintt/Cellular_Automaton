#ifndef CELL_H
#define CELL_H

#include <stdint.h>

struct Cell
{
    uint8_t type;
    uint64_t livedIterations;

    int32_t var1;
    int32_t var2;
    //... add more if needed
};


#endif