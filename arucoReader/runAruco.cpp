//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"
#include <thread>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <iostream>
#include <ctello.h>
#include <string.h>


int AVRAGE = 5;
float SLEEP =1000;

void theEqualizer(int ac[], int* eq, int round){
	ac[0] += eq[0];
	ac[1] += eq[1];
	ac[2] += eq[2];
	ac[3] += eq[3];
	if(round%AVRAGE==0){
		ac[0] = ac[0]/AVRAGE;
		ac[1] = ac[1]/AVRAGE;
		ac[2] = ac[2]/AVRAGE;
		ac[3] = ac[3]/AVRAGE;
	}
	
		
}


void theStringer(int* arr,ctello::Tello& tello){
	std::string str = "rc ";
        str+= std::to_string(arr[0]);
	str+=" ";
        str+= std::to_string(arr[1]);
	str+=" ";
        str+= std::to_string(arr[2]);
	str+=" ";
        str+= std::to_string(arr[3]);

	 
	 std::cout << "passing: " <<str << std::endl;
	 tello.SendCommand(str);
}

void acquiring(aruco &detector, ctello::Tello& tello){
	int count = 100;
	
	if(detector.ifArucoExist == 0){
		
		while(detector.ifArucoExist != 1){

			std::cout << " can't find aRuco" << std::endl;
			tello.SendCommand("rc 0 0 0 40");
			usleep(500);
			count--;

		}
	}
	return;
}
void runAruco(aruco &detector, drone *myDrone, ctello::Tello& tello){
	
        char sign = '+';
        int staller  = 0;
        int* equalizer;
	int accumulate[4] = {0};
        
        while(detector.ifArucoExist == 0){
        	std::cout << "waiting for aruco" << std::endl;        	
	}
	
	tello.SendCommand("rc 0 0 20 0");
	sleep(2);
	if(detector.rightLeft > 0.2)
        	myDrone->setRightOrLeft(0); // Right drone
        else
        	myDrone->setRightOrLeft(1); // Left drone
        	
       	int dontsee = 0;
    while(true){

        myDrone->addInfo(detector);
	
        staller++;

	equalizer = myDrone->move_drone();
	if(equalizer[0] == -1 && equalizer[1] == -1 && equalizer[2] == -1 && equalizer[3] == -1){
		dontsee++;
		if(dontsee >= 1000){
			acquiring(detector, tello);
			dontsee = 0;
		}
		equalizer[0] == 0;
		equalizer[1] == 0;
		equalizer[2] == 0;
		equalizer[3] == 0;
		tello.SendCommand("rc 0 0 0 0");
	}
	else{
		dontsee = 0;
		theEqualizer(accumulate, equalizer, staller);	
	        if(staller%AVRAGE==0){

		        theStringer(accumulate,tello);	        
		        accumulate[0] = 0;
		        accumulate[1] = 0;
		        accumulate[2] = 0;
		        accumulate[3] = 0;
        		

        	}
		if(staller%10==0){
			std::cout << std::endl << std::endl
	       		<< "forward: " << detector.forward 
			<< " right left: " << detector.rightLeft 
			<< " updown: " << detector.upDown
			<< " right or left: " << myDrone->getRightOrLeft()
			<< std::endl << std::endl;}
	}
       usleep(SLEEP);

	}
}


int main(){
    std::ifstream programData("../config.json");

    drone myDrone;
    
    nlohmann::json data;
    programData >> data;
    programData.close();
    std::string droneName = data["DroneName"];
    std::string commandString = "nmcli c up " + droneName;
    const char *command = commandString.c_str();
    system(command);
    ctello::Tello tello;
    tello.SendCommandWithResponse("streamon");
    std::string videoPath = data["cameraString"];
    std::string yamlCalibrationPath = data["yamlCalibrationPath"];
    bool isCameraString = data["isCameraString"];
    float currentMarkerSize = data["currentMarkerSize"];
    
    if (isCameraString){
        std::string cameraString = data["cameraString"];
        aruco detector(yamlCalibrationPath,cameraString,currentMarkerSize);
    	tello.SendCommandWithResponse("takeoff");
    	runAruco(detector,&myDrone,tello);
    }else{
        int cameraPort = data["cameraPort"];       
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
        tello.SendCommandWithResponse("takeoff");
    	runAruco(detector, &myDrone ,tello);

    }
    

    tello.SendCommandWithResponse("land");
    cv::destroyAllWindows();
	
 	
    	
    return 0;
}




        
        
        
