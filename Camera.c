#include "Camera.h"


struct Camera* Camera_cons(size_t viewHeightInCells, size_t viewWidthInCells,
                           size_t xInCells, size_t yInCells)
{
    struct Camera* cam = calloc(1, sizeof(struct Camera));
    cam->viewHeightInCells = viewHeightInCells;
    cam->viewWidthInCells = viewWidthInCells;
    cam->xInCells = xInCells;
    cam->yInCells = yInCells;

    return cam;
}


void Camera_decons(struct Camera** cam)
{
    free(*cam);
    cam = NULL;
}