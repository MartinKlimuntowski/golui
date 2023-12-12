#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
#include <unistd.h> 

struct Button
{
    int x;
    int y;
    int width;
    int height;
    int isPressed;
};

struct Cell
{
    int x;
    int y;
    int current;
    int next;
};

struct Cell cellArray[_H][_V]; //[H][V]

struct Button startButton;
struct Button clearButton;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void initCells()
{
    int cellSize = CELL_SIZE_CONST - 1; // Set the size of each grid cell to a fixed pixel size

    for (int i = 0; i <= _H; i++)
    {
        for (int j = 0; j <= _V; j++)
        {
            cellArray[i][j].x = cellSize * i;
            cellArray[i][j].y = cellSize * j;
            cellArray[i][j].current = FALSE;
        }
    }


    //some starting cells for dev:
    //234
    //OOX 2
    //XOX 3
    //OXX 4

    cellArray[4][2].current = TRUE;
    
    cellArray[2][3].current = TRUE;
    cellArray[4][3].current = TRUE;

    cellArray[3][4].current = TRUE;
    cellArray[4][4].current = TRUE;

}

int getNeighbours(int i, int j) {
    int neighbors = 0;

    int minRow = (i - 1 < 0) ? 0 : i - 1;
    int maxRow = (i + 1 >= _H) ? _H - 1 : i + 1;
    int minCol = (j - 1 < 0) ? 0 : j - 1;
    int maxCol = (j + 1 >= _V) ? _V - 1 : j + 1;

    for (int row = minRow; row <= maxRow; row++) {
        for (int col = minCol; col <= maxCol; col++) {
            if (!(row == i && col == j)) {
                neighbors += cellArray[row][col].current;
            }
        }
    }

    return neighbors;
}

void calculateCells()
{

    for (int i = 0; i < (SCREEN_WIDTH / (CELL_SIZE_CONST - 1)); i++)
    {
        for (int j = 0; j < (GRID_HEIGHT / (CELL_SIZE_CONST - 1)); j++)
        {
            // Any live cell with fewer than two live neighbours dies, as if by underpopulation.
            // Any live cell with two or three live neighbours lives on to the next generation.
            // Any live cell with more than three live neighbours dies, as if by overpopulation.
            // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

            int neighbours = getNeighbours(i, j);

            if (cellArray[i][j].current)
            { // If the cell is alive
                if (neighbours < 2 || neighbours > 3)
                {
                    cellArray[i][j].next = FALSE;
                }
                else
                {
                    cellArray[i][j].next = TRUE;
                }
            }
            else
            { // If the cell is dead
                if (neighbours == 3)
                {
                    cellArray[i][j].next = TRUE;
                }
                else
                {
                    cellArray[i][j].next = FALSE;
                }
            }
        }
    }
}

void initializeButton(struct Button *button, int x, int y, int width, int height, int isPressed)
{
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->isPressed = isPressed;
}

void initButtons()
{
    // start button
    initializeButton(&startButton, 0, BUTTON_ROW_START, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);

    // button
    initializeButton(&clearButton, (0 + BUTTON_WIDTH), BUTTON_ROW_START, BUTTON_WIDTH, BUTTON_HEIGHT, FALSE);
}

void drawButtons()
{
    // Draw button
    SDL_SetRenderDrawColor(renderer, GREY_PRESSED);
    SDL_Rect startButtonRect = {startButton.x, startButton.y, startButton.width, startButton.height};
    SDL_RenderFillRect(renderer, &startButtonRect);

    // Border top
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_Rect startButtonBorder1 = {startButton.x, startButton.y, startButton.width - BORDER_WIDTH, BORDER_WIDTH};
    SDL_RenderFillRect(renderer, &startButtonBorder1);

    // border left
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_Rect startButtonBorder2 = {startButton.x, startButton.y, BORDER_WIDTH, startButton.height};
    SDL_RenderFillRect(renderer, &startButtonBorder2);

    // Border bottom
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_Rect startButtonBorder3 = {(startButton.x + BORDER_WIDTH), ((startButton.y + startButton.height)) - BORDER_WIDTH, startButton.width, BORDER_WIDTH};
    SDL_RenderFillRect(renderer, &startButtonBorder3);

    // border right
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_Rect startButtonBorder4 = {((startButton.x + startButton.width) - BORDER_WIDTH), startButton.y + BORDER_WIDTH, BORDER_WIDTH, startButton.height};
    SDL_RenderFillRect(renderer, &startButtonBorder4);

    //////////////////
    SDL_SetRenderDrawColor(renderer, GREY);
    SDL_Rect clearButtonRect = {clearButton.x, clearButton.y, clearButton.width, clearButton.height};
    SDL_RenderFillRect(renderer, &clearButtonRect);

    // Border top
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_Rect clearButtonBorder1 = {clearButton.x, clearButton.y, clearButton.width - BORDER_WIDTH, BORDER_WIDTH};
    SDL_RenderFillRect(renderer, &clearButtonBorder1);

    // border left
    SDL_SetRenderDrawColor(renderer, WHITE);
    SDL_Rect clearButtonBorder2 = {clearButton.x, clearButton.y, BORDER_WIDTH, clearButton.height};
    SDL_RenderFillRect(renderer, &clearButtonBorder2);

    // Border bottom
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_Rect clearButtonBorder3 = {(clearButton.x + BORDER_WIDTH), ((clearButton.y + clearButton.height)) - BORDER_WIDTH, clearButton.width, BORDER_WIDTH};
    SDL_RenderFillRect(renderer, &clearButtonBorder3);

    // border right
    SDL_SetRenderDrawColor(renderer, BLACK);
    SDL_Rect clearButtonBorder4 = {((clearButton.x + clearButton.width) - BORDER_WIDTH), clearButton.y + BORDER_WIDTH, BORDER_WIDTH, clearButton.height};
    SDL_RenderFillRect(renderer, &clearButtonBorder4);
}

