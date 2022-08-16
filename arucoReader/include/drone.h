#include "aruco.h"

/*@Y*/
#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstdlib>
/*
#define MAX_DIST 1.2;
#define MIN_DIST 0.8;
#define MAX_LEFT_RIGHT 0.4;
#define MIN_LEFT_RIGHT 0.2;
#define UP_DOWN_RANGE 0.15;
#define YAW_RANGE 0.1;
#define ROTATE_RANGE 0.1;
/*@Ye*/


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
        bool RightOrLeft;
        // std::vector<arocuLeaderInfo> targetsList;

    public:
        drone();
        // ~drone();
 	void addInfo(aruco &aruco);
        void get_leader_position();
        void move_drone();
        void runDrone();
        void setRightOrLeft(int i);
        bool getRightOrLeft();
        double getRightLeft(aruco &detector);
		/////////////// Logic implamentation from here //////////////////
	//std::vector<double> datAruco(aruco &detector){    };

	 void move_to(float *v_data);
	/*
	v_data[0] = forward
	v_data[1] = rightLeft
	v_data[2] = upDown
	v_data[3] = leftOverAngle.first
	v_data[4] = leftOverAngle.second
	v_data[5] = idr
	v_data[6] = ifArucoExist
	*/

};





