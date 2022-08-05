#include "aruco.h"

class drone{
    private:
        // int id;
        aruco currentInfo;
        // std::vector<arocuLeaderInfo> targetsList;

    public:
        drone(aruco currentInfo);
        // ~drone();
   
        aruco get_leader_position();
        void move_drone(aruco positionChange);
        void run();
};
