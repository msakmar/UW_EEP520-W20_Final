#ifndef __MAZE_ROBOT_AGENT__H
#define __MAZE_ROBOT_AGENT__H 

#include "enviro.h"

#include <tuple>        // std::tuple
#include <set>
#include <vector>

using namespace enviro;

int round(int numToRound, int multiple ) {
    if( numToRound >= 0 ) {
        return ( ( numToRound % multiple ) > multiple/2 ) ? ( numToRound + multiple - numToRound%multiple ) : ( numToRound - numToRound%multiple );
    } else {
        numToRound = (-1)*numToRound;
        return ( ( numToRound % multiple ) > multiple/2 ) ? (-1)*( numToRound + multiple - numToRound%multiple ) : (-1)*( numToRound - numToRound%multiple );
    }
}

class maze_robotController : public Process, public AgentInterface {

    public:
    maze_robotController() : Process(), AgentInterface(), f(0), tau(0), firing(false), positions_been() {}

    void init() {      
        //Record the initial cell location of the robot in the maze  
        int initial_cell_x = round(x(), 60);
        int initial_cell_y = round(y(), 60);
        initial_cell = std::make_tuple(initial_cell_x, initial_cell_y);
        previous_cell = initial_cell;
        current_cell = initial_cell;

        //Record collitions with the walls of the maze
        notice_collisions_with("Rect_6_66", [&](Event &e) {
            std::cout << "Ran into a Rect_6_66 wall!\n";
        });  

        notice_collisions_with("Rect_6_126", [&](Event &e) {
            std::cout << "Ran into a Rect_6_126 wall!\n";
        }); 

        notice_collisions_with("Rect_6_186", [&](Event &e) {
            std::cout << "Ran into a Rect_6_186 wall!\n";
        }); 

        notice_collisions_with("Rect_6_306", [&](Event &e) {
            std::cout << "Ran into a Rect_6_306 wall!\n";
        }); 

        //Determine user input via the keyboard
        watch("keydown", [&](Event &e) {
            auto k = e.value()["key"].get<std::string>();
            if ( k == "w" ) {
                f = magnitude;              
            } else if ( k == "s" ) {
                f = -magnitude;  
            } else if ( k == "a" ) {
                tau = -magnitude;
            } else if ( k == "d" ) {
                tau = magnitude;
            } else if ( k == "z" ) {
                std::cout << "Pressed Z to print positions_been\n";
                for (auto it = positions_been.begin(); it != positions_been.end(); it++ ) {
                    std::cout << "Tuple in positions_been: " << std::get<0>(*it) << ", " << std::get<1>(*it) << "\n";
                }
            }
        });        
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

    void start() {}

    void update() {
        //Update the robot's applied force based off of the user input captured in init()
        apply_force(f,tau);

        //Record the 'instantaneous' new cell that the robot is in
        int new_cell_x = round(x(), 60);
        int new_cell_y = round(y(), 60);
        new_cell = std::make_tuple(new_cell_x, new_cell_y);

        //Determine if the instantaneous' new cell is equal to the last update() current cell 
        //If they are different, the robot has changed from one cell to another cell in the maze

        if ( new_cell != current_cell ) {

            //If the current cell cannot be found in the 
            if ( positions_been.find(current_cell) == positions_been.end() ) {
                std::cout << "New area discovered!\n";
                emit ( Event( "AddPlacer", {
                    { "cell_x", std::get<0>(current_cell) },
                    { "cell_y", std::get<1>(current_cell) },
                    { "cell_theta", 0 },
                    { "style", BEEN_STYLE}
                } ) );
                positions_been.insert(current_cell);
            } else {
                std::cout << "Visiting an old area!\n";
            }
            previous_cell = current_cell;
            current_cell = new_cell;
        }        

        // label("Current Position: " + std::to_string((int) x()) 
        // + ", " 
        // + std::to_string((int)y()) 
        // + "\n New Cell: "
        // + std::to_string(new_cell_x) 
        // + ", " 
        // + std::to_string(new_cell_y)
        // + "\n Current Cell: "
        // + std::to_string(std::get<0>(current_cell)) 
        // + ", " 
        // + std::to_string(std::get<1>(current_cell))
        // + "\n Previous Cell: "
        // + std::to_string(std::get<0>(previous_cell)) 
        // + ", " 
        // + std::to_string(std::get<1>(previous_cell)),20,20);
    }
    void stop() {
        std::cout << "Current placer agent id vector size: " << current_placer_agent_ids.size() << "\n";

        for( auto it = current_placer_agent_ids.begin(); it != current_placer_agent_ids.end(); it++ ) {
            if( agent_exists( (*it) ) ) {
                std::cout << "Removing agent id:" << (*it) << "\n";
                Agent _temp = find_agent( (*it) );
                remove_agent( _temp.get_id() );
                std::cout << "Current placer agent id vector size: " << current_placer_agent_ids.size() << "\n";
            }
        }

        current_placer_agent_ids.clear();
    }

    double f, tau;
    double const magnitude = 200;
    bool firing;
    const json BULLET_STYLE = { 
                   {"fill", "green"}, 
                   {"stroke", "#888"}, 
                   {"strokeWidth", "5px"},
                   {"strokeOpacity", "0.25"}
               };
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