#include <unistd.h>

class arocuLeaderInfo
{
private:
    double upDown ;
    double forward ;
    double rightLeft ;
    int idr ;
    bool ifArucoExist ;
    bool clockwise;
    int angle;
    bool runCamera ;
public:
    
    arocuLeaderInfo(/* args */);
    ~arocuLeaderInfo();
};

arocuLeaderInfo::arocuLeaderInfo(/* args */)
{
}

arocuLeaderInfo::~arocuLeaderInfo()
{
}
