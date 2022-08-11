//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"


void runAruco(aruco &detector, drone &myDrone){
    while(true){
       /*std::cout 
        << "forward: " << detector.forward 
        << " right left: " << detector.rightLeft 
        << " updown: " << detector.upDown
        << " angle: " << detector.leftOverAngle.first 
        << " clockwise: " << detector.leftOverAngle.second 
        << " id: " << detector.idr 
        << " read Aruco: " << detector.ifArucoExist
        << std::endl;*/
        
        myDrone.addInfo(detector);
        myDrone.move_drone();
    }
}
void tzokArucoReaderRun(drone &myDrone){
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
int main(){
    drone ourDrone;
    
    tzokArucoReaderRun(ourDrone);
    return 0;
}
