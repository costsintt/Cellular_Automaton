#include "App.h"

static void drawThickPoint(struct App* self, uint64_t x, uint64_t y, uint64_t thickness,
                           uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    SDL_SetRenderDrawColor(self->renderer, r, g, b, a);
    uint64_t leftPix = thickness / 2;
    uint64_t rightPix = thickness - leftPix;
    for(uint64_t i = x - leftPix; i <= x + rightPix; i++)
    {
        for(uint64_t j = y - leftPix; j <= y + rightPix; j++)
        {
            SDL_RenderDrawPoint(self->renderer, i, j);
        }
    }
}

static void button_pause(kiss_button *button, SDL_Event *e, int *draw, bool *pauseBoardIter)
{
    kiss_button_event(button, e, draw);
    *pauseBoardIter = button->active;
}

static void button_load(kiss_button *button, SDL_Event *e, struct App* app)
{
    if (kiss_button_event(button, e, &app->draw)) app->loadBoard = true;
}

static void button_save(kiss_button *button, SDL_Event *e, struct App* app)
{
    if (kiss_button_event(button, e, &app->draw)) app->copyBoard = true;
}

static void button_nextIter(kiss_button *button, SDL_Event *e, struct App* app)
{
    if(kiss_button_event(button, e, &app->draw)) app->goToNextIter = true; 
}

void graphWindow_draw(kiss_window* window, struct App* app,
                      uint64_t fromX, uint64_t toX, uint64_t fromY, uint64_t toY)
{
    kiss_window_draw(window, app->renderer);
    if(app->datas->len <= fromX || fromX >= toX) return;
    

    uint64_t* xs = calloc(app->datas->len, sizeof(uint64_t));
    uint64_t* ys = calloc(app->datas->len, sizeof(uint64_t));


    size_t i = 0;
    uint64_t xPixels = 0;
    uint64_t yPixels = 0;
    double_t pixelsInOneY = (double_t)window->base.rect.h / (toY - fromY);
    uint64_t y0InPixels = window->base.rect.y + window->base.rect.h;
    for(uint64_t* data = app->datas->begin(app->datas);; data = app->datas->next(app->datas), i++)
    {
        if(fromX > i) continue;
        for(uint8_t n = 1; n < app->lengthOfOneData; n++)
        {
            xPixels = window->base.rect.x + (double_t)window->base.rect.w / (toX - fromX) * (i - fromX);
            yPixels = y0InPixels - pixelsInOneY * data[n];
            drawThickPoint(app, xPixels, yPixels, 1,
                           255*n, 125*n, 50*n, 255);
        }
        if(app->datas->done(app->datas)) break;
    }

}

struct App* App_cons(size_t screenHeight, size_t screenWidth, unsigned tickDuration,
                     uint8_t lengthOfOneData, size_t camViewHeightInCells,
                     size_t camViewWidthInCells, size_t camXInCells, size_t camYInCells,
                     char* appName)
{
    struct App* app = calloc(1, sizeof(struct App));
    app->screenHeight = screenHeight;
    app->screenWidth = screenWidth;
    app->tickDuration = tickDuration;
    app->lengthOfOneData = lengthOfOneData;

    app->name = appName;

    size_t camViewSize = camViewHeightInCells < camViewWidthInCells
                         ? camViewHeightInCells : camViewWidthInCells;
    app->cam = Camera_cons(camViewSize, camViewSize, camXInCells, camYInCells);
    
    app->datas = sList_cons();

    App_initDrawingThings(app);

    return app;
}


void App_decons(struct App** app)
{
    Camera_decons(&((*app)->cam));
    free(*app);
    app = NULL;
}


