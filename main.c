#include <SDL2/SDL.h>
#include"main.h"


#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define GRID_SIZE 10

#define NUMBER_OF_BUTTONS 3


#define BUTTON_ROW_START (SCREEN_HEIGHT-100)

#define BUTTON_WIDTH ((int)(SCREEN_WIDTH/NUMBER_OF_BUTTONS))
#define BUTTON_HEIGHT 100



#define RED  255, 0, 0, 255     // Red color
#define GREEN  0, 255, 0, 255   // Green color
#define BLUE  0, 0, 255, 255    // Blue color


#define TRUE 1U
#define FALSE 0U

/*
// typedef struct
// {
//     int x;
//     int y;
//     int width;
//     int height;
//     int isPressed;

// } Button;

// struct Button startButton;
// struct Button endButton;
// struct Button clearButton;

// //PROTOTYPES
// void initializeButton(struct Button *button, int x, int y, int width, int height, int isPressed);


// void initializeButton(struct Button *button, int x, int y, int width, int height, int isPressed)
// {
//     button->x = x;
//     button->y = y;
//     button->width = width;
//     button->height = height;
//     button->isPressed = isPressed;
// }

// void initButtons()
// {
//     initializeButton(&startButton, 100, 100, 200, 100, FALSE);
// }
*/


struct Button{
    int x;
    int y;
    int width;
    int height;
    int isPressed;
};

struct Button startButton;
struct Button endButton;
struct Button clearButton;

// Prototype declaration
void initializeButton(struct Button *button, int x, int y, int width, int height, int isPressed);

// Function definition
void initializeButton(struct Button *button, int x, int y, int width, int height, int isPressed) {
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->isPressed = isPressed;
}

void initButtons() 
{
    //start button
    initializeButton(&startButton, 0, BUTTON_ROW_START, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);
}

void drawGrid(SDL_Renderer *renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set grid color to white

    int cellSize = SCREEN_WIDTH / GRID_SIZE; // Calculate size of each grid cell

    // Draw vertical lines
    for (int x = 0; x <= SCREEN_WIDTH; x += cellSize)
    {

        SDL_RenderDrawLine(renderer, x, 0, x, SCREEN_HEIGHT);
    }

    // Draw horizontal lines
    for (int y = 0; y <= SCREEN_HEIGHT; y += cellSize)
    {

        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }
}

int main()
{
    initButtons();
    
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    // Main loop
    int running = 1;
    while (running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = SDL_FALSE;
                break;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
        SDL_RenderClear(renderer);

        // Draw your grid or game here using SDL functions
        // drawGrid(renderer);

        // Draw button
        SDL_SetRenderDrawColor(renderer, BLUE); // Green color
        SDL_Rect buttonRect = {startButton.x, startButton.y, startButton.width, startButton.height};
        SDL_RenderFillRect(renderer, &buttonRect);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
