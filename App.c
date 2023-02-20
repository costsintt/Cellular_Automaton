#include "App.h"

static void button_event(kiss_button *button, SDL_Event *e, int *draw, bool *pauseBoardIter)
{
    if (kiss_button_event(button, e, draw)) *pauseBoardIter ^= 1;
}

struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration)
{
    struct App* app = calloc(1, sizeof(struct App));
    app->screenHeight = screenHeight;
    app->screenWidth = screenWidth;
    app->cam = Camera_cons(APP_CAMERA_DEFAULT_VIEWHEIGHTINCELLS,
                           APP_CAMERA_DEFAULT_VIEWWIDTHINCELLS,
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
    kiss_array_new(&(self->objects));

    self->renderer = kiss_init(APP_WINDOW_NAME, &self->objects,
                               self->screenWidth, self->screenHeight);

    kiss_window_new(&(self->main_window), NULL, 0,
                    0, 0, 0, 0,
                    0, 0, self->screenWidth, self->screenHeight);
    kiss_window_new(&(self->buttonsWindow), &self->main_window, 0, 50, 97, 5, 10.0, 0, 0, 0, 0);
    self->buttonsWindow.bg = kiss_blue;

    kiss_button_new(&self->main_pauseButton, &self->buttonsWindow,
                    50, 50, 100, 1.0,
                    kiss_pauseButtonNormal, kiss_pauseButtonActive, kiss_pauseButtonPreLight);

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    self->keyboard = SDL_GetKeyboardState(NULL);

    self->pauseBoardIter = false;
}


bool App_updateWindow(struct App* self)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        kiss_window_event(&(self->main_window), &event, &(self->draw));
        button_event(&(self->main_pauseButton), &event, &(self->draw), &(self->pauseBoardIter));
        if(event.type == SDL_QUIT)
        {
            return false;
            break;
        }
        else if(event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
            self->screenWidth = event.window.data1;
            self->screenHeight = event.window.data2;
            self->main_window.base.rect.w = self->screenWidth;
            self->main_window.base.rect.h = self->screenHeight;

            kiss_genResize((kiss_genData*)&self->buttonsWindow);
            kiss_genRelocate((kiss_genData*)&self->buttonsWindow);

            kiss_genResize((kiss_genData*)&self->main_pauseButton);
            kiss_genRelocate((kiss_genData*)&self->main_pauseButton);

            
        }
    }

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    return true;
}


void App_drawBoard(struct App* self, struct Board* board)
{
    SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
    SDL_Rect rect;
    
    float cellHeight = (float)self->main_window.base.rect.h / self->cam->viewHeightInCells;
    float cellWidth = (float)self->main_window.base.rect.w / self->cam->viewWidthInCells;
    float x = (float)self->main_window.base.rect.x;
    float y = (float)self->main_window.base.rect.y;

    size_t max_i = self->cam->yInCells + self->cam->viewHeightInCells;
    size_t max_j = self->cam->xInCells + self->cam->viewWidthInCells;
    if(max_i > board->shape[0]) max_i = board->shape[0];
    if(max_j > board->shape[1]) max_j = board->shape[1];


    for(size_t i = self->cam->yInCells; i < max_i; i++)
    {
        rect.h = (int)(y + cellHeight) - (int)(y);
        for(size_t j = self->cam->xInCells; j < max_j; j++)
        {         
            rect.w = (int)(x + cellWidth) - (int)(x);
            rect.x = x;
            rect.y = y;
            if(board->grid[i][j])
                SDL_RenderFillRect(self->renderer, &rect);
            x += cellWidth;
        }
        y += (float)cellHeight;
        x = (float)self->main_window.base.rect.x;
    }
    
}


void App_takeKeyboardInput(struct App* self, struct Board* board)
{
    unsigned camVel = 1;
    if(self->keyboard[SDL_SCANCODE_LSHIFT]) camVel = 8;
    if(self->keyboard[SDL_SCANCODE_W] && self->cam->yInCells >= camVel)
        self->cam->yInCells -= camVel;
    if(self->keyboard[SDL_SCANCODE_S] &&
       self->cam->yInCells + self->cam->viewHeightInCells + camVel <= board->shape[0])
        self->cam->yInCells += camVel;
    if(self->keyboard[SDL_SCANCODE_A] && self->cam->xInCells >= camVel)
        self->cam->xInCells -= camVel;
    if(self->keyboard[SDL_SCANCODE_D] &&
       self->cam->xInCells + self->cam->viewWidthInCells + camVel <= board->shape[1])
        self->cam->xInCells += camVel;
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