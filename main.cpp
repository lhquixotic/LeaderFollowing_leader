#include "Leader_following.h"

using namespace std;

int main()
{
    cout << "Main Thread" << endl;


    Communication follower_com;

    //follower_com.CAN_retrans(VEHICLE_BRAKE_MSG);
    static int CANmsg_origin_brake[9] = {0};
    static int CANmsg_origin_pedal[9] = {0};
    static int CANmsg_leader_uwb[9] = {0};
    thread th1(&Communication::CAN_retrans,&follower_com,VEHICLE_BRAKE_MSG,CANmsg_origin_brake);
    thread th2(&Communication::CAN_retrans,&follower_com,VEHICLE_PEDAL_ANGLE_MSG,CANmsg_origin_pedal);
    thread th3(&Communication::CAN_retrans,&follower_com,LEADER_UWB_MSG,CANmsg_leader_uwb);

    th1.join();
    th2.join();
    th3.join();

    return 0;
}
