Project Goal
===
- ***Project 'Robot Maze'***: Utilizes enviro.  Illustrates decorations, dynamically loading different mazes, and labels
- ***Goal***: The objective of the project is to create a robot agent that can solve four non-looping mazes.  Each maze will be loaded
dynamically, starting with a smaller one and getting more complicated with each subsequent maze.  The code to this project must be well documented and should be stable.  
- ***Additional features (goals)***:
    - Decorating the maze and the robot agent dynamically through the use of buttons
    - Buttons change the behavior of the robot
    - User can take control of the robot at any time via the keyboard
    - The maze changes tile color as the robot passes over a tile, indicating the robot 'has been there before' or made a choice to take a left, right, or straight turn

Key Challenges
===
- ***Dynamically loading new mazes***
    - The original method used for loading a maze via the config.json's "statics" object array could not be used
    - In addition, creating a 'static' agent with a complex shape caused Chipmunk2D to crash or create an unintended shape
        - TODO: INSERT IMAGE OF BROKEN MAZE HERE
    - Instead, universal 'static' agent rectangle 'pieces' had to be used to build the maze
        - This required more planning to ensure proper alignment between the rectangles and that the robot could fit in the maze to traverse it correctly
        - Therefore, 'hallways' of 54 units wide were chosen to fit a robot of size 20 units by 20 units.  This allowed enough room for the robot to move around the 'hallways'
            - TODO: INSERT IMAGE OF NEW MAZE HERE
        - The rectangle 'pieces' were chosen to be dynamically loaded and removed by an invisible agent called `maze_coordinator`
            - The `maze_coordinator` is responsible for determining what agents should be loaded/unloaded
                - These agents include the rectangular 'pieces', the robot, and the placer pieces indicated where the robot has been
- ***Dynamically loading the robot***
    - The original maze robot was implemented using sensors, but an unforeseen bug caused segmentation faults loading an invisible agent that implemented sensors.  Originally a second invisible agent was created for managing multiple robot agents, called `robot_coordinator`.
        - To determine what was causing the segmentation fault, the robot was first removed; this stopped the segementation faults
        - Next, the contents of the `robot_coordinator` were placed into the `maze_controller` and the robot agent was added back in.  However, the segmentation faults still occurred.
        - A third attempted work-around was to load the agent with sensors as 'dynamic', but off-screen, and to simply teleport the agent around as-needed, but that also caused a segmentation fault 
        - Further attempts involved simpler agents.  The multiagent example from the `enviro` github, which includes an invisible agent, was used to determine that *any* project that involved an agent with sensors and an invisible agent caused segmentation faults
- ***Tracking where the robot has been in the maze, highlighting the previous 'cells'***
    - In order to track where the robot has been in the loaded area on the webpage, the area was broken up into 'cells'
        - Each cell is 60 units by 60 units
        - In order to properly break the area into 60x60 'cells' a rounding function had to be created
            - This function had to be able to round to the nearest multiple of 60 with both negative and positive values
    - The multiagent example from the `enviro` github was used to manually move the robot around the area
        - The `label` feature from enviro was utilized to print out the robot's current unit-by-unit `x` and `y` position, in addition to the appropriate 'cell' location
    - A set, `positions_been` was utilized to keep track of all of the previously-visited `x` and `y` cell locations
        - A set was used to ensure there were no duplicates
    - To keep track of whether the robot was moving into a new cell, two variables were used; `previous_cell` and `new_cell`.  The `new_cell` variable was the latest `x` and `y` cell location of the robot, while the `previous_cell` variable kept the last 'cell' location in update()
        - If `new_cell` and `previous_cell` were different, this meant that the robot traveled into a new 'cell'
        - If `previous_cell` was not in the `positions_been` set, then the `positions_cell`'s `x` and `y` coordinates were new and a placer should be placed in that location
    - A `placer` agent was created to visually mark on the page where the robot has been
