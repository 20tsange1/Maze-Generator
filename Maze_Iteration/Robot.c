// Use a 2D array for the maze.
// Make the robot consider both sides and record down how many steps.

#include <stdio.h>
#include <stdlib.h>
#include "graphics.h"
#include <time.h>

#define CANVAS_WIDTH 680
#define CANVAS_HEIGHT 680

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3

void drawMaze(int maze[17][17])
{
    background();
    setColour(black);

    for (int y = 0; y < 17; y++)
    {
        for (int x = 0; x < 17; x++)
        {
            if (maze[y][x] == 1)
            {
                setColour(white);
            }
            else
            {
                setColour(black);
            }
            fillRect(x * 40, y * 40, x * 40 + 40, y * 40 + 40);
            setColour(black);
            drawRect(x * 40, y * 40, x * 40 + 40, y * 40 + 40);
        }
    }
}

void resetMaze(int maze[17][17])
{

    for (int y = 0; y < 17; y++)
    {
        for (int x = 0; x < 17; x++)
        {
            maze[y][x] = 0;
        }
    }
}

int createExit(int maze[17][17])
{
    for (int i = 16; i > 1; i--)
    {
        if (maze[i][15] == 1)
        {
            maze[i][16] = 1;
            return i;
        }
    }
    return -1;
}

