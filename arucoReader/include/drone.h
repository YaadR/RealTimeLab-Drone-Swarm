#include "aruco.h"

class drone{
    private:
        int id;
        // aruco currentInfo;
        double upDown ;
        double forward ;
        double rightLeft ;
        int idr ;
        bool ifArucoExist ;
        //std::pair<int, bool> leftOverAngel ;
        bool runCamera ;
        // std::vector<arocuLeaderInfo> targetsList;

    public:
        drone(aruco currentInfo);
        // ~drone();
   
        void get_leader_position();
        void move_drone();
        void run();
};
