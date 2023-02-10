#include "Camera.h"


struct Camera* Camera_cons(size_t viewHeight, size_t viewWidth, size_t x, size_t y)
{
    struct Camera* cam = calloc(1, sizeof(struct Camera));
    cam->viewHeight = viewHeight;
    cam->viewWidth = viewWidth;
    cam->x = x;
    cam->y = y;

    return cam;
}


void Camera_decons(struct Camera** cam)
{
    free(*cam);
    cam = NULL;
}