void App_initDrawingThings(struct App* self)
{
    kiss_array_new(&(self->objects));

    self->renderer = kiss_init(self->name, &self->objects,
                               self->screenWidth, self->screenHeight);

    SDL_SetRenderDrawBlendMode(self->renderer, SDL_BLENDMODE_BLEND);

    kiss_window_new(&(self->main_window), NULL,
                    0, 0, 100, 0, 255,
                    0, 0, 0, 0, 0.0, 1, 1,
                    0, 0, self->screenWidth, self->screenHeight);
    kiss_window_new(&(self->test_window), &self->main_window,
                    0, 0, 120, 25, 255,
                    50, 50, 50, 50, 0.0, 1, 1,
                    0, 0, 0, 0);
    kiss_window_new(&(self->buttonsWindow), &self->main_window,
                    0, 95, 95, 95, 200,
                    50, 97, 100, 6, 0.0, 1, 1,
                    0, 0, 0, 0);
    kiss_window_new(&(self->graphWindow), &self->main_window,
                    1, 0, 0, 0, 75,
                    74, 26, 50, 50, 0.0, 1, 1,
                    0, 0, 0, 0);

    kiss_button_new(&self->pauseButton, &self->buttonsWindow,
                    50, 50, 5, 80, 1.0,
                    kiss_pauseButtonNormal, kiss_pauseButtonActive,
                    kiss_pauseButtonPreLight, true);
    self->pauseButton.active = 1;
    
    kiss_button_new(&self->loadButton, &self->buttonsWindow,
                    40, 50, 5, 80, 1.0,
                    kiss_loadButtonNormal, kiss_loadButtonActive, kiss_loadButtonPreLight, false);

    kiss_button_new(&self->saveButton, &self->buttonsWindow,
                    60, 50, 5, 80, 1.0,
                    kiss_saveButtonNormal, kiss_saveButtonActive,
                    kiss_saveButtonPreLight, false);
    kiss_button_new(&self->nextIterButton, &self->buttonsWindow,
                    70, 50, 5, 80, 1.0,
                    kiss_nextIterButtonNormal, kiss_nextIterButtonActive,
                    kiss_nextIterButtonPreLight, false);

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    self->keyboard = SDL_GetKeyboardState(NULL);

    self->pauseBoardIter = true;
    self->goToNextIter = false;
}


bool App_updateWindow(struct App* self)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        kiss_window_event(&self->main_window, &event, &self->draw);
        kiss_window_event(&self->buttonsWindow, &event, &self->draw);
        kiss_window_event(&self->graphWindow, &event, &self->draw);
        button_pause(&self->pauseButton, &event, &self->draw, &self->pauseBoardIter);
        button_load(&self->loadButton, &event, self);
        button_save(&self->saveButton, &event, self);
        button_nextIter(&self->nextIterButton, &event, self);
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

            kiss_genResize((kiss_genData*)&self->test_window);
            kiss_genRelocate((kiss_genData*)&self->test_window);

            kiss_genResize((kiss_genData*)&self->buttonsWindow);
            kiss_genRelocate((kiss_genData*)&self->buttonsWindow);

            kiss_genResize((kiss_genData*)&self->graphWindow);
            kiss_genRelocate((kiss_genData*)&self->graphWindow);

            kiss_genResize((kiss_genData*)&self->pauseButton);
            kiss_genRelocate((kiss_genData*)&self->pauseButton);

            kiss_genResize((kiss_genData*)&self->loadButton);
            kiss_genRelocate((kiss_genData*)&self->loadButton);

            kiss_genResize((kiss_genData*)&self->saveButton);
            kiss_genRelocate((kiss_genData*)&self->saveButton);

            kiss_genResize((kiss_genData*)&self->nextIterButton);
            kiss_genRelocate((kiss_genData*)&self->nextIterButton);

            
        }
    }

    self->ticksPassedToTheLatestUpdate = SDL_GetTicks64();

    return true;
}


