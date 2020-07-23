#include "Leader_following.h"

using namespace std;

int main()
{
    cout << "Main Thread" << endl;


    Communication follower_com;

    thread th1(&Communication::CAN_retrans,&follower_com,VEHICLE_BRAKE_MSG);
    //thread th2(&follower_com.CAN_retrans,VEHICLE_PEDAL_ANGLE_MSG);

    th1.join();
    //th2.join();



    return 0;
}
