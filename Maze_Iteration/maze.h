#define GRID_HEIGHT (31)
#define GRID_WIDTH (31)

enum directions
{
    RIGHT,
    DOWN,
    LEFT,
    UP
};

void drawMaze(int maze[GRID_HEIGHT][GRID_WIDTH]);

void resetMaze(int maze[GRID_HEIGHT][GRID_WIDTH]);

void resetmazePath(int mazePath[GRID_HEIGHT * GRID_WIDTH][2]);

int createEntrance(int maze[GRID_HEIGHT][GRID_WIDTH]);

int createExit(int maze[GRID_HEIGHT][GRID_WIDTH]);

void initialiseEE(int maze[GRID_HEIGHT][GRID_WIDTH], int mazeStart[2], int mazeEnd[2]);

void setMaze(int maze[GRID_WIDTH][GRID_WIDTH], int currentX, int currentY, int direction, int num);

int checkPathValid(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int xmulti, int ymulti, int xmulti2, int ymulti2, int num);

void generateMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation);

void generateTwoMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation);

void generateLoopMaze(int maze[GRID_HEIGHT][GRID_WIDTH], int currentX, int currentY, int prevOrientation);