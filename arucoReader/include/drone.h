#include "aruco.h"

class drone{
    private:
        int id;
        aruco currentInfo;
        // std::vector<arocuLeaderInfo> targetsList;

    public:
        drone(/* args */);
        ~drone();
    
    drone::drone(/* args */){}
    
    drone::~drone(){}
    
    data get_leader_position();
    void move_drone(data positionChange);
    void run();
    
};
   