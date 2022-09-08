# RealTime Lab - Drone Swarm


**University of Haifa - RealTime Lab**

### Authors:

* [Almog Hadad](https://github.com/AlmogHadad)
* [Daniel Ramrazker](https://github.com/danielramraz)
* [Yaad Rebhun](https://github.com/YaadR)

## Project description:
* Aruco detection and extraction of data position.
* Drone movment logic & swarm structure.
* Robust flight structure, with minimal communication.


![Swarm video](Vid/swarmgif.gif)
## Run Me:
IDE: **Raspberry.pi**
### Installations required:
```
install cmake
install tello
```
### Libraries used:
1. opencv :
   * `opencv2`- used to create the aruco reader.
2. ctello:
   * `ctello` - used to control the drone.
3. thread:
   * `thread` - creating multithreaded program.

### How to run:  
* download the project to <"some path">.
* make sure your drone connect to your computer with wifi.
* in command line:
```
cd <some path>/arucoReader/build
make
./runAruco
```

### Basic interupts:
#### Quit program:
You can quit by pressing "ctr + c", the drone will land in his place.

## Algorithem & Math:
#### 1. Acquiring leader
  * The drone detect aruco and set his own position in the swarm according to relative starting position.
  * The drone use the Aruco reader to identify the leader drone.
  
  
#### 2. Detecting leader movement
  * The aruco marker provide position data relative to 3 vectors, which translate into 4 movment axis:
  ![ArucoVectors](/Vid/aruco-axis.png)

#### 3. Bounding box
  * The logic for the drone movment, is triggered by diviation in drone position from certain distances it needs to maintain. Maximun and minimum distance in the x,y,z axis and constrain delta angle ,all relative to the starting point of the axes defined by the aruco marker. baisicly the drone strive to find oasis zone.
#### 4. Drone movement logic:
  * The more the drone deviates from it's bounding box - the more thrust it puts in correction direction.
#### 5. Lost leader:
  * In case of sight disruption, the drone will roatate in place, in search for the arocu marker for reconnection with the leader.


----------------------------------------------------------------------------------------------------------------
