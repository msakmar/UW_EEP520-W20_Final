#ifndef __MAZE_COORDINATOR_AGENT__H
#define __MAZE_COORDINATOR_AGENT__H 

#include "enviro.h"
#include <vector>
#include <math.h>

using namespace enviro;


// TODO: NEED TO REMOVE PLACER AGENTS WHEN SWITCHING THE MAZE

class MazeCoordinatorController : public Process, public AgentInterface {

    public:
    MazeCoordinatorController() : 
        Process(),
        AgentInterface(),
        initial(0),
        maze_number(0),
        counter(0),
        default_robot("maze_robot"),
        current_maze_agents_ids()
        {}

    void init() {
        Agent& maze_text_agent = add_agent("MazeText", 0, 0, 0, {{"fill","white"},{"stroke","white"}});

        std::cout << "Current placer size: " << current_placer_agent_ids.size() << "\n";

        if( initial == 0 ) {
            // std::cout << "Adding Maze 1\n";
            current_maze_agents_ids = build_maze_1();
            current_robot_agent_ids = add_robot(default_robot, 60, 60, -90*(M_PI/180));
            maze_text_agent.decorate(R"(<g>
                <style> 
                .heavy { font: bold 30px sans-serif; fill: green;}
                </style>
                <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 1</text></g>)"
            );            
            initial = 1;
            maze_number = 3;
        }   

        watch("AddPlacer", [&](Event e) {
            std::cout << "Adding a placer\n";
            int current_cell_x = e.value()["cell_x"];
            int current_cell_y = e.value()["cell_y"];
            int current_cell_theta = e.value()["cell_theta"];

            Agent& temp = add_agent("placer", current_cell_x, current_cell_y, current_cell_theta, { {"fill", "orange"}, {"fill-opacity","25%"} } );

            std::cout << "Adding a new placer: " << temp.get_id() << "\n";
            current_placer_agent_ids.push_back( temp.get_id() );

            std::cout << "Current placer agent id size: " << current_placer_agent_ids.size()  << "\n";

        });

