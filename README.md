Project Goal
===
- ***Project 'Maze Runner'***: Utilizes enviro.  Illustrates sensors, decorations, dynamically loading different mazes, and labels
- ***Goal***: The objective of the project is to create a robot agent that can solve four non-looping mazes.  Each maze will be loaded
dynamically, starting with a smaller one and getting more complicated with each subsequent maze.  The code to this project must be well documented and should be stable.  
- ***Additional features (goals)***:
    - Decorating the maze and the robot agent dynamically through the use of buttons
    - Buttons change the behavior of the robot
    - User can take control of the robot at any time via the keyboard
    - The maze changes tile color as the robot passes over a tile, indicating the robot 'has been there before' or made a choice to take a left, right, or straight turn

Key Challenges
===
- Dynamically loading new mazes and relocating the robot
    - The original method used for loading a maze via the config.json's "statics" object array could not be used
    - In addition, creating a 'static' agent with a complex shape caused Chipmunk2D to crash or create an unintended shape
    - Instead, universal 'static' agent rectangle 'pieces' had to be used to build the maze
        - This required more planning to ensure proper alignment between the rectangles and that the robot could fit in the maze to traverse it correctly
        - Therefore, 'hallways' of 54 units wide were chosen to fit a robot of size 20 units by 20 units.  This allowed enough room for the robot to move around the 'hallways'
- Dynamically loading the robot
- Moving the robot straight along a path in the maze

To Do List
===
1. ~~Load maze dynamically from rectangle shapes and 'invisible' maze coordinator~~
2. ~~Load multiple mazes dynamically, cycling through each one~~
3. Load Robot dynamically in beginning of maze
4. Tune Robot movement to smoothly traverse maze
5. Decorate Robot
6. Add buttons to change decoration of Robot
7. Change color of tiles moved over by the robot to indicate the robot 'has been here'
8. Add user control to take over the robot
10. Add buttons to change behavior of robot


Project Rubric
===

Goal: Be creative. Make a fun, engaging, and or informative simulation or game. Learn some C++ and multi-agent systems in the process. 

Projects are due Wednesday March 18 at 11:59pm.

| Points | Description | Completed |
|-----|-----|-----|
| 5   | Uses enviro, or extends enviro in some way.  | DONE! |
| 5   | Code is well documented. Each method and class should have inline comments say what it does. See [here](https://github.com/klavinslab/elma) for an example.  |
| 5   | Project is shared with as a new Github Repository.  | DONE! |
| 5   | Repo has a licence file (e.g. the [mit licence](https://opensource.org/licenses/MIT)) | DONE! |
| 5   | Repo has a README.md file | DONE! |
| 10  | README has an overview of what the goal of the project is | DONE! |
| 10  | README has a description of key challenges and how they were addressed |
| 10  | README describes how to install and run the code (assuming the user has docker installed) |
| 10  | README describes how to run and/or use the project |
| 5   | README acknowedges all sources of information or code used |
| 20  | Project compiles |
| 20  | Project runs without crashing. Occasional crashes will not result in partial deductions.  |
| 20  | Project shows a substantial behavior |
| 20  | Behavior is mostly bug free |
|----|----|
| Total | 150 |