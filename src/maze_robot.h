#ifndef __MAZE_ROBOT_AGENT__H
#define __MAZE_ROBOT_AGENT__H 

#include "enviro.h"

#include <tuple>        // std::tuple
#include <set>
#include <vector>

using namespace enviro;


//! Round a number to the nearest multiple
    /*!
      \param numToRound The input integer to round
      \param multiple The integer to round towards
      \return The rounded number as an int
    */
int round(int numToRound, int multiple ) {
    if( numToRound >= 0 ) {
        return ( ( numToRound % multiple ) > multiple/2 ) ? ( numToRound + multiple - numToRound%multiple ) : ( numToRound - numToRound%multiple );
    } else {
        numToRound = (-1)*numToRound;
        return ( ( numToRound % multiple ) > multiple/2 ) ? (-1)*( numToRound + multiple - numToRound%multiple ) : (-1)*( numToRound - numToRound%multiple );
    }
}

//! A class for controlling a robot agent

//! Watches for input from the user via the keyboard and induces forces onto a robot agent
//! Watches for collisions with rectangular agents part of the maze
//! Keeps track of the robot's cell locations
class maze_robotController : public Process, public AgentInterface {

    public:
    maze_robotController() : Process(), AgentInterface(), f(0), tau(0), positions_been(), collision_counter(0), old_collision_counter(0) {}

    //! Initialization method. This method should be overridden by derived
    //! classes. It will usually be called once, after all processes and
    //! communication objects have been added to the manager, but before
    //! the Manager starts running.
    void init() {      
        //Record the initial cell location of the robot in the maze  
        int initial_cell_x = round(x(), 60);
        int initial_cell_y = round(y(), 60);
        initial_cell = std::make_tuple(initial_cell_x, initial_cell_y);
        previous_cell = initial_cell;

        //Record collitions with the walls of the maze
        notice_collisions_with("Rect_6_66", [&](Event &e) {
            // std::cout << "Ran into a Rect_6_66 wall!\n";
            collision_counter++;
        });  

        //Record collitions with the walls of the maze
        notice_collisions_with("Rect_6_126", [&](Event &e) {
            // std::cout << "Ran into a Rect_6_126 wall!\n";
            collision_counter++;
        }); 

        //Record collitions with the walls of the maze
        notice_collisions_with("Rect_6_186", [&](Event &e) {
            // std::cout << "Ran into a Rect_6_186 wall!\n";
            collision_counter++;
        }); 

        //Record collitions with the walls of the maze
        notice_collisions_with("Rect_6_306", [&](Event &e) {
            // std::cout << "Ran into a Rect_6_306 wall!\n";
            collision_counter++;
        }); 

        //Determine user input via the keyboard keydown events
        watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == "w" ) {
                f = 0.85*magnitude;              
            } else if ( k == "s" ) {
                f = -0.85*magnitude;  
            } else if ( k == "a" ) {
                tau = -0.75*magnitude;
            } else if ( k == "d" ) {
                tau = 0.75*magnitude;
            } else if ( k == "z" ) {
                std::cout << "Pressed Z to print positions_been\n";
                for (auto it = positions_been.begin(); it != positions_been.end(); it++ ) {
                    std::cout << "Tuple in positions_been: " << std::get<0>(*it) << ", " << std::get<1>(*it) << "\n";
                }
            }
        });

        //Determine user input via the keyboard keyup events
        watch("keyup", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();

            if ( k == "w" || k == "s" ) {
                f = 0;               
            } else if ( k == "a" ) {
                tau = 0;
            } else if ( k == "d" ) {
                tau = 0;
            } 
        });
    }

    //! Start method. This method should be  overridden by derived
    //! classes. It will be called just before the manager starts running.
    //! It may be called multiple times, if the manager is started and stopped.
    void start() {}

    //! Update method. It will be called repeatedly by the manager at a frequency
    //! determined by the period used when the process is scheduled with the
    //! Manager (see Elma::Manager::schedule).
    void update() {
        //Update the robot's applied force based off of the user input captured in init()
        apply_force(f,tau);

        //Record the 'instantaneous' new cell that the robot is located in
        int new_cell_x = round(x(), 60);
        int new_cell_y = round(y(), 60);
        new_cell = std::make_tuple(new_cell_x, new_cell_y);

        //Determine if the 'instantaneous' new cell is equal to the previous cell 
        //If they are different, the robot has changed from one cell to another cell in the maze
        if ( new_cell != previous_cell ) {

            //If the previous cell cannot be found in the positions_been vector, the robot is in a new cell
            if ( positions_been.find(previous_cell) == positions_been.end() ) {
                //IN A NEW CELL/AREA

                //Emit an event to place a 'placer' agent in the previous cell's location
                emit ( Event( "AddPlacer", {
                    { "cell_x", std::get<0>(previous_cell) },
                    { "cell_y", std::get<1>(previous_cell) },
                    { "cell_theta", 0 },
                    { "style", BEEN_STYLE}
                } ) );
                positions_been.insert(previous_cell);
            } else {
                //IN AN OLD CELL/AREA
            }
            previous_cell = new_cell;
        }

        if ( old_collision_counter != collision_counter ) {
            //Emit an event to update the collision counter
            emit ( Event( "UpdateCounter", {
                { "collision_counter", collision_counter},
            } ) );

            old_collision_counter = collision_counter;
        }

        //Debug code to print out the current position, new cell, and current cell information
        // label("Current Position: " + std::to_string((int) x()) 
        // + ", " 
        // + std::to_string((int)y()) 
        // + "\n New Cell: "
        // + std::to_string(new_cell_x) 
        // + ", " 
        // + std::to_string(new_cell_y)
        // + "\n Previous Cell: "
        // + std::to_string(std::get<0>(previous_cell)) 
        // + ", " 
        // + std::to_string(std::get<1>(previous_cell)),20,20);
    }

    //! Stop method. This method should be  overridden by derived
    //! classes. It will be called just after the manager stops running.
    //! It may be called multiple times, if the manager is started and stopped.       
    void stop() {}

    double f, tau;
    double const magnitude = 200;
    const json BEEN_STYLE = { 
                   {"fill", "orange"}, 
                   {"fill-opacity","25%"}
               };

    std::tuple<int,int> initial_cell;
    std::tuple<int,int> previous_cell;
    std::tuple<int,int> current_cell;
    std::tuple<int,int> new_cell;
    std::set<std::tuple<int,int>> positions_been;
    vector<int> current_placer_agent_ids;
    int collision_counter;
    int old_collision_counter;
};

class maze_robot : public Agent {
    public:
    maze_robot(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    maze_robotController c;
};

DECLARE_INTERFACE(maze_robot)

#endif