        /*
        *  Remove existing maze agents, stogreen in a current_maze_agents_ids vector
        *  Clear the current_maze_agents_ids vector
        *  Build new maze using a method
        *  Save new maze agents in the current_maze_agents vector
        */
        watch("SwitchMaze", [&](Event e) {
            maze_text_agent.decorate("");

            for( auto it = current_maze_agents_ids.begin(); it < current_maze_agents_ids.end(); it++ ) {
                remove_agent( (*it) );
            }

            for( auto it = current_robot_agent_ids.begin(); it < current_robot_agent_ids.end(); it++ ) {
                remove_agent( (*it) );
            }

            //Clear the maze agent ids vector
            current_maze_agents_ids.clear();
            current_robot_agent_ids.clear();

            //Determine which maze to create next
            switch ( maze_number ) {
            case 1:
                std::cout << "Adding Maze 2\n";
                current_maze_agents_ids = build_maze_2();
                current_robot_agent_ids = add_robot(default_robot, 0, 60, 0);
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 2</text></g>)"
                );
                maze_number = 2;
                break;
            case 2:
                std::cout << "Adding Maze 3\n";
                current_maze_agents_ids = build_maze_3();
                current_robot_agent_ids = add_robot(default_robot, 0, 0, 180*(M_PI/180));
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 3</text></g>)"
                );
                maze_number = 3;
                break;
            case 3:
                std::cout << "Adding Maze 4\n";
                current_maze_agents_ids = build_maze_4();
                current_robot_agent_ids = add_robot(default_robot, 0, 120, 180*(M_PI/180));
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-170" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 4</text></g>)"
                );
                maze_number = 4;
                break;
            case 4:
                std::cout << "Resetting...\n";

                for( auto it = current_placer_agent_ids.begin(); it < current_placer_agent_ids.end(); it++ ) {
                    remove_agent( (*it) );
                }
                
                current_placer_agent_ids.clear();
                std::cout << "Adding Maze 1\n";

                current_maze_agents_ids = build_maze_1();
                current_robot_agent_ids = add_robot(default_robot, 60, 60, -90*(M_PI/180));
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 1</text></g>)"
                );
                maze_number = 1;
                break;
            default:
                // leave empty
                std::cout << "Default maze_numer switch statement.\n";
                std::cout << "Adding Maze 1\n";
                current_maze_agents_ids = build_maze_1();
                current_robot_agent_ids = add_robot(default_robot, 60, 60, -90*(M_PI/180));
            }
        });
     
    }

    void start() {}

    void update() {
        if ( counter++ > 100 ) {
            emit( Event( "SwitchMaze" ) );
            counter = 0;
            initial = 0;
        }
    }
    void stop() {}

    vector<int> add_robot(string robot_type, double loc_x, double loc_y, double loc_theta) {
        vector<int> agent_ids;
        Agent& temp = add_agent(robot_type, loc_x, loc_y, loc_theta, {{"fill","blue"},{"stroke","blue"}});

        agent_ids.push_back(temp.get_id());
        return agent_ids;
    }

    vector<int> build_maze_original_messed_up() {
        vector<int> maze_agent_ids;
        Agent& maze_1_agent = add_agent("Maze_1", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        maze_agent_ids.push_back(maze_1_agent.get_id());
        return maze_agent_ids;

    }

    vector<int> build_maze_attempt_with_shape() {
        vector<int> maze_agent_ids;
        Agent& maze_2_agent = add_agent("Maze_2", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        maze_agent_ids.push_back(maze_2_agent.get_id());
        return maze_agent_ids;
    }

    vector<int> build_maze_1() {
        vector<int> agent_ids;
            
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", 30, 30, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect6by186two = add_agent("Rect_6_186", 0, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by186two.get_id());

        Agent& Rect6by126one = add_agent("Rect_6_126", 30, 90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by126one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, 30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66two.get_id());
        
        return agent_ids;
    }

    vector<int> build_maze_2() {
        vector<int> agent_ids;
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", -30, 30, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", 30, -60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect6by186one = add_agent("Rect_6_186", 0, 90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by186one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 0, 30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 60, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66three.get_id());
        
        return agent_ids;
    }

    vector<int> build_maze_3() {
        vector<int> agent_ids;
        Agent& Rect186by6one = add_agent("Rect_6_186", -90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect6by186one = add_agent("Rect_6_186", 0, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by186one.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -60, 90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", 60, 90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 0, 30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66three.get_id());

        Agent& Rect6by66four = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66four.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", -30, 60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect66by6two = add_agent("Rect_6_66", 30, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect66by6two.get_id());
        return agent_ids;
    }

    vector<int> build_maze_4() {
        vector<int> agent_ids;
        Agent& Rect306by6one = add_agent("Rect_6_306", -150, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect306by6one.get_id());

        Agent& Rect306by6two = add_agent("Rect_6_306", 150, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect306by6two.get_id());

        Agent& Rect186by6one = add_agent("Rect_6_186", -30, -60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6one.get_id());

        Agent& Rect186by6two = add_agent("Rect_6_186", 90, 0, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect186by6two.get_id());

        Agent& Rect126by6one = add_agent("Rect_6_126", 30, 90, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect126by6one.get_id());

        Agent& Rect66by6one = add_agent("Rect_6_66", -90, 60, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect66by6one.get_id());

        Agent& Rect66by6two = add_agent("Rect_6_66", 30, -120, 90*(M_PI/180), {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect66by6two.get_id());

        Agent& Rect6by306one = add_agent("Rect_6_306", 0, 150, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by306one.get_id());

        Agent& Rect6by126one = add_agent("Rect_6_126", -30, 90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by126one.get_id());

        Agent& Rect6by126two = add_agent("Rect_6_126", -90, -150, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by126two.get_id());

        Agent& Rect6by126three = add_agent("Rect_6_126", 90, -150, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by126three.get_id());

        Agent& Rect6by66one = add_agent("Rect_6_66", -120, -30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66one.get_id());

        Agent& Rect6by66two = add_agent("Rect_6_66", -60, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66two.get_id());

        Agent& Rect6by66three = add_agent("Rect_6_66", 0, -30, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66three.get_id());

        Agent& Rect6by66four = add_agent("Rect_6_66", 60, -90, 0, {{"fill","black"},{"stroke","black"}});
        agent_ids.push_back(Rect6by66four.get_id());

        return agent_ids;
    }

    int initial;
    int maze_number;
    int counter;
    string default_robot;
    vector<int> current_maze_agents_ids;
    vector<int> current_robot_agent_ids;
    vector<int> current_placer_agent_ids;
};

class MazeCoordinator : public Agent {
    public:
    MazeCoordinator(json spec, World& world) : Agent(spec, world) {
        add_process(c);
    }
    private:
    MazeCoordinatorController c;
};

DECLARE_INTERFACE(MazeCoordinator)

#endif