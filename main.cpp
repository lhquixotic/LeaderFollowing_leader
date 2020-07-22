#include "Leader_following.h"

using namespace std;

int main()
{
    cout << "Main Thread" << endl;


    Communication follower_com;

    thread th1(&follower_com.CAN_retrans,VEHICLE_BRAKE_MSG);
    thread th2(&follower_com.CAN_retrans,VEHICLE_PEDAL_ANGLE_MSG);

    th1.join();
    th2.join();

    /* TEST:CAN receive in main thread
    int * canmsg;
    while(1)
    {
        canmsg = follower_communication.CAN_get_msg(23);
        cout << "Leader_acceleration = " << Leader_acceleration << endl;
        follower_communication.CAN2Val_acc(canmsg,4);
        usleep(SAMPLE_TIME);

    }
    */

    /* TEST:CAN Send in main thread
    int test_message = 234;
    while(1){
        follower_communication.CAN_send(test_message);
        usleep(200000);
    }
    */

    return 0;
}
