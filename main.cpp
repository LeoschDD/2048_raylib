#include <cmath>
#include <iostream>
#include <ostream>
#include <raylib.h>
#include <print>
#include <vector>

#define WINDOW_WIDTH 400
#define WINDOW_HEIGHT 400

#define GRID_SIZE 4
#define CELL_SIZE (int)(WINDOW_WIDTH / GRID_SIZE)

static bool animating = false;

typedef struct
{
    int value;
    bool merged;
    Vector2 currPos;
    Vector2 toPos;
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
        case 0: return WHITE;
        case 2: return Color(238,228,218,255);
        case 4: return Color(237,224,200,255);
        case 8: return Color(242,177,121,255);
        case 16: return Color(245,149,99,255);
        case 32: return Color(246,124,95,255);
        case 64: return Color(246,94,59,255);
        case 128: return Color(237,207,114,255);
        case 256: return Color(237,204,97,255);
        case 512: return Color(237,200,80,255);
        case 1024: return Color(237,197,63,255);
        case 2048: return Color(237,194,46,255);
        case 4096: return Color(60,58,50,255);
        case 8192: return Color(60,58,50,255);
        case 16384: return Color(60,58,50,255);
        default: return Color(60,58,50,255);
    }
}

void renderGrid(Grid& grid)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y ++)
        {
            Cell cell = grid[x][y];
            DrawRectangle(CELL_SIZE * x, CELL_SIZE * y, CELL_SIZE, CELL_SIZE, getCellColor(cell.value));
            if (cell.value) DrawText((const char*)std::to_string(cell.value).c_str(), CELL_SIZE * x, CELL_SIZE * y, 20, BLACK);
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

    for (int i = 0; i < GRID_SIZE * GRID_SIZE; i++)
    {
        for (int x = 0; x < GRID_SIZE; x++)
        {
            for (int y = 0; y < GRID_SIZE; y++)
            {
                Cell& thisCell = grid[x][y];
                if (thisCell.value == 0) continue;

                for (int j = 1; j <= GRID_SIZE; j++)
                {
                    // out of bound check
                    if (x + (j * dirX) < 0 || x + (j * dirX) >= GRID_SIZE ||
                        y + (j * dirY) < 0 || y + (j * dirY) >= GRID_SIZE) break;

                    Cell& nextCell = grid[x + (j * dirX)][y + (j * dirY)];

                    //merged
                    if (thisCell.value == nextCell.value && !nextCell.merged && !thisCell.merged)
                    {
                        moved = true;
                        thisCell.value = 0;
                        nextCell.value *= 2;
                        nextCell.merged = true;
                    }
                    else if (nextCell.value == 0)
                    {
                        moved = true;
                        nextCell.value = grid[x + ((j - 1) * dirX)][y + ((j - 1) * dirY)].value;
                        grid[x + ((j - 1) * dirX)][y + ((j - 1) * dirY)].value = 0;
                    }
                    else break;
                }
            }
        }
    }
    return moved;
}

void spawnCell(Grid& grid)
{
    std::vector<Vector2> empty;

    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y].value == 0) empty.push_back(Vector2((float)x, (float)y));
        }
    }

    Vector2 spawn = empty[rand() % (empty.size())];
    grid[(int)spawn.x][(int)spawn.y].value = (rand() % 10 == 0) ? 4 : 2;
}

bool canMove(Grid& grid)
{
    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (grid[x][y].value == 0) return true;
        }
    }

    for (int x = 0; x < GRID_SIZE; x++)
    {
        for (int y = 0; y < GRID_SIZE; y++)
        {
            if (x < (GRID_SIZE - 1)) if (grid[x][y].value == grid[x + 1][y].value) return true;
            if (y < (GRID_SIZE - 1)) if (grid[x][y].value == grid[x][y + 1].value) return true;
        }
    }
    return false;
}

int main()
{
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "2048");

    Grid grid{};

    spawnCell(grid);
    spawnCell(grid);

    while (!WindowShouldClose())
    {
        if (!animating)
        {
            if (IsKeyPressed(KEY_DOWN)) if (moveGrid(grid, MoveDirection::DOWN)) spawnCell(grid);
            if (IsKeyPressed(KEY_UP)) if (moveGrid(grid, MoveDirection::UP)) spawnCell(grid);
            if (IsKeyPressed(KEY_LEFT)) if (moveGrid(grid, MoveDirection::LEFT)) spawnCell(grid);
            if (IsKeyPressed(KEY_RIGHT)) if (moveGrid(grid, MoveDirection::RIGHT)) spawnCell(grid);
        }
        BeginDrawing();
        renderGrid(grid);
        if (!canMove(grid)) DrawText("GAME OVER!", 30, 20, 50, BLACK);
        EndDrawing();
    }
    return 0;
}