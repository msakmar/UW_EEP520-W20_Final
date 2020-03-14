Project Rubric
===

Goal: Be creative. Make a fun, engaging, and or informative simulation or game. Learn some C++ and multi-agent systems in the process. 

Projects are due Wednesday March 18 at 11:59pm.

| Points | Description |
|-----|-----|
| 5   | Uses enviro, or extends enviro in some way.  |
| 5   | Code is well documented. Each method and class should have inline comments say what it does. See [here](https://github.com/klavinslab/elma) for an example.  |
| 5   | Project is shared with as a new Github Repository.  |
| 5   | Repo has a licence file (e.g. the [mit licence](https://opensource.org/licenses/MIT) |
| 5   | Repo has a README.md file |
| 10  | README has an overview of what the goal of the project is |
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


Project Goal
===
- ***Maze Runner***: Utilizes enviro.  Illustrates sensors, decorations, dynamically loading different mazes, and labels
- ***Goal***: The objective of the project is to create a robot agent that can solve three non-looping mazes.  Each maze will be loaded
dynamically, starting with a smaller one and getting more complicated with each subsequent maze.  The code to this project must be well documented and should be stable.  
- ***Additional features (goals)***:
    - Decorating the maze and the robot agent dynamically through the use of buttons
    - Buttons change the behavior of the robot
    - User can take control of the robot at any time via the keyboard
    - The maze changes tile color as the robot passes over a tile, indicating the robot 'has been there before' or made a choice to take a left, right, or straight turn

Key Challenges
===
- Moving the robot straight along a path in the maze
- Dynamically loading the robot
- Dynamically loading new mazes and relocating the robot

To Do List
===
1. Load maze dynamically from .svg file
2. Load another maze dynamically from a .svg file
3. Load Robot dynamically in beginning of maze
4. Tune Robot movement to smoothly traverse maze
5. Decorate Robot
6. Add buttons to change decoration of Robot
7. Change color of tiles moved over by the robot to indicate the robot 'has been here'
8. Add user control to take over the robot
10. Add buttons to change behavior of robot