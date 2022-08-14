#include "aruco.h"
#include "arocuLeaderInfo.h"

class drone{
    private:
        int id;
        aruco origin;
        arocuLeaderInfo currentInfo;
        
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
    ~drone();
    void calculate_info_to_RC();
 	void add_info(aruco &aruco);
    void get_leader_position();
    void move_drone();
    void run();
};
