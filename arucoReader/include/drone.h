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
       	bool clockwise;
       	int angle;
        bool runCamera ;
        // std::vector<arocuLeaderInfo> targetsList;

    public:
        drone();
        // ~drone();
 	void addInfo(aruco &aruco);
        void get_leader_position();
        void move_drone();
        void run();
};
