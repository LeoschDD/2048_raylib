#include <cmath>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <print>

#ifndef NDEBUG
    #define LOG_INFO(...) printf("Info: %s\n", __VA_ARGS__)
    #define LOG_WARNING(...) printf("Warning: %s\n  File: %s\n  Line: %d\n", __VA_ARGS__, __FILE__, __LINE__)
    #define LOG_ERROR(...) printf("Error: %s\n  File: %s\n  Line: %d\n", __VA_ARGS__, __FILE__, __LINE__)
#else
    #define LOG_INFO(...)
    #define LOG_WARNING(...)
    #define LOG_ERROR(...)
#endif

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define GRID_SIZE 4
#define CELL_SIZE (int)(WINDOW_WIDTH / GRID_SIZE)

typedef struct
{
    int value;
    bool merged;
} Cell;

typedef Cell Grid[GRID_SIZE][GRID_SIZE];

enum class MoveDirection
{
    UP = 0,
    DOWN = 1,
    LEFT = 2,
    RIGHT = 3
};

Color getCellColor(int value)
{
    switch (value)
    {
        case 0:    return WHITE;
        case 2:    return Color(238,228,218,255);
        case 4:    return Color(237,224,200,255);
        case 8:    return Color(242,177,121,255);
        case 16:   return Color(245,149, 99,255);
        case 32:   return Color(246,124, 95,255);
        case 64:   return Color(246, 94, 59,255);
        case 128:  return Color(237,207,114,255);
        case 256:  return Color(237,204, 97,255);
        case 512:  return Color(237,200, 80,255);
        case 1024: return Color(237,197, 63,255);
        case 2048: return Color(237,194, 46,255);
        case 4096: return Color( 60, 58, 50,255);
        case 8192: return Color( 60, 58, 50,255);
        case 16384:return Color( 60, 58, 50,255);
        default:   return Color( 60, 58, 50,255);
    }
}


void renderGrid(Grid& grid)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            DrawRectangle(x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE,
                CELL_SIZE, getCellColor(grid[x][y].value));
            const char* value = std::to_string(grid[x][y].value).c_str();
            if (grid[x][y].value) DrawText(value, x * CELL_SIZE, y * CELL_SIZE, CELL_SIZE / 2, BLACK);
        }
    }
}

bool moveGrid(Grid& grid, MoveDirection direction)
{
    bool moved = false;

    int dirX = (direction == MoveDirection::LEFT) ? -1 : (direction == MoveDirection::RIGHT) ? 1 : 0;
    int dirY = (direction == MoveDirection::UP) ? -1 : (direction == MoveDirection::DOWN) ? 1 : 0;

    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            grid[x][y].merged = false;
        }
    }
    for (int j = 0; j < GRID_SIZE * GRID_SIZE; j++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int y = 0; y < GRID_SIZE; y++)
            {
                for (int i = 1; i < GRID_SIZE; i++)
                {
                    if (grid[x][y].value == 0 || x + (i * dirX) < 0 || x + (i * dirX) >= GRID_SIZE ||
                        y + (i * dirY) < 0 || y + (i * dirY) >= GRID_SIZE) break;

                    Cell& nextCell = grid[x + (i * dirX)][y + (i * dirY)];

                    if (nextCell.value == grid[x][y].value && !nextCell.merged && !grid[x][y].merged)
                    {
                        moved = true;
                        nextCell.value = grid[x][y].value * 2;
                        grid[x][y].value = 0;
                        grid[x][y].merged = false;
                        nextCell.merged = true;
                        continue;
                    }
                    else if (nextCell.value) break;

                    moved = true;
                    nextCell.value = grid[x][y].value;
                    nextCell.merged = false;
                    grid[x][y].value = 0;
                    grid[x][y].merged = false;
                }

            }
        }
    }
    return moved;
}

void spawnCell(Grid& grid)
{
    int value = (rand() % 1) + 1;

    int x = rand() % GRID_SIZE - 1;
    int y = rand() % GRID_SIZE - 1;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        for (int j = 0; j < GRID_SIZE; j++)
        {
            if (!grid[x][y].value)
            {
                grid[x][y].value = value * 2;
                return;
            }
            else
            {
                x = (x + i) % GRID_SIZE - 1;
                y = (y + j) % GRID_SIZE - 1;

            }
        }
    }
}

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2048");

    Grid grid{};

    grid[1][1].value = 128;

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_DOWN)) if (moveGrid(grid, MoveDirection::DOWN)) spawnCell(grid);
        if (IsKeyPressed(KEY_UP)) if (moveGrid(grid, MoveDirection::UP)) spawnCell(grid);
        if (IsKeyPressed(KEY_LEFT)) if (moveGrid(grid, MoveDirection::LEFT)) spawnCell(grid);
        if (IsKeyPressed(KEY_RIGHT)) if (moveGrid(grid, MoveDirection::RIGHT)) spawnCell(grid);

        BeginDrawing();
        renderGrid(grid);
        EndDrawing();
    }
    return 0;
}