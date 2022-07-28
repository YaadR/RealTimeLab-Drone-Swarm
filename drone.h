// #include iostream

enum data {x,y,z,alpa,rotation};

class drone{
    private:
        int id;

    public:
        drone(/* args */);
        ~drone();
    
    drone::drone(/* args */){}
    
    drone::~drone(){}
    
    data get_leader_position();
    void move_drone(data positionChange);
    void run();
};
   
