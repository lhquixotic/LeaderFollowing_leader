#include "Leader_following.h"

using namespace std;
/*
int UWB_distance = 20 ;
int UWB_fangwei = 0 ;
int UWB_zitai = 0 ;
int Leader_velocity = 0 ;
int Leader_acceleration = 0 ;
int Follower_velocity = 0 ;
int Follower_acceleration = 0 ;

int Leader_ACC_pedal_position = 0;
int Leader_Remote_position = 0;
int Leader_Brake_pedal_position = 0;
int Leader_Actual_acc = 0;
int Leader_Speed = 0;
int Leader_Pressure = 0;
int Leader_Steering_wheel_angle = 0;
int Leader_Steering_wheel_speed = 0;
int Leader_Steering_wheel_state = 0;
int Leader_Count = 0;
int Leader_Check = 0;
int Leader_Wheel_speed = 0;
int Leader_La_acc = 0;
int Leader_Yr_speed = 0;
int Leader_Target_gear = 0;
int Leader_Current_gear = 0;
int Leader_Acc_pedal = 0;
int Leader_Brake_pedal = 0;

int Control_steer_angle = 32767 ;
int Control_steer_enable = 0 ;
int Control_steer_velocity = 56 ;

int Control_mode = 2;
int Control_acceleration = 0;
int Control_pressure = 0.5;
*/
int MSG[8] ={0};

int *CAN_acc_pedal_msg = MSG;
int *CAN_brake_msg = MSG;
int *CAN_steering_wheel_msg = MSG;
int *CAN_wheel_msg = MSG;
int *CAN_la_yr_msg = MSG;
int *CAN_gear_position_msg = MSG;
int *CAN_pedal_angle_msg = MSG;

int main()
{
    cout << "Main Thread" << endl;


    Communication follower_communication;

    thread th4(&follower_communication.CAN_receive, VEHICLE_ACC_PEDAL_MSG);//send control signals
    thread th5(&follower_communication.CAN_receive, VEHICLE_BRAKE_MSG);//send control signals
    thread th6(&follower_communication.CAN_receive, VEHICLE_STEERING_WHEEL_MSG);//send control signals
    thread th7(&follower_communication.CAN_receive, VEHICLE_WHEEL_MSG);//send control signals
    thread th8(&follower_communication.CAN_receive, VEHICLE_LA_YR_MSG);//send control signals
    thread th9(&follower_communication.CAN_receive, VEHICLE_GEAR_POSITION_MSG);//send control signals
    thread th10(&follower_communication.CAN_receive, VEHICLE_PEDAL_ANGLE_MSG);//send control signals
    thread th11(&follower_communication.CAN0_update);//send control signals


    th4.join();
    th5.join();
    th6.join();
    th7.join();
    th8.join();
    th9.join();
    th10.join();
    th11.join();
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
