//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "drone.h"
#include "arocuLeaderInfo.h"


void print_aruco_polling_output(aruco &detector, drone &myDrone){       //debug function
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
        
        myDrone.add_info(detector);
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
    }else{
        int cameraPort = data["cameraPort"];
        aruco detector(yamlCalibrationPath,cameraPort,currentMarkerSize);
    }
    print_aruco_polling_output(detector, myDrone);    //printing func
}

int main(){
    drone ourDrone;
    ourDrone.run();
    //tzokArucoReaderRun(ourDrone);
    return 0;
}