/*
// Function pointers for drawing the button and its borders
typedef void (*DrawFunction)(SDL_Renderer*, Button*);

struct Button {
    int x;
    int y;
    int width;
    int height;
    int isPressed;
    int borderWidth; // Border width
    Uint32 borderColorTop; // Border color for top
    Uint32 borderColorLeft; // Border color for left
    Uint32 borderColorBottom; // Border color for bottom
    Uint32 borderColorRight; // Border color for right
    Uint32 backgroundColor; // Button's background color
    DrawFunction draw; // Function pointer for drawing the button
};

// Function to draw the button
void drawButton(SDL_Renderer* renderer, Button* button) {
    // Draw button background
    SDL_SetRenderDrawColor(renderer, (button->backgroundColor >> 24) & 0xFF,
                                       (button->backgroundColor >> 16) & 0xFF,
                                       (button->backgroundColor >> 8) & 0xFF,
                                       button->backgroundColor & 0xFF);
    SDL_Rect buttonRect = { button->x, button->y, button->width, button->height };
    SDL_RenderFillRect(renderer, &buttonRect);

    // Draw top border
    SDL_SetRenderDrawColor(renderer, (button->borderColorTop >> 24) & 0xFF,
                                      (button->borderColorTop >> 16) & 0xFF,
                                      (button->borderColorTop >> 8) & 0xFF,
                                      button->borderColorTop & 0xFF);
    SDL_Rect topBorderRect = { button->x, button->y, button->width - button->borderWidth, button->borderWidth };
    SDL_RenderFillRect(renderer, &topBorderRect);

    // Draw left border
    SDL_SetRenderDrawColor(renderer, (button->borderColorLeft >> 24) & 0xFF,
                                      (button->borderColorLeft >> 16) & 0xFF,
                                      (button->borderColorLeft >> 8) & 0xFF,
                                      button->borderColorLeft & 0xFF);
    SDL_Rect leftBorderRect = { button->x, button->y, button->borderWidth, button->height };
    SDL_RenderFillRect(renderer, &leftBorderRect);

    // Draw bottom border
    SDL_SetRenderDrawColor(renderer, (button->borderColorBottom >> 24) & 0xFF,
                                      (button->borderColorBottom >> 16) & 0xFF,
                                      (button->borderColorBottom >> 8) & 0xFF,
                                      button->borderColorBottom & 0xFF);
    SDL_Rect bottomBorderRect = { button->x + button->borderWidth, button->y + button->height - button->borderWidth,
                                  button->width - button->borderWidth, button->borderWidth };
    SDL_RenderFillRect(renderer, &bottomBorderRect);

    // Draw right border
    SDL_SetRenderDrawColor(renderer, (button->borderColorRight >> 24) & 0xFF,
                                      (button->borderColorRight >> 16) & 0xFF,
                                      (button->borderColorRight >> 8) & 0xFF,
                                      button->borderColorRight & 0xFF);
    SDL_Rect rightBorderRect = { button->x + button->width - button->borderWidth, button->y + button->borderWidth,
                                 button->borderWidth, button->height - button->borderWidth * 2 };
    SDL_RenderFillRect(renderer, &rightBorderRect);
}

// Function to initialize a button
void initButton(Button* button, int x, int y, int width, int height) {
    button->x = x;
    button->y = y;
    button->width = width;
    button->height = height;
    button->isPressed = 0;
    button->borderWidth = 2;
    button->backgroundColor = GREY;
    button->borderColorTop = WHITE;
    button->borderColorLeft = WHITE;
    button->borderColorBottom = BLACK;
    button->borderColorRight = BLACK;
    button->draw = drawButton;
}
*/

void drawGrid()
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Set grid color to white

    int cellSize = CELL_SIZE_CONST - 1; // Set the size of each grid cell to a fixed pixel size

    // Draw vertical lines
    for (int x = 0; x <= SCREEN_WIDTH; x += cellSize)
    {
        SDL_RenderDrawLine(renderer, x, 0, x, GRID_HEIGHT);
    }

    // Draw horizontal lines
    for (int y = 0; y <= GRID_HEIGHT; y += cellSize)
    {
        SDL_RenderDrawLine(renderer, 0, y, SCREEN_WIDTH, y);
    }

    // cosmetic
    SDL_RenderDrawLine(renderer, (SCREEN_WIDTH - 1), 0, (SCREEN_WIDTH - 1), GRID_HEIGHT);
}

void drawSquares()
{
    for (int i = 0; i < (SCREEN_WIDTH / (CELL_SIZE_CONST - 1)); i++)
    {
        for (int j = 0; j < (GRID_HEIGHT / (CELL_SIZE_CONST - 1)); j++)
        {
            if(cellArray[i][j].next)
            {
                SDL_SetRenderDrawColor(renderer, WHITE);
            }
            else
            {
                SDL_SetRenderDrawColor(renderer, BLACK);
            }

            // SDL_SetRenderDrawColor(renderer, RED);
            SDL_Rect tempRect = {cellArray[i][j].x, cellArray[i][j].y, CELL_SIZE, CELL_SIZE};
            SDL_RenderFillRect(renderer, &tempRect);
        }
    }
}

int main()
{
    initCells();
    initButtons();

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Game of Life", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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

        // draw buttons
        drawButtons();

        sleep(1);
        //calc squares
        calculateCells();

        // draw squares
        drawSquares();

        // Draw your grid or game here using SDL functions
        drawGrid(renderer);

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
