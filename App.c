#include "App.h"


struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration)
{
    struct App* app = calloc(1, sizeof(struct App));
    app->screenHeight = screenHeight;
    app->screenWidth = screenWidth;
    app->cam = Camera_cons(APP_CAMERA_DEFAULT_VIEWHEIGHT, APP_CAMERA_DEFAULT_VIEWWIDTH,
                           APP_CAMERA_DEFAULT_X, APP_CAMERA_DEFAULT_Y);
    app->tickDuration = tickDuration;

    return app;
}


void App_decons(struct App** app)
{
    Camera_decons(&((*app)->cam));
    free(*app);
    app = NULL;
}


void App_init(struct App* self)
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
    }

    self->window = SDL_CreateWindow(APP_WINDOW_NAME, APP_WINDOW_DEFAULT_X, APP_WINDOW_DEFAULT_Y,
                                    self->screenWidth, self->screenHeight,
                                    SDL_WINDOW_SHOWN);
    self->renderer =  SDL_CreateRenderer(self->window, -1, 0);

    self->cellHeight = self->screenHeight * 1.0 / self->cam->viewHeight;
    self->cellWidth = self->screenWidth * 1.0 / self->cam->viewWidth;

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    self->keyboard = SDL_GetKeyboardState(NULL);
}


bool App_updateWindow(struct App* self)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    { 
        switch (event.type)
        {
            case SDL_QUIT:
            {
                return false;
                break;
            }
        }
    }

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    return true;
}


void App_drawBoard(struct App* self, struct Board* board)
{
    SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    rect.h = self->cellHeight; rect.w = self->cellWidth;

    size_t max_i = self->cam->y + self->cam->viewHeight;
    size_t max_j = self->cam->x + self->cam->viewWidth;
    if(max_i > board->shape[0]) max_i = board->shape[0];
    if(max_j > board->shape[1]) max_j = board->shape[1];


    size_t n = 0;
    size_t m = 0;
    for(size_t i = self->cam->y; i < max_i; i++)
    {
        for(size_t j = self->cam->x; j < max_j; j++)
        {         
            rect.x = m * self->cellWidth;
            rect.y = n * self->cellHeight;
            if(board->grid[i][j]) SDL_RenderFillRect(self->renderer, &rect);
            m++;
        }
        n++;
        m = 0;
    }
    
}


void App_takeKeyboardInput(struct App* self)
{
    unsigned camVel = 1;
    if(self->keyboard[SDL_SCANCODE_LSHIFT]) camVel = 8;
    if(self->keyboard[SDL_SCANCODE_W] && self->cam->y >= camVel) self->cam->y -= camVel;
    if(self->keyboard[SDL_SCANCODE_S]) self->cam->y += camVel;
    if(self->keyboard[SDL_SCANCODE_A] && self->cam->x >= camVel) self->cam->x -= camVel;
    if(self->keyboard[SDL_SCANCODE_D]) self->cam->x += camVel;
}


void App_clearWindow(struct App* self)
{
    SDL_SetRenderDrawColor(self->renderer, 0, 50, 10, 0);
    SDL_RenderClear(self->renderer);
}


void App_swapWindowBuffers(struct App* self)
{
    SDL_RenderPresent(self->renderer);
}


void App_waitIfNeeded(struct App* self)
{
    uint64_t ticksPassed = SDL_GetTicks64() - self->ticksPassedToTheLatestUpdate;
    SDL_Delay(ticksPassed < self->tickDuration ? self->tickDuration - ticksPassed : 0);
}