//
// Created by tzuk on 6/6/22.
//
#include <opencv2/opencv.hpp>
#include <nlohmann/json.hpp>
#include <unistd.h>
#include "aruco.h"
#include "arocuLeaderInfo.h"
#include "drone.h"

void runAruco(aruco &detector, drone &drone){
    while(true){
        std::cout << "forward: " << detector.forward << " right left: " << detector.rightLeft << " updown: " << detector.upDown
                  << " angle: " << detector.leftOverAngle.first << " clockwise: " << detector.leftOverAngle.second << " id: " << detector.idr << " read Aruco: " << detector.ifArucoExist << std::endl;
        drone.currentInfo = detctor;
    }
}
void tzokArucoReaderRun(drone &drone){
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
    runAruco(detector , drone);
}
int main(){
    drone ourDrone;

    ourDrone.tzokArucoReaderRun();
    ourDrone.currentInfo.printAruco();
    // tzokArucoReaderRun();
    return 0;
}
