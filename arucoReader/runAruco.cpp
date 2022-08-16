//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"
#include <thread>

void runAruco(aruco &detector, drone *myDrone){
        while(detector.ifArucoExist != 1){
        	std::cout << "waiting for aruco" << std::endl;
        }
	if(detector.rightLeft < 0)
        	myDrone->setRightOrLeft(0); // left drone
        else
        	myDrone->setRightOrLeft(1); // right drone
    	
    while(true){
      /* std::cout 
       << "forward: " << detector.forward 
        << " right left: " << detector.rightLeft 
        << " updown: " << detector.upDown
        << " angle: " << detector.leftOverAngle.first 
        << " clockwise: " << detector.leftOverAngle.second 
        << " id: " << detector.idr 
        << " read Aruco: " << detector.ifArucoExist
        << " right or left: " << myDrone->getRightOrLeft()
        << std::endl;*/
        
        myDrone->addInfo(detector);
        myDrone->move_drone();
    	//myDrone.runDrone(detector);
    }
}
void tzukArucoReaderRun(drone *myDrone){
    std::ifstream programData("../config.json");
    nlohmann::json data;
    programData >> data;
    programData.close();
    std::string yamlCalibrationPath = data["yamlCalibrationPath"];
    bool isCameraString = data["isCameraString"];
    float currentMarkerSize = data["currentMarkerSize"];
    if (isCameraString){
        std::string cameraString = data["cameraString"];
        aruco detector(yamlCalibrationPath,cameraString,currentMarkerSize);
    	runAruco(detector, myDrone);
    }else{
        int cameraPort = data["cameraPort"];
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
    	runAruco(detector, myDrone);
    }
}
/*void run(drone *myDrone){
	myDrone->runDrone();
}*/
int main(){
	system("v4l2 -d /dev/video0 focus_auto=0");
    	drone ourDrone;
    	tzukArucoReaderRun(&ourDrone);
    	/*std::thread th1(tzukArucoReaderRun, &ourDrone);
    	std::thread th2(run, &ourDrone);
    
    	th1.join();
    	th2.join();*/
    	
    return 0;
}
