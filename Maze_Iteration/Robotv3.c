#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>
#include <string.h>
#include <math.h>

#define GRID_HEIGHT (17)
#define GRID_WIDTH (17)

#define SIDE_LENGTH (40)

#define CANVAS_HEIGHT (SIDE_LENGTH * GRID_HEIGHT)
#define CANVAS_WIDTH (SIDE_LENGTH * GRID_WIDTH)

#define BOT_DIAMETER (SIDE_LENGTH * 0.75)

#define BOT_OFFSET ((SIDE_LENGTH - BOT_DIAMETER) / 2)

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
            setColour(black);
            drawRect(x * SIDE_LENGTH, y * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
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
        if (maze[i][GRID_WIDTH - 2]) // To check if there is a valid previous passage to enter from.
        {
            maze[i][GRID_WIDTH - 1] = 1;
            return i;
        }
    }
    return -1;
}

void initialiseEE(int maze[GRID_HEIGHT][GRID_WIDTH], int mazeStart[2], int mazeEnd[2])
{
    // Creating a valid entrance;
    mazeStart[0] = 0;
    mazeStart[1] = createEntrance(maze);

    // Creating a valid exit on the rightmost side.
    mazeEnd[0] = GRID_WIDTH - 1;
    mazeEnd[1] = createExit(maze);
}

