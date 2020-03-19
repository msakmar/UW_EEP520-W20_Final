#ifndef __MAZE_COORDINATOR_AGENT__H
#define __MAZE_COORDINATOR_AGENT__H 

#include "enviro.h"
#include <vector>
#include <math.h>
#include <tuple>

using namespace enviro;

//! A class for coordinating a robot within multiple mazes

//! Watches for when to place 'placer' agents for highlighting where the robot has been
//! Watches for when to switch the maze
//! Keeps track of the robot's cell locations
class MazeCoordinatorController : public Process, public AgentInterface {

    public:
    MazeCoordinatorController() : 
        Process(),
        AgentInterface(),
        initial(0),
        maze_number(0),
        default_robot("maze_robot"),
        current_maze_agents_ids(),
        current_placer_agent_ids(),
        maze_collision_counter(0),
        counter_text_agent_id(-1),
        current_placer_tuples(),
        make_placers(1)
        {}
        
    //! Initialization method. This method should be overridden by derived
    //! classes. It will usually be called once, after all processes and
    //! communication objects have been added to the manager, but before
    //! the Manager starts running.
    void init() {
        Agent& maze_text_agent = add_agent("MazeText", 0, 0, 0, {{"fill","white"},{"stroke","white"}});
        Agent& maze_collision_counter_agent = add_agent("MazeText", 0, 0, 0, {{"fill","white"},{"stroke","white"}});
        counter_text_agent_id = maze_collision_counter_agent.get_id();

        if( initial == 0 ) {
            // std::cout << "Adding Maze 1\n";
            current_maze_agents_ids = build_maze_1();
            current_robot_agent_id = add_robot(default_robot, 60, 60, -90*(M_PI/180));
            maze_text_agent.decorate(R"(<g>
                <style> 
                .heavy { font: bold 30px sans-serif; fill: green;}
                </style>
                <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 1</text></g>)"
            );            
            initial = 1;
            maze_number = 1;
            make_placers = 1;
        }

        watch("UpdateCounter", [&](Event e) {
            maze_collision_counter = e.value()["collision_counter"];
        });

        watch("AddPlacer", [&](Event e) {
            int current_cell_x = e.value()["cell_x"];
            int current_cell_y = e.value()["cell_y"];
            int current_cell_theta = e.value()["cell_theta"];
            std::tuple<int,int,int,int> temp_placer = std::make_tuple( current_cell_x, current_cell_y, current_cell_theta, 0 );
            current_placer_tuples.push_back( temp_placer );
        });

