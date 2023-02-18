#ifndef CAMERA
#define CAMERA

#include <stdlib.h>


struct Camera
{
    size_t viewHeightInCells;
    size_t viewWidthInCells;

    size_t xInCells;
    size_t yInCells;

};

struct Camera* Camera_cons(size_t viewHeightInCells, size_t viewWidthInCells,
                           size_t xInCells, size_t yInCells);


void Camera_decons(struct Camera** cam);


#endif