int mazeTouched(int mazeTouch[17][17])
{
    for (int y = 0; y < 17; y++)
    {
        for (int x = 0; x < 17; x++)
        {
            if (mazeTouch[y][x] == 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

// Recursive algorithm to generate maze by exploring path possibilities.
void generateMaze(int maze[17][17], int currentX, int currentY, int prevOrientation)
{
    int direction;

    // This is so it starts at a random direction first before cycling through the rest.
    int dire = rand() % 4;

    for (int i = 0; i < 3; i++)
    {
        // So all 4 sides are attempted
        direction = (dire + i) % 4;

        // So it doesn't backtrack
        if (direction != prevOrientation)
        {
            switch (direction)
            {

            // left
            case (0):
                // Checks if the area on the side is still within the bounds and if there is enough space to create a new path, then it checks if a valid path can be created which requires both sides to be walls.
                if (((currentX - 1 > 0) && (maze[currentY][currentX - 2] == 0)) && ((maze[currentY + 1][currentX - 1] == 0) && (maze[currentY - 1][currentX - 1] == 0)))
                {
                    maze[currentY][currentX - 1] = 1;
                    generateMaze(maze, currentX - 1, currentY, 1);
                }
                break;

            // right
            case (1):
                if (((currentX + 1 < 16) && (maze[currentY][currentX + 2] == 0)) && ((maze[currentY + 1][currentX + 1] == 0) && (maze[currentY - 1][currentX + 1] == 0)))
                {
                    maze[currentY][currentX + 1] = 1;
                    generateMaze(maze, currentX + 1, currentY, 0);
                }
                break;

            // up
            case (2):
                if (((currentY - 1 > 0) && (maze[currentY - 2][currentX] == 0)) && ((maze[currentY - 1][currentX + 1] == 0) && (maze[currentY - 1][currentX - 1] == 0)))
                {
                    maze[currentY - 1][currentX] = 1;
                    generateMaze(maze, currentX, currentY - 1, 3);
                }
                break;

            // down
            case (3):
                if (((currentY + 1 < 16) && (maze[currentY + 2][currentX] == 0)) && ((maze[currentY + 1][currentX + 1] == 0) && (maze[currentY + 1][currentX - 1] == 0)))
                {
                    maze[currentY + 1][currentX] = 1;
                    generateMaze(maze, currentX, currentY + 1, 2);
                }
                break;
            }
        }
    }
}

void generateLoopMaze(int maze[17][17], int currentX, int currentY, int prevOrientation)
{
    int direction;

    // This is so it starts at a random direction first before cycling through the rest.
    int dire = rand() % 4;

    for (int i = 0; i < 3; i++)
    {
        // Tries all sides
        direction = (dire + i) % 4;

        // So it doesn't backtrack while generating
        if (direction != prevOrientation)
        {
            switch (direction)
            {

            // left
            case (0):
                // Checks if the left hand box is still valid (within maze size) and then checks if opening that box will allow for a valid passage (two walls on the left and the right of it)
                if ((currentX - 1 > 0) && ((maze[currentY + 1][currentX - 1] == 0) && (maze[currentY - 1][currentX - 1] == 0)))
                {
                    maze[currentY][currentX - 1] = 1;
                    generateLoopMaze(maze, currentX - 1, currentY, 1);
                }
                break;

            // right
            case (1):
                if ((currentX + 1 < 16) && ((maze[currentY + 1][currentX + 1] == 0) && (maze[currentY - 1][currentX + 1] == 0)))
                {
                    maze[currentY][currentX + 1] = 1;
                    generateLoopMaze(maze, currentX + 1, currentY, 0);
                }
                break;

            // up
            case (2):
                if ((currentY - 1 > 0) && ((maze[currentY - 1][currentX + 1] == 0) && (maze[currentY - 1][currentX - 1] == 0)))
                {
                    maze[currentY - 1][currentX] = 1;
                    generateLoopMaze(maze, currentX, currentY - 1, 3);
                }
                break;

            // down
            case (3):
                if ((currentY + 1 < 16) && ((maze[currentY + 1][currentX + 1] == 0) && (maze[currentY + 1][currentX - 1] == 0)))
                {
                    maze[currentY + 1][currentX] = 1;
                    generateLoopMaze(maze, currentX, currentY + 1, 2);
                }
                break;
            }
        }
    }
}

// saying that the maze ends there
//}

void drawRobot(int x, int y, int direction)
{
    foreground();
    setColour(blue);

    // Creates illusion of movement by redrawing the robot 40 times.
    for (int i = 0; i < 40; i++)
    {
        sleep(10);
        clear();

        switch (direction)
        {
        case (RIGHT):
            fillArc((x - 1) * 40 + 5 + i, y * 40 + 5, 30, 30, 0, 360);
            break;

        case (DOWN):
            fillArc(x * 40 + 5, (y - 1) * 40 + 5 + i, 30, 30, 0, 360);
            break;

        case (LEFT):
            fillArc((x + 1) * 40 + 5 - i, y * 40 + 5, 30, 30, 0, 360);
            break;

        case (UP):
            fillArc(x * 40 + 5, (y + 1) * 40 + 5 - i, 30, 30, 0, 360);
            break;
        }
    }
}

// As robot shouldn't directly have access to map layout.
int queryMaze(int maze[17][17], int robotX, int robotY, int direction)
{

    switch (direction)
    {
    case (RIGHT):
        return (maze[robotY][robotX + 1]);
        break;

    case (DOWN):
        return (maze[robotY + 1][robotX]);
        break;

    case (LEFT):
        return (maze[robotY][robotX - 1]);
        break;

    case (UP):
        return (maze[robotY - 1][robotX]);
        break;
    }
    return 0;
}

int atMarker(int mazeEnd[2], int robotX, int robotY)
{
    if ((robotX == mazeEnd[0]) && (robotY == mazeEnd[1]))
    {
        return 1;
    }
    else
    {
        return 0;
    }

    return -1;
}


void solveMaze(int maze[17][17], int mazeEnd[2], int mazeStart[2], int* direction, int* robotX, int* robotY)
{

    // make empty list, then add items whenever a new step is made, then use it at end as the solved maze path

    // go forward, left, right, back, anything but the same direction you came in.

    int success = 1;

    // Algorithm keeps the wall on the right

    while (!atMarker(mazeEnd, *robotX, *robotY))
    {

        // Success variable is so it only animates a move if there is an actual movement, not when there is a turn.
        if (success == 1)
        {
            drawRobot(*robotX, *robotY, *direction);
        }

        success = 0;

        switch (*direction)
        {
        // Right
        case (RIGHT):
            // Facing right
            if ((queryMaze(maze, *robotX, *robotY, DOWN) == 0) && (queryMaze(maze, *robotX, *robotY, RIGHT) == 1))
            {
                *robotX += 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, DOWN) == 1)
            {
                *direction = DOWN;
                *robotY += 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, RIGHT) == 0)
            {
                *direction = UP;
            }

            break;

        // Down
        case (DOWN):
            // Facing down
            if ((queryMaze(maze, *robotX, *robotY, LEFT) == 0) && (queryMaze(maze, *robotX, *robotY, DOWN) == 1))
            {
                *robotY += 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, LEFT) == 1)
            {
                *direction = LEFT;
                *robotX -= 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, DOWN) == 0)
            {
                *direction = RIGHT;
            }

            break;

        // Left
        case (LEFT):
            // Facing left
            if ((queryMaze(maze, *robotX, *robotY, UP) == 0) && (queryMaze(maze, *robotX, *robotY, LEFT) == 1))
            {
                *robotX -= 1;
                success = 1;
            }
            else if ((queryMaze(maze, *robotX, *robotY, UP) == 1))
            {
                *direction = UP;
                *robotY -= 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, LEFT) == 0)
            {
                *direction = DOWN;
            }

            break;

        // Up
        case (UP):
            // Facing up
            if ((queryMaze(maze, *robotX, *robotY, RIGHT) == 0) && (queryMaze(maze, *robotX, *robotY, UP) == 1))
            {
                *robotY -= 1;
                success = 1;
            }
            else if ((queryMaze(maze, *robotX, *robotY, RIGHT) == 1))
            {
                *direction = RIGHT;
                *robotX += 1;
                success = 1;
            }
            else if (queryMaze(maze, *robotX, *robotY, UP) == 0)
            {
                *direction = LEFT;
            }

            break;
        }
    }

    drawRobot(*robotX, *robotY, *direction);
}