        //! Remove existing maze agents, stogreen in a current_maze_agents_ids vector
        //! Clear the current_maze_agents_ids vector
        //! Build new maze using a method
        //! Save new maze agents in the current_maze_agents vector
        watch("SwitchMaze", [&](Event e) {
            maze_text_agent.decorate("");

            for( auto it = current_maze_agents_ids.begin(); it < current_maze_agents_ids.end(); it++ ) {
                remove_agent( (*it) );
            }

            remove_agent( current_robot_agent_id );

            //Clear the maze agent ids vector
            current_maze_agents_ids.clear();

            //Determine which maze to create next
            switch ( maze_number ) {
            case 1:
                //std::cout << "Adding Maze 2\n";
                current_maze_agents_ids = build_maze_2();
                current_robot_agent_id = add_robot(default_robot, 0, 60, 0);
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 2</text></g>)"
                );
                maze_number = 2;
                break;
            case 2:
                // std::cout << "Adding Maze 3\n";
                current_maze_agents_ids = build_maze_3();
                current_robot_agent_id = add_robot(default_robot, 0, 0, 180*(M_PI/180));
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-110" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 3</text></g>)"
                );
                maze_number = 3;
                break;
            case 3:
                // std::cout << "Adding Maze 4\n";
                current_maze_agents_ids = build_maze_4();
                current_robot_agent_id = add_robot(default_robot, 0, 120, 180*(M_PI/180));
                maze_text_agent.decorate(R"(<g>
                    <style> 
                    .heavy { font: bold 30px sans-serif; fill: green;}
                    </style>
                    <text x="0" y="-170" dominant-baseline="middle" text-anchor="middle" class="heavy">Maze 4</text></g>)"
                );
                maze_number = 4;
                break;
            case 4:
                //For indicating to remove the placers
                make_placers = 0;

                int x_temp = -120;
                int y_temp = -120;

                for (int i = 0; i < 5; i ++ ) {
                    for (int j = 0; j < 5; j ++ ) {
                        Agent& temp_end_placers = add_agent("placer", x_temp + 60*i, y_temp + 60*j, 0, {{"fill","white"},{"stroke","white"}});
                    }
                }

                Agent& maze_collision_counter_agent = find_agent(counter_text_agent_id);
                maze_collision_counter_agent.decorate("");

                Agent& temp_end_placer = add_agent("placer", 0, 0,0, {{"fill","white"},{"stroke","white"}});
                maze_text_agent.decorate("");

                temp_end_placer.decorate(R"(<g>
                <style>
                    .thanks { font: italic 30px sans-serif; fill: purple; }
                    .ending { font: bold 50px sans-serif; fill: gold; }
                </style>
                <text x="0" y="40" dominant-baseline="middle" text-anchor="middle" class="thanks">Thank you for playing!</text>
                <text x="0" y="0" dominant-baseline="middle" text-anchor="middle" class="ending">THE END</text>
                    </g>)"
                );
                maze_number = -1;
                break;
            }
        });
     
    }

    //! Start method. This method should be  overridden by derived
    //! classes. It will be called just before the manager starts running.
    //! It may be called multiple times, if the manager is started and stopped.
    void start() {}

    //! Update method. This method should be  overridden by derived
    //! classes. It will be called repeatedly by the manager at a frequency
    //! determined by the period used when the process is scheduled with the
    //! Manager (see Elma::Manager::schedule).
    void update() {
        Agent& maze_collision_counter_agent = find_agent(counter_text_agent_id);
        if (maze_number != -1 ) {
            Agent& Robot_Agent_Temp = find_agent(current_robot_agent_id);
            
            string collision_string = R"(<g><style>.warning { font: bold 30px sans-serif; fill: red;}</style><text x="150" y="-110" dominant-baseline="middle" text-anchor="middle" class="warning"> Collision: )" + std::to_string(maze_collision_counter) + R"(</text></g>)";

            if( ( -90 <= Robot_Agent_Temp.x() && Robot_Agent_Temp.x() <= -30 ) && ( 90 <= Robot_Agent_Temp.y() && Robot_Agent_Temp.y() <= 150 ) && ( maze_number == 1 ) ) {
                //Completed maze 1
                maze_collision_counter = 0;
                emit( Event( "SwitchMaze" ) );
            } else if( ( -30 <= Robot_Agent_Temp.x() && Robot_Agent_Temp.x() <= 30 ) && ( -150 <= Robot_Agent_Temp.y() && Robot_Agent_Temp.y() <= -90 ) && ( maze_number == 2 ) ) {
                //Completed maze 2
                maze_collision_counter = 0;
                emit( Event( "SwitchMaze" ) );
            } else if( ( -30 <= Robot_Agent_Temp.x() && Robot_Agent_Temp.x() <= 30 ) && ( 90 <= Robot_Agent_Temp.y() && Robot_Agent_Temp.y() <= 150 ) && ( maze_number == 3 ) ) {
                //Completed maze 3
                maze_collision_counter = 0;
                emit( Event( "SwitchMaze" ) );
            } else if( ( -30 <= Robot_Agent_Temp.x() && Robot_Agent_Temp.x() <= 30 ) && ( -210 <= Robot_Agent_Temp.y() && Robot_Agent_Temp.y() <= -150 ) && ( maze_number == 4 ) ) {
                //Completed maze 4
                maze_collision_counter = 0;
                emit( Event( "SwitchMaze" ) );
            }

            if( maze_number == 4 ) {
                collision_string = R"(<g><style>.warning { font: bold 30px sans-serif; fill: red;}</style><text x="150" y="-170" dominant-baseline="left" text-anchor="left" class="warning"}> Collision: )" + std::to_string(maze_collision_counter) + R"(</text></g>)";
            }

            maze_collision_counter_agent.decorate(collision_string);

            for ( auto it = current_placer_tuples.begin(); it != current_placer_tuples.end(); it++ ) {
                if ( std::get<3>((*it)) == 0 && ( make_placers == 1 ) ) {
                    Agent& temp_agent = add_agent("placer",std::get<0>((*it)),std::get<1>((*it)),std::get<2>((*it)), {{"fill","orange"}, {"fillOpacity", "25%"},{"stroke","none"}});
                    current_placer_agent_ids.push_back( temp_agent.get_id() );
                    std::get<3>((*it)) = 1;
                }
            }
            /* Causes a segmentation fault
            if ( make_placers == 0 ) {
                for ( auto it = current_placer_agent_ids.begin(); it != current_placer_agent_ids.end(); it++ ) {
                    remove_agent( (*it) );
                }
                make_placers = 1;
                current_placer_agent_ids.clear();
            }
            */
        } else if (maze_number == -1 ) {
            // std::cout << "Made it to stop() in maze_coordinator.  Thanks again for playing!\n";
            maze_collision_counter_agent.decorate("");
        }
    }

    //! Stop method. This method should be  overridden by derived
    //! classes. It will be called just after the manager stops running.
    //! It may be called multiple times, if the manager is started and stopped.
    void stop() {}

    //! Add a robot agent to the maze
    /*!
      \param robot_type The type of robot to insert into the maze
      \param loc_x The x coordinate to place the robot into
      \param loc_y The y coordinate to place the robot into
      \param loc_theta The theta angle to place the robot into
      \return An int of the robot agent id
    */
    int add_robot(string robot_type, double loc_x, double loc_y, double loc_theta) {
        Agent& temp = add_agent(robot_type, loc_x, loc_y, loc_theta, {{"fill","blue"},{"stroke","blue"}});
        return temp.get_id();
    }

    //For debug example
    vector<int> build_maze_original_messed_up() {
        vector<int> maze_agent_ids;
        Agent& maze_1_agent = add_agent("Maze_1", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        maze_agent_ids.push_back(maze_1_agent.get_id());
        return maze_agent_ids;

    }

    //For debug example
    vector<int> build_maze_attempt_with_shape() {
        vector<int> maze_agent_ids;
        Agent& maze_2_agent = add_agent("Maze_2", 0, 0, 0, {{"fill","black"},{"stroke","black"}});
        maze_agent_ids.push_back(maze_2_agent.get_id());
        return maze_agent_ids;
    }

    //! Add a rectangular agents to form a maze
    /*!
      \return A vector of agent ids
    */
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

    //! Add a rectangular agents to form a maze
    /*!
      \return A vector of agent ids
    */
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

    //! Add a rectangular agents to form a maze
    /*!
      \return A vector of agent ids
    */
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

    //! Add a rectangular agents to form a maze
    /*!
      \return A vector of agent ids
    */
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
    int maze_collision_counter;

    string default_robot;
    vector<int> current_maze_agents_ids;
    int current_robot_agent_id;
    vector<int> current_placer_agent_ids;
    vector<std::tuple<int,int,int,int>> current_placer_tuples;
    int counter_text_agent_id;
    bool make_placers;
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