void App_drawBoard(kiss_window* window, struct App* self, struct Board* board)
{
    kiss_window_draw(window, self->renderer);
    SDL_Rect rect;
    
    float cellHeight = (float)window->base.rect.h / self->cam->viewHeightInCells;
    float cellWidth = (float)window->base.rect.w / self->cam->viewWidthInCells;
    float x = (float)window->base.rect.x;
    float y = (float)(window->base.rect.y + window->base.rect.h) - cellHeight;

    size_t max_i = self->cam->yInCells + self->cam->viewHeightInCells;
    size_t max_j = self->cam->xInCells + self->cam->viewWidthInCells;
    if(max_i > board->shape[0]) max_i = board->shape[0];
    if(max_j > board->shape[1]) max_j = board->shape[1];


    for(size_t i = self->cam->yInCells; i < max_i; i++)
    {
        rect.h = (int)(y + cellHeight) - (int)(y) - 1;
        for(size_t j = self->cam->xInCells; j < max_j; j++)
        {         
            rect.w = (int)(x + cellWidth) - (int)(x) - 1;
            rect.x = x;
            rect.y = y;
            if(board->grid[i][j].type == 1)
            {
                SDL_SetRenderDrawColor(self->renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(self->renderer, &rect);
            }
            else if(board->grid[i][j].type == 2)
            {
                SDL_SetRenderDrawColor(self->renderer, 255, 155, 155, 255);
                SDL_RenderFillRect(self->renderer, &rect);
            }
            else if(board->grid[i][j].type == 3)
            {
                SDL_SetRenderDrawColor(self->renderer, 255, 55, 55, 255);
                SDL_RenderFillRect(self->renderer, &rect);
            }
                
            x += cellWidth;
        }
        y -= (float)cellHeight;
        x = (float)window->base.rect.x;
    }
    
}


void App_updateCamera(struct App* self, struct Board* board)
{
    uint32_t camVel = 1;
    uint32_t zoomVel = 1;
    
    if(self->keyboard[SDL_SCANCODE_W] && !App_willCameraClip(self, board, camVel, DIR_UPWARD))
        self->cam->yInCells += camVel;

    if(self->keyboard[SDL_SCANCODE_S] && !App_willCameraClip(self, board, camVel, DIR_DOWNWARD))
        self->cam->yInCells -= camVel;

    if(self->keyboard[SDL_SCANCODE_D] && !App_willCameraClip(self, board, camVel, DIR_RIGHTWARD))
        self->cam->xInCells += camVel;

    if(self->keyboard[SDL_SCANCODE_A] && !App_willCameraClip(self, board, camVel, DIR_LEFTWARD))
        self->cam->xInCells -= camVel;


    if(self->keyboard[SDL_SCANCODE_Q] && self->cam->viewHeightInCells > zoomVel
    && self->cam->viewWidthInCells > zoomVel)
    {
        self->cam->viewHeightInCells -= zoomVel;
        self->cam->viewWidthInCells -= zoomVel;
        if(self->cam->viewWidthInCells % 2
           && !App_willCameraClip(self, board, zoomVel, DIR_RIGHTWARD)
           && !App_willCameraClip(self, board, zoomVel - 1, DIR_LEFTWARD))
        {
            self->cam->xInCells += zoomVel;   
        }
        if(self->cam->viewHeightInCells % 2
           && !App_willCameraClip(self, board, zoomVel, DIR_UPWARD)
           && !App_willCameraClip(self, board, zoomVel - 1, DIR_DOWNWARD))
        {
            self->cam->yInCells += zoomVel;   
        }
    }

    if(self->keyboard[SDL_SCANCODE_E]
    && self->cam->viewHeightInCells + zoomVel <= board->shape[0]
    && self->cam->viewWidthInCells + zoomVel <= board->shape[1])
    {
        self->cam->viewHeightInCells += zoomVel;
        self->cam->viewWidthInCells += zoomVel;
        if(self->cam->viewWidthInCells % 2
           && !App_willCameraClip(self, board, zoomVel, DIR_RIGHTWARD)
           && !App_willCameraClip(self, board, zoomVel, DIR_LEFTWARD))
        {
            self->cam->xInCells -= zoomVel;   
        }
        if(self->cam->viewHeightInCells % 2
           && !App_willCameraClip(self, board, zoomVel, DIR_UPWARD)
           && !App_willCameraClip(self, board, zoomVel, DIR_DOWNWARD))
        {
            self->cam->yInCells -= zoomVel;   
        }
    }

    while(App_willCameraClip(self, board, 0, DIR_UPWARD))
        self->cam->yInCells--;
    while(App_willCameraClip(self, board, 0, DIR_RIGHTWARD))
        self->cam->xInCells--;

    
}


void App_clearWindow(struct App* self)
{
    SDL_SetRenderDrawColor(self->renderer, 255, 0, 0, 0);
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

bool App_willCameraClip(struct App* self, struct Board* board,
                        size_t vel, uint8_t dir)
{
    switch(dir)
    {
        case DIR_RIGHTWARD:
        if(self->cam->xInCells + self->cam->viewWidthInCells + vel > board->shape[1]) 
            return 1;
        break;

        case DIR_UPWARD:
        if(self->cam->yInCells + self->cam->viewHeightInCells + vel > board->shape[0])
            return 1;
        break;

        case DIR_LEFTWARD:
        if(self->cam->xInCells < vel)
            return 1;
        break;

        case DIR_DOWNWARD:
        if(self->cam->yInCells < vel)
            return 1;
        break;
    }
    return 0;
}