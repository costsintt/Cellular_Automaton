#ifndef CAMERA
#define CAMERA

#include <stdlib.h>


struct Camera
{
    size_t viewHeight;
    size_t viewWidth;

    size_t x;
    size_t y;

};


struct Camera* Camera_cons(size_t viewHeight, size_t viewWidth, size_t x, size_t y);


void Camera_decons(struct Camera** cam);


#endif