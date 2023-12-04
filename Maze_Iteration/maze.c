#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>
#include <string.h>
#include "maze.h"

#define GRID_HEIGHT (31)
#define GRID_WIDTH (31)
#define SIDE_LENGTH (20)

enum directions
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

void drawMaze(int maze[GRID_HEIGHT][GRID_WIDTH])
{
    background();
    setColour(black);

    for (int y = 0; y < GRID_HEIGHT; y++)
    {
        for (int x = 0; x < GRID_WIDTH; x++)
        {
            if (maze[y][x])
            {
                setColour(white);
            }
            else
            {
                setColour(black);
            }
            fillRect(x * SIDE_LENGTH, y * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
        }
    }
}

void resetMaze(int maze[GRID_HEIGHT][GRID_WIDTH])
{
    memset(maze, 0, sizeof(maze[0][0]) * GRID_HEIGHT * GRID_WIDTH);
}

void resetmazePath(int mazePath[GRID_HEIGHT * GRID_WIDTH][2])
{
    memset(mazePath, -1, sizeof(mazePath[0][0]) * GRID_HEIGHT * GRID_WIDTH * 2);
}

int createEntrance(int maze[GRID_HEIGHT][GRID_WIDTH])
{
    for (int i = 1; i < GRID_HEIGHT - 1; i++)
    {
        if (maze[i][1]) // To check if there is a valid previous passage to enter from.
        {
            maze[i][0] = 1;
            return i;
        }
    }
    return -1;
}

int createExit(int maze[GRID_HEIGHT][GRID_WIDTH])
{
    for (int i = GRID_HEIGHT - 1; i > 1; i--)
    {
        if (maze[i][GRID_WIDTH - 2])
        {
            maze[i][GRID_WIDTH - 1] = 1;
            return i;
        }
    }
    return -1;
}

void initialiseEE(int maze[GRID_HEIGHT][GRID_WIDTH], int mazeStart[2], int mazeEnd[2])
{
    mazeStart[0] = 0;
    mazeStart[1] = createEntrance(maze);

    mazeEnd[0] = GRID_WIDTH - 1;
    mazeEnd[1] = createExit(maze);
}

void setMaze(int maze[GRID_WIDTH][GRID_WIDTH], int currentX, int currentY, int direction, int num)
{
    setColour(green);
    sleep(15);
    for (int i = 1; i <= num; i++)
    {
        switch (direction)
        {
        case (RIGHT):
            maze[currentY][currentX + i] = 1;
            fillRect((currentX + i) * SIDE_LENGTH, currentY * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
            break;

        case (DOWN):
            maze[currentY + i][currentX] = 1;
            fillRect(currentX * SIDE_LENGTH, (currentY + i) * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
            break;

        case (LEFT):
            maze[currentY][currentX - i] = 1;
            fillRect((currentX - i) * SIDE_LENGTH, currentY * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
            break;

        case (UP):
            maze[currentY - i][currentX] = 1;
            fillRect(currentX * SIDE_LENGTH, (currentY - i) * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
            break;
        }
    }
}

int checkPathValid(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int xmulti, int ymulti, int xmulti2, int ymulti2, int num)
{
    int counter = 0;
    for (int i = 1; i <= num; i++)
    {
        if (!maze[currentY + (i + 1) * ymulti][currentX + (i + 1) * xmulti])
        {
            counter++;
        }
        if (!maze[currentY + (i * ymulti) + (1 * ymulti2)][currentX + (i * xmulti) + (1 * xmulti2)] &&
            !maze[currentY + (i * ymulti) - (1 * ymulti2)][currentX + (i * xmulti) - (1 * xmulti2)])
        {
            counter++;
        }
    }

    if (counter == 2 * num)
    {
        return 1;
    }

    return 0;
}

// Recursive algorithm to generate maze by exploring path possibilities.
void generateMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation)
{
    int direction;

    int firstDirection = rand() % 4;

    for (int i = 0; i < 4; i++)
    {
        // So all 4 sides are attempted
        direction = (firstDirection + i) % 4;

        if (direction != prevOrientation)
        {
            switch (direction)
            {

            case (RIGHT):
                // Checks for validity of new path. (Two sides are walls + not connecting through to another path)
                if ((currentX + 1 < GRID_WIDTH - 1) &&
                    checkPathValid(maze, currentX, currentY, 1, 0, 0, 1, 1))
                {
                    setMaze(maze, currentX, currentY, direction, 1);
                    generateMaze(maze, currentX + 1, currentY, LEFT);
                }
                break;

            case (DOWN):
                if ((currentY + 1 < GRID_HEIGHT - 1) &&
                    checkPathValid(maze, currentX, currentY, 0, 1, 1, 0, 1))
                {
                    setMaze(maze, currentX, currentY, direction, 1);
                    generateMaze(maze, currentX, currentY + 1, UP);
                }
                break;

            case (LEFT):
                if ((currentX - 1 > 0) &&
                    checkPathValid(maze, currentX, currentY, -1, 0, 0, 1, 1))
                {
                    setMaze(maze, currentX, currentY, direction, 1);
                    generateMaze(maze, currentX - 1, currentY, RIGHT);
                }
                break;

            case (UP):
                if ((currentY - 1 > 0) &&
                    checkPathValid(maze, currentX, currentY, 0, -1, 1, 0, 1))
                {
                    setMaze(maze, currentX, currentY, direction, 1);
                    generateMaze(maze, currentX, currentY - 1, DOWN);
                }
                break;
            }
        }
    }
}

// Makes more "perfect" mazes
void generateTwoMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation)
{
    int direction;

    int firstDirection = rand() % 4;

    for (int i = 0; i < 4; i++)
    {
        direction = (firstDirection + i) % 4;

        if (direction != prevOrientation)
        {
            switch (direction)
            {

            case (RIGHT):
                // Checks for validity of new path. (Two sides are walls + not connecting through to another path)
                if (
                    (currentX + 2 < GRID_WIDTH - 1) &&
                    checkPathValid(maze, currentX, currentY, 1, 0, 0, 1, 2))
                {
                    setMaze(maze, currentX, currentY, direction, 2);
                    generateTwoMaze(maze, currentX + 2, currentY, LEFT);
                }
                break;

            case (DOWN):
                if ((currentY + 2 < GRID_HEIGHT - 1) &&
                    checkPathValid(maze, currentX, currentY, 0, 1, 1, 0, 2))
                {
                    setMaze(maze, currentX, currentY, direction, 2);
                    generateTwoMaze(maze, currentX, currentY + 2, UP);
                }
                break;

            case (LEFT):
                if ((currentX - 2 > 0) &&
                    checkPathValid(maze, currentX, currentY, -1, 0, 0, 1, 2))
                {
                    setMaze(maze, currentX, currentY, direction, 2);
                    generateTwoMaze(maze, currentX - 2, currentY, RIGHT);
                }
                break;

            case (UP):
                if (
                    (currentY - 2 > 0) &&
                    checkPathValid(maze, currentX, currentY, 0, -1, 1, 0, 2))
                {
                    setMaze(maze, currentX, currentY, direction, 2);
                    generateTwoMaze(maze, currentX, currentY - 2, DOWN);
                }
                break;
            }
        }
    }
}

void generateLoopMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation)
{
    int direction;

    int firstDirection = rand() % 4;

    for (int i = 0; i < 3; i++)
    {
        // Tries all sides
        direction = (firstDirection + i) % 4;

        if (direction != prevOrientation)
        {
            switch (direction)
            {

            case (RIGHT):
                // Checks for validity of new path. (Two sides are walls)
                if (
                    (currentX + 1 < GRID_WIDTH - 1) &&
                    ((!maze[currentY + 1][currentX + 1]) &&
                     (!maze[currentY - 1][currentX + 1])))
                {
                    maze[currentY][currentX + 1] = 1;
                    generateLoopMaze(maze, currentX + 1, currentY, LEFT);
                }
                break;

            case (DOWN):
                if (
                    (currentY + 1 < GRID_HEIGHT - 1) &&
                    ((!maze[currentY + 1][currentX + 1]) &&
                     (!maze[currentY + 1][currentX - 1])))
                {
                    maze[currentY + 1][currentX] = 1;
                    generateLoopMaze(maze, currentX, currentY + 1, UP);
                }
                break;

            case (LEFT):
                if (
                    (currentX - 1 > 0) &&
                    ((!maze[currentY + 1][currentX - 1]) &&
                     (!maze[currentY - 1][currentX - 1])))
                {
                    maze[currentY][currentX - 1] = 1;
                    generateLoopMaze(maze, currentX - 1, currentY, RIGHT);
                }
                break;

            case (UP):
                if (
                    (currentY - 1 > 0) &&
                    ((!maze[currentY - 1][currentX + 1]) &&
                     (!maze[currentY - 1][currentX - 1])))
                {
                    maze[currentY - 1][currentX] = 1;
                    generateLoopMaze(maze, currentX, currentY - 1, DOWN);
                }
                break;
            }
        }
    }
}