void endFound()
{
    clear();
    background();
    clear();

    setColour(black);
    fillRect(0, 0, CANVAS_WIDTH, CANVAS_HEIGHT);

    foreground();
    setColour(white);
    drawString("Robot has solved the maze", 250, 340);
}

// Example basic maze ( 0 = black 1 = white)

int main()
{

    srand((unsigned int)time(NULL));

    setWindowSize(CANVAS_WIDTH, CANVAS_HEIGHT);

    // change to calloc? so memory gets allocated and bits get changed to 0. This prevents old memory from being accessed.

    int maze[17][17];
    int mazeTouch[17][17];

    int currentX = 1;
    int currentY = 1;
    int loop = 0;

    int mazeStart[2];
    int mazeEnd[2];

    int robotX;
    int robotY;

    int direction = 0;

    resetMaze(maze);
    resetMaze(mazeTouch);

    // As we have set the maze start at 0,1 (This can be changed if wanted with a similar function to creating the exit)
    mazeStart[0] = 0;
    mazeStart[1] = 1;

    robotX = mazeStart[0];
    robotY = mazeStart[1];

    maze[mazeStart[1]][mazeStart[0]] = 1;
    maze[currentY][currentX] = 1;

    switch (loop)
    {
    case (0):
        generateMaze(maze, currentX, currentY, 0);
        break;

    case (1):
        generateLoopMaze(maze, currentX, currentY, 0);
        break;
    }

    // Creating a valid exit on the rightmost side.
    mazeEnd[0] = 16;
    mazeEnd[1] = createExit(maze);

    drawMaze(maze);

    // use pointers instead of returning
    solveMaze(maze, mazeEnd, mazeStart, &direction, &robotX, &robotY);

    endFound();

    return 0;
}