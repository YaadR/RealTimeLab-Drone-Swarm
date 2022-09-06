#include "aruco.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <cstdlib>
#include <string.h>

class drone{
    private:
        enum{left, right} id;

        /*raw info from aruco*/
        float rightLeft ;               //for each vector
        float forward ;                 //for each vector
        float upDown ;                  //for each vector
       	int angle;                      //for each vector
       	int rollAngle;			//for each vector

        bool ifArucoExist ;             //necessary only for raw info 

        int idr ;                   //check if still relevant
       	bool clockwise;             //check if still relevant
        bool runCamera ;            //check if still relevant
        bool RightOrLeft = 1;           //check if still relevant

        aruco* ourAruco;

    public:
        drone();
        // ~drone();
        void set_id(int id);
        // void set_info(std::vector <auto> rawInfo);
 	void addInfo(aruco &aruco);
        //std::vector <auto> get_leader_position();
        //std::string move_drone();
        int* move_drone();
        void runDrone();
        void setRightOrLeft(int i);         //check if still relevant
        bool getRightOrLeft();
        double getRightLeft(aruco &detector);
		/////////////// Logic implamentation from here //////////////////
	//std::vector<double> datAruco(aruco &detector){    };

	    //void move_to(float *v_data);
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