// Recursive algorithm to generate maze by exploring path possibilities.
void generateMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation)
{
    int direction;

    // This is so it starts at a random direction first before cycling through the rest.
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
                if (
                    ((currentX + 1 < GRID_WIDTH - 1) &&
                     (!maze[currentY][currentX + 2])) &&
                    ((!maze[currentY + 1][currentX + 1]) &&
                     (!maze[currentY - 1][currentX + 1])))
                {
                    maze[currentY][currentX + 1] = 1;
                    generateMaze(maze, currentX + 1, currentY, LEFT);
                }
                break;

            case (DOWN):
                if (
                    ((currentY + 1 < GRID_HEIGHT - 1) &&
                     (!maze[currentY + 2][currentX])) &&
                    ((!maze[currentY + 1][currentX + 1]) &&
                     (!maze[currentY + 1][currentX - 1])))
                {
                    maze[currentY + 1][currentX] = 1;
                    generateMaze(maze, currentX, currentY + 1, UP);
                }
                break;

            case (LEFT):
                if (
                    ((currentX - 1 > 0) &&
                     (!maze[currentY][currentX - 2])) &&
                    ((!maze[currentY + 1][currentX - 1]) &&
                     (!maze[currentY - 1][currentX - 1])))
                {
                    maze[currentY][currentX - 1] = 1;
                    generateMaze(maze, currentX - 1, currentY, RIGHT);
                }
                break;

            case (UP):
                if (
                    ((currentY - 1 > 0) &&
                     (!maze[currentY - 2][currentX])) &&
                    ((!maze[currentY - 1][currentX + 1]) &&
                     (!maze[currentY - 1][currentX - 1])))
                {
                    maze[currentY - 1][currentX] = 1;
                    generateMaze(maze, currentX, currentY - 1, DOWN);
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

void setX(int coordX[3], int x, int i, int direction)
{
    switch (direction)
    {

    case (DOWN):
    case (UP):
    {
        coordX[0] = x * SIDE_LENGTH + BOT_OFFSET + i;
        coordX[1] = (x + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordX[2] = x * SIDE_LENGTH + SIDE_LENGTH / 2 + i;
        break;
    }

    case (RIGHT):
        coordX[0] = x * SIDE_LENGTH + BOT_OFFSET + i;
        coordX[1] = x * SIDE_LENGTH + BOT_OFFSET + i;
        coordX[2] = (x + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        break;

    case (LEFT):
        coordX[0] = (x + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordX[1] = (x + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordX[2] = x * SIDE_LENGTH + BOT_OFFSET + i;
        break;
    }
}

void setY(int coordY[3], int y, int i, int direction)
{
    switch (direction)
    {
    case (RIGHT):
    case (LEFT):
    {
        coordY[0] = y * SIDE_LENGTH + BOT_OFFSET + i;
        coordY[1] = (y + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordY[2] = y * SIDE_LENGTH + SIDE_LENGTH / 2 + i;
        break;
    }

    case (DOWN):
        coordY[0] = y * SIDE_LENGTH + BOT_OFFSET + i;
        coordY[1] = y * SIDE_LENGTH + BOT_OFFSET + i;
        coordY[2] = (y + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        break;

    case (UP):
        coordY[0] = (y + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordY[1] = (y + 1) * SIDE_LENGTH - BOT_OFFSET + i;
        coordY[2] = y * SIDE_LENGTH + BOT_OFFSET + i;
        break;
    }
}

void drawRobot(int *x, int *y, int direction)
{
    int coordX[3];
    int coordY[3];

    foreground();
    setColour(blue);

    // Creates illusion of movement by redrawing the robot SIDE_LENGTH times.
    for (int i = 0; i < SIDE_LENGTH; i++)
    {
        sleep(5);
        clear();

        switch (direction)
        {
        case (RIGHT):
            setX(coordX, (*x - 1), i, direction);
            setY(coordY, *y, 0, direction);
            fillPolygon(3, coordX, coordY);
            break;

        case (DOWN):
            setX(coordX, *x, 0, direction);
            setY(coordY, (*y - 1), i, direction);
            fillPolygon(3, coordX, coordY);
            break;

        case (LEFT):
            setX(coordX, (*x + 1), -i, direction);
            setY(coordY, *y, 0, direction);
            fillPolygon(3, coordX, coordY);
            break;

        case (UP):
            setX(coordX, *x, 0, direction);
            setY(coordY, (*y + 1), -i, direction);
            fillPolygon(3, coordX, coordY);
            break;
        }
    }
}

// As robot shouldn't directly have access to map layout.
int canMoveForward(int maze[GRID_HEIGHT][GRID_WIDTH], int *robotX, int *robotY, int direction)
{
    switch (direction)
    {
    case (RIGHT):
        return (maze[*robotY][*robotX + 1]);
        break;

    case (DOWN):
        return (maze[*robotY + 1][*robotX]);
        break;

    case (LEFT):
        return (maze[*robotY][*robotX - 1]);
        break;

    case (UP):
        return (maze[*robotY - 1][*robotX]);
        break;
    }
    return 0;
}

int atMarker(int mazeEnd[2], int *robotX, int *robotY)
{
    return ((*robotX == mazeEnd[0]) && (*robotY == mazeEnd[1]));
}

void forward(int *direction, int *robotX, int *robotY)
{
    switch (*direction)
    {
    case (RIGHT):
        *robotX += 1;
        break;

    case (DOWN):
        *robotY += 1;
        break;

    case (LEFT):
        *robotX -= 1;
        break;

    case (UP):
        *robotY -= 1;
        break;
    }
}

void left(int *direction)
{
    *direction = (*direction + 4 - 1) % 4;
}

void right(int *direction)
{
    *direction = (*direction + 1) % 4;
}

void mazePathAdd(int mazePath[GRID_WIDTH * GRID_HEIGHT][2], int *counter, int *robotX, int *robotY)
{
    mazePath[*counter][0] = *robotX;
    mazePath[*counter][1] = *robotY;
    *counter += 1;
}

void solveMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int mazeEnd[2], int mazeStart[2], int *direction, int *robotX, int *robotY, int mazePath[GRID_WIDTH * GRID_HEIGHT][2])
{
    int success = 1;
    int counter = 0;

    // Algorithm keeps the wall on the right

    while (!atMarker(mazeEnd, &*robotX, &*robotY))
    {

        // Success variable is so a move is only animated if there is an actual movement, not when there is a turn.
        if (success)
        {
            drawRobot(&*robotX, &*robotY, *direction);

            // Record that the robot has been there.
            mazePathAdd(mazePath, &counter, &*robotX, &*robotY);
        }

        success = 0;

        switch (*direction)
        {
        case (RIGHT):
            // Facing right
            if (!canMoveForward(maze, &*robotX, &*robotY, DOWN) &&
                canMoveForward(maze, &*robotX, &*robotY, RIGHT))
            {
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (canMoveForward(maze, &*robotX, &*robotY, DOWN))
            {
                right(&*direction);
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (!canMoveForward(maze, &*robotX, &*robotY, RIGHT))
            {
                left(&*direction);
            }

            break;

        case (DOWN):
            if (!canMoveForward(maze, &*robotX, &*robotY, LEFT) &&
                canMoveForward(maze, &*robotX, &*robotY, DOWN))
            {
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (canMoveForward(maze, &*robotX, &*robotY, LEFT))
            {
                right(&*direction);
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (!canMoveForward(maze, &*robotX, &*robotY, DOWN))
            {
                left(&*direction);
            }

            break;

        case (LEFT):
            if (!canMoveForward(maze, &*robotX, &*robotY, UP) &&
                canMoveForward(maze, &*robotX, &*robotY, LEFT))
            {
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (canMoveForward(maze, &*robotX, &*robotY, UP))
            {
                right(&*direction);
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (!canMoveForward(maze, &*robotX, &*robotY, LEFT))
            {
                left(&*direction);
            }

            break;

        case (UP):
            if (!canMoveForward(maze, &*robotX, &*robotY, RIGHT) &&
                canMoveForward(maze, &*robotX, &*robotY, UP))
            {
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (canMoveForward(maze, &*robotX, &*robotY, RIGHT))
            {
                right(&*direction);
                forward(&*direction, &*robotX, &*robotY);
                success = 1;
            }
            else if (!canMoveForward(maze, &*robotX, &*robotY, UP))
            {
                left(&*direction);
            }

            break;
        }
    }

    drawRobot(&*robotX, &*robotY, *direction);
    mazePathAdd(mazePath, &counter, &*robotX, &*robotY);
}

void deleteRepeat(int mazePath[GRID_WIDTH * GRID_HEIGHT][2])
{
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        if ((mazePath[i][0] != -1) && (mazePath[i][1] != -1))
        {
            for (int j = GRID_WIDTH * GRID_HEIGHT - 1; j > i; j--)
            {
                if ((mazePath[i][0] == mazePath[j][0]) && (mazePath[i][1] == mazePath[j][1]))
                {
                    for (int m = i + 1; m < j; m++)
                    {
                        mazePath[m][0] = -1;
                        mazePath[m][1] = -1;
                    }
                }
            }
        }
    }
}

void printSolved(int mazePath[GRID_WIDTH * GRID_HEIGHT][2])
{
    setColour(red);
    for (int i = 0; i < GRID_WIDTH * GRID_HEIGHT; i++)
    {
        if (!(mazePath[i][0] == -1))
        {
            sleep(100);
            fillRect(mazePath[i][0] * SIDE_LENGTH, mazePath[i][1] * SIDE_LENGTH, SIDE_LENGTH, SIDE_LENGTH);
        }
    }
}

int main(int argc, char **argv)
{
    // User chooses whether the maze can have loops
    int loop = 0;
    if (argc == 2)
    {
        loop = atoi(argv[1]);
    }

    // Initialisation of a new seed for the random function.
    srand((unsigned int)time(NULL));

    setWindowSize(CANVAS_WIDTH, CANVAS_HEIGHT);

    int maze[GRID_HEIGHT][GRID_WIDTH];
    int mazePath[GRID_WIDTH * GRID_HEIGHT][2];

    int currentX = 8;
    int currentY = 8;

    int mazeStart[2];
    int mazeEnd[2];

    int robotX;
    int robotY;

    int direction = 0;

    resetMaze(maze);
    resetmazePath(mazePath);

    maze[currentY][currentX] = 1;

    switch (loop)
    {
    case (0):
        generateMaze(maze, currentX, currentY, -1);
        break;

    case (1):
        generateLoopMaze(maze, currentX, currentY, -1);
        break;
    }

    initialiseEE(maze, mazeStart, mazeEnd);

    robotX = mazeStart[0];
    robotY = mazeStart[1];

    drawMaze(maze);

    solveMaze(maze, mazeEnd, mazeStart, &direction, &robotX, &robotY, mazePath);

    deleteRepeat(mazePath);
    printSolved(mazePath);

    return 0;
}