- ***Removing `placer` agents after completing all mazes***
    - Another issue run into during this project was removing `placer` agents by the `maze_controller`.
    - To debug, the adding and removing of the `placer` agents was put into the `maze_robot` class
        - Unfortunately, segmentation faults still occured
    - The same methedology was used for adding and removing the rectangular 'pieces', so it is believed that somehow agents added via the init() and watch() functions cannot be safely removed by enviro in another function
        - Agents were verified that they existed and could be found before trying to remove them via `remove_agent()`, however, as soon as this function was called with an agent_id that belonged to a `placer` agent, a segementation fault occurred.

Acknowledgements
===
- Many thanks to Professor Klavins for his example code in the enviro github and for the time and effort from the TAs throughout the quarter of the EEP520-W20 class
- The mozilla developer page for SVG elements was leveraged for understanding how text and styling could be added to the project:
    - https://developer.mozilla.org/en-US/docs/Web/SVG
- A forum was utilized for understanding how to efficiently round: 
    - https://gist.github.com/aslakhellesoy/1134482 
- The C++ reference website helped greatly in utilizing some standard template library functions:
    - https://en.cppreference.com/w/cpp
ENVIRO: The multi-agent, multi-user, multi-everything simulator
===

Setting Up Docker
===

To start the docker image environment, do
```bash
docker run -p80:80 -p8765:8765 -v $PWD:/source -it klavins/enviro:v6.1 bash
```
This will start a bash promt from which you can build new projects and run the enviro server.

The Project Layout
===
The project is broken up into the following directories and files
```
Makefile
config.json
    Contains the agent definitions of the project
defs/
    Contains the definitions of the project
lib/
    Contains the libraries of the project
src/
    Contains the source files for the project
```

Running The Project
===
Start the enviro server in bash, make clean the project, make the project, and then start the enviro server
```bash
esm start
make clean
make
enviro
```

Open a web browser, preferrably chromium-based, and navigate to `http://localhost`, if using Docker Desktop.  A small 3x3 maze should be displayed with a robot.  Control the robot with 'w-a-s-d' control.  

Entering the key combination, `Ctrl-C`, in the `bash` terminal will stop the enviro server.  Simply type in 'enviro' into the `bash` terminal again to restart the server.

To Do List
===
1. ~~Load maze dynamically from rectangle shapes and 'invisible' maze coordinator~~
2. ~~Load multiple mazes dynamically, cycling through each one~~
3. ~~Load Robot dynamically in beginning of maze~~
4. ~~Tune Robot's manual movement to smoothly traverse maze~~
5. Decorate Robot
6. Add buttons to change decoration of Robot
7. Change color of tiles moved over by the robot to indicate the robot 'has been here'
8. Add automated control to take over the robot
10. Add buttons to change behavior of robot

Project Rubric
===

Goal: Be creative. Make a fun, engaging, and or informative simulation or game. Learn some C++ and multi-agent systems in the process. 

Projects are due Wednesday March 18 at 11:59pm.

| Points | Description | Completed |
|-----|-----|-----|
| 5   | Uses enviro, or extends enviro in some way.  | DONE! |
| 5   | Code is well documented. Each method and class should have inline comments say what it does. See [here](https://github.com/klavinslab/elma) for an example.  | DONE! |
| 5   | Project is shared with as a new Github Repository.  | DONE! |
| 5   | Repo has a licence file (e.g. the [mit licence](https://opensource.org/licenses/MIT)) | DONE! |
| 5   | Repo has a README.md file | DONE! |
| 10  | README has an overview of what the goal of the project is | DONE! |
| 10  | README has a description of key challenges and how they were addressed |DONE! |
| 10  | README describes how to install and run the code (assuming the user has docker installed) |
| 10  | README describes how to run and/or use the project | DONE! |
| 5   | README acknowedges all sources of information or code used | DONE! |
| 20  | Project compiles | DONE! |
| 20  | Project runs without crashing. Occasional crashes will not result in partial deductions.  | DONE! |
| 20  | Project shows a substantial behavior | ~SORTA DONE |
| 20  | Behavior is mostly bug free | DONE! |
|----|----|
| Total | 150 |