The maze generation program utilises a 2D array and my implementation of a recursive random direction maze path generator to build a maze. This process can be seen as the maze is generated in green.

![](https://github.com/20tsange1/publicWork/blob/main/mazeGenerated.png)

The robot then attempts to solve the maze using the right hand to the wall rule.

When complete, the program will then print out the solution (shortest) path for the maze in red.

![](https://github.com/20tsange1/publicWork/blob/main/mazeGeneratedSolved.png)

Pointers are used within the main functions where possible to alter variables declared in main without intialising new local variables.

The maze can be generated with loops and without loops.

Changing the maze sizings will need to be done within the source code itself at the top where the constant values are defined.

Values that can be changed:
- GRID_HEIGHT
- GRID_WIDTH
- SIDE_LENGTH
- ANIMATION_SPEED

Requirements:
- GRID_HEIGHT * SIDE_LENGTH <= 680 (May change depending on screen dimensions, this is the specification for an M2 Macbook Air)
- GRID_WIDTH * SIDE_LENGTH <= 1460
- GRID_HEIGHT % 4 = 3
- GRID_WIDTH % 4 = 3
- ANIMATION_SPEED >= 0 (The greater the speed, the slower the animation)

Program should be run using the following two commands:

gcc -o mazeRobotFinal mazeRobotFinal.c graphics.c    
./mazeRobotFinal LOOP | java -jar drawapp-2.0.jar

Where LOOP must be replaced by either (DEFAULT = 0):
- 0 = No loops
- 1 = Longer Length 2 Paths ("Perfect" Maze)
- 2 = Loops

References:
https://www.tutorialspoint.com/c_standard_library/c_function_srand.htm (For use of srand)

