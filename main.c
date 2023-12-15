#include <SDL2/SDL.h>
#include <stdio.h>
#include "main.h"
//#include <unistd.h>

enum GameState {
    STATE_NULL,
    STATE_INIT,
    STATE_PAUSED,
    STATE_START,
};

struct Button
{
    int x;
    int y;
    int width;
    int height;
    int isPressed;
    SDL_Rect rect;
};

struct Cell
{
    int x;
    int y;
    int current;
    int next;
    SDL_Rect rect;
};

struct Cell cellArray[_H][_V]; //[H][V]

struct Button startButton;
struct Button clearButton;

enum GameState nowState = STATE_NULL;
enum GameState nextState = STATE_PAUSED;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

void initCells()
{
    int cellSize = CELL_SIZE_CONST - 1; // Set the size of each grid cell to a fixed pixel size

    for (int i = 0; i <= _H; i++) {
        for (int j = 0; j <= _V; j++) {
            cellArray[i][j].x = cellSize * i;
            cellArray[i][j].y = cellSize * j;
            cellArray[i][j].current = FALSE;

            // Populate the SDL_Rect for each cell
            cellArray[i][j].rect.x = cellSize * i;
            cellArray[i][j].rect.y = cellSize * j;
            cellArray[i][j].rect.w = cellSize;
            cellArray[i][j].rect.h = cellSize;
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

int getNeighbours(int i, int j)
{
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
    button->rect.x = x;
    button->rect.y = y;
    button->rect.w = width;
    button->rect.h = height;
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
//    if()
    SDL_SetRenderDrawColor(renderer, GREY);
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

int pointInsideRect(int x, int y, SDL_Rect rect)
{
    return (x >= rect.x && x < rect.x + rect.w && y >= rect.y && y < rect.y + rect.h);
}

void getClickedCellIndices(int clickX, int clickY, int *row, int *col) {
    *row = -1;
    *col = -1;

    for (int i = 0; i < _H; i++) {
        for (int j = 0; j < _V; j++) {
            if (clickX >= cellArray[i][j].rect.x &&
                clickX < cellArray[i][j].rect.x + cellArray[i][j].rect.w &&
                clickY >= cellArray[i][j].rect.y &&
                clickY < cellArray[i][j].rect.y + cellArray[i][j].rect.h) {
                // Set the row and column indices if the click coordinates are within the cell's SDL_Rect
                *row = i;
                *col = j;
                return;
            }
        }
    }
}


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

            cellArray[i][j].current = cellArray[i][j].next;
        }
    }

    if(nextState == STATE_START)
    {
        sleep(1);
    }
}

void clearCells()
{
    for (int i = 0; i < (SCREEN_WIDTH / (CELL_SIZE_CONST - 1)); i++)
    {
        for (int j = 0; j < (GRID_HEIGHT / (CELL_SIZE_CONST - 1)); j++)
        {
            cellArray[i][j].next = FALSE;
            cellArray[i][j].current = FALSE;
        }
    }
}


/*
state machine:
switch(nextstate)
{
    case STATE1:
        if(nextstate != nowstate)
        {//first entry
            nowstate = state1
        }
        else
        {//loop entry
            next_state = STATE2;
        }

        if(nextstate!= nowstate)
        {

        }
        break;
    case STATE2:

        break;
    case STATE3:

        break;
    default:
        return 1;

}
*/


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
    while(running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                {
                    running = SDL_FALSE;
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                {
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        int mouseX = event.button.x;
                        int mouseY = event.button.y;

                        if (pointInsideRect(mouseX, mouseY, startButton.rect))
                        {
                            printf("Start clicked!\n");
                            ///////////////////
                            startButton.isPressed = TRUE;
                            nextState = STATE_START;
                        }
                        else if (pointInsideRect(mouseX, mouseY, clearButton.rect))
                        {
                            printf("Clear Button clicked!\n");
                            ///////////////////
                            clearButton.isPressed = TRUE;
                            clearCells();
                            nextState = STATE_PAUSED;
                        }
                        else
                        {
                            int clickedRow, clickedCol;
                            getClickedCellIndices(mouseX, mouseY, &clickedRow, &clickedCol);
                            if(nextState == STATE_PAUSED)
                            {
                                if (clickedRow != -1 && clickedCol != -1)
                                {
                                    printf("Cell clicked at (%d, %d)\n", clickedRow, clickedCol);
                                    cellArray[clickedRow][clickedCol].next = !cellArray[clickedRow][clickedCol].current;

                                }
                            }
                            printf("Grid clicked at (%d, %d)!\n", mouseX, mouseY);
                        }
                    }

                }break; //SDL_MOUSEBUTTONDOWN break
                default:
                {
                    //printf("no user input\n");
                }
                break;
            }//end of event switch
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 64, 64, 64, 255);
        SDL_RenderClear(renderer);

        // Draw buttons
        drawButtons();

        if(nextState == STATE_START)
        {
            calculateCells();
        }

        // Draw squares
        drawSquares();

        // Draw grid
        drawGrid();

        // Update the screen
        SDL_RenderPresent(renderer);

    }//while running end

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
