//
// Created by nvidia on 20-6-21.
//
#include "Leader_following.h"

using namespace std;

[[noreturn]] void Communication::CAN0_update(){
    for(;;){
        //Communication::CAN_send(Con2CAN_acc_pedal(),
        //                        LEADER_MSG);
        Communication::CAN_send(Con2CAN_brake(),
                                LEADER_MSG);
        /*
        Communication::CAN_send(Con2CAN_steering_wheel(),
                                LEADER_MSG);
        Communication::CAN_send(Con2CAN_wheel(),
                                LEADER_MSG);
        Communication::CAN_send(Con2CAN_la_yr(),
                                LEADER_MSG);
        Communication::CAN_send(Con2CAN_gear_position(),
                                LEADER_MSG);
        Communication::CAN_send(Con2CAN_pedal_angle(),
                                LEADER_MSG);
        */
        usleep(SAMPLE_TIME);
        //cout << "CAN1 data is updating..." << endl;
    }
}
//Send all messages

void Communication::CAN_send(int *message_ptr,int id,int msg_length,bool EFF, int CAN_channel){
    //cout << "Sending..." << endl;

    /***********************Sockek_CAN config*****************************/
    //CAN send configuration
    int socket_word,nbytes;
    struct sockaddr_can addr;
    struct ifreq ifr;
    struct can_frame frame[2] = {{0}};
    mutex cansend_mut;
    cansend_mut.lock();
    socket_word = socket(PF_CAN,SOCK_RAW,CAN_RAW);

    if(CAN_channel == 0)
        strcpy(ifr.ifr_name,"can0");
    else if(CAN_channel == 1)
        strcpy(ifr.ifr_name,"can1");

    ioctl(socket_word,SIOCGIFINDEX,&ifr);
    addr.can_family =  AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    bind(socket_word,(struct sockaddr *)&addr,sizeof(addr));
    setsockopt(socket_word,SOL_CAN_RAW,CAN_RAW_FILTER,NULL,0);
    /***********************************************************************/

    //CAN Information
    if(EFF)
        frame[0].can_id = CAN_EFF_FLAG | id;
    else
        frame[0].can_id = id;//CAN ID NOTICE:the input should map to HEX
    frame[0].can_dlc = msg_length;//Message Length

    //CAN message
    for(int i=0;i<msg_length;i++)
        frame[0].data[i] = *(message_ptr+i);

    //Send CAN message
    nbytes = write(socket_word,&frame[0],sizeof(frame[0]));
    // Print CAN message or error
    if(nbytes != sizeof(frame[0])){
        cout << "CAN Send Error! Check bitrate. " << endl;
    }
    else{

        cout << "Sending: ";
        /*
        if(id == CONTROL_STEER_ID)
            cout << "(control steer)";
        else if(id == CONTROL_ACC_ID)
            cout << "(control acc)";
        */
         cout << " CAN ID 0x" << hex << id << " : ";
        for(int i=0; i< msg_length; i++) {
            //CANmsg_acc[i] = *(CANmsg_acc_ptr + i);
            cout << hex << message_ptr[i] << " ";
        }
        cout << endl;

    };
    close(socket_word);
    cansend_mut.unlock();
}
//Send a certain ID CANmsg

[[noreturn]] void Communication::CAN_receive(int id,int msg_length,bool EFF,int CAN_channel){
    for(;;){
        /*
        if(id == VEHICLE_ACC_ID)
            CAN2Val_acc(CAN_get_msg(id,EFF,CAN_channel),msg_length);
        else if(id == VEHICLE_SPEED_ID)
            CAN2Val_speed(CAN_get_msg(id,EFF,CAN_channel),msg_length);
        else if(id == UWB_POSITION_ID)
            CAN2Val_UWB_position(CAN_get_msg(id,EFF,CAN_channel),msg_length);
        else if(id == UWB_LEADERSTATE_ID){
            int* msg = CAN_get_msg(id,EFF,CAN_channel);
            if(msg[0] == 0xA1)
                CAN2Val_acc_pedal(msg,msg_length);
            else if(msg[0] == 0xA2)
                CAN2Val_brake(msg,msg_length);
            else if(msg[0] == 0xA3)
                CAN2Val_steering_wheel(msg,msg_length);
            else if(msg[0] == 0xA4)
                CAN2Val_wheel(msg,msg_length);
            else if(msg[0] == 0xA5)
                CAN2Val_la_yr(msg,msg_length);
            else if(msg[0] == 0xA6)
                CAN2Val_gear_position(msg,msg_length);
            else if(msg[0] == 0xA7)
                CAN2Val_pedal_angle(msg,msg_length);
            else
                CAN2Val_UWB_leaderstate(msg,msg_length);
        }
        */
        if(id == VEHICLE_ACC_PEDAL_ID)
            ;//CAN_acc_pedal_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_BRAKE_ID)
            CAN_brake_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_STEERING_WHEEL_ID)
            ;//CAN_steering_wheel_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_WHEEL_ID)
            ;//CAN_wheel_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_LA_YR_ID)
            ;//CAN_la_yr_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_GEAR_POSITION_ID)
            ;//CAN_gear_position_msg = CAN_get_msg(id,EFF,CAN_channel);
        else if(id == VEHICLE_PEDAL_ANGLE_ID)
            ;//CAN_pedal_angle_msg = CAN_get_msg(id,EFF,CAN_channel);
    }

}
//Receive a certain ID CANmsg

int * Communication::CAN_get_msg(int id, bool EFF, int CAN_channel){
    //cout << "Receiving ID " << id << " ..." << endl;

    /***********************Sockek_CAN config*****************************/
    static int socket_word, nbytes;
    static struct sockaddr_can addr;
    static struct ifreq ifr;
    static struct can_frame frame;
    static struct can_filter rfliter[1];
    mutex candump_mut;
    candump_mut.lock();
    socket_word = socket(PF_CAN, SOCK_RAW, CAN_RAW);

    //Choose CAN channel
    if(CAN_channel == 0)
        strcpy(ifr.ifr_name, "can0" );
    else if(CAN_channel == 1)
        strcpy(ifr.ifr_name, "can1");

    ioctl(socket_word, SIOCGIFINDEX, &ifr);
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    struct can_filter rfilter[1];
    bind(socket_word, (struct sockaddr *)&addr, sizeof(addr));
    //定义接收规则，只接收表示符等于 id 的报文
    rfilter[0].can_id = id;
    if(EFF)
        rfilter[0].can_mask = CAN_EFF_MASK;
    else
        rfilter[0].can_mask = CAN_SFF_MASK;
    //设置过滤规则
    setsockopt(socket_word, SOL_CAN_RAW, CAN_RAW_FILTER, &rfilter, sizeof(rfilter));
    nbytes = read(socket_word, &frame, sizeof(frame));
    if(nbytes > 0)
    {
        //cout << "Receiving..." << endl;
    }
    /**********************************************************************/
    //cout << "**********************" << endl;
    // Convert CANmsg to int ptr
    /*
    cout << "Receiving: ";
    if(id == VEHICLE_ACC_ID)
        cout << "(vehicle acc)";
    else if(id == VEHICLE_SPEED_ID)
        cout << "(vehicle speed)";
    else if(id == UWB_POSITION_ID)
        cout << "(UWB position)";
    else if(id == UWB_LEADERSTATE_ID)
        cout << "(UWB leader state)";
    */
    cout << " CAN ID 0x" << hex << id << ": ";

     static int CAN_msg[8] = {0};
    for (int i=0;i<8;i++){
        CAN_msg[i] = (int)frame.data[i];
        cout << CAN_msg[i] << " " ;
    }
    cout << endl;

    close(socket_word);
    candump_mut.unlock();
    return CAN_msg;

}
//get CAN message

/*
void Communication::CAN1_update(){
    for(;;){
        time_t can2_time;
        can2_time = time(NULL);
        Follower_velocity = 0;
        Follower_acceleration = 0;
        usleep(SAMPLE_TIME);
        cout << "CAN2 data is updating..." << endl;
        //cout << "CAN2 data is updating..." << "  Time is " << can2_time << endl;
    }
}
*/
// CAN1 update

/********************************Convert value to CAN message************************************/
/*
int * Communication::Con2CAN_steer(int steer_enable,int steer_angle,int steer_velocity){
    static int msg_steer[8] = {0};
    msg_steer[0] = steer_enable;
    msg_steer[1] = steer_velocity/4;
    msg_steer[2] = steer_angle%256;
    msg_steer[3] = steer_angle/256;
    return msg_steer;
}
//Convert steer information to CANmsg

int * Communication::Con2CAN_acc(int control_mode,int acc_value, int pressure_value){
    static int msg_acc[8] = {0};
    static int loop_number = 0;
    msg_acc[2] = control_mode;
    if(control_mode==0){//mode 0:No Brake
        msg_acc[0] = 0;
        msg_acc[1] = 0;
    }
    else if(control_mode==1){//mode 1:Require deacc
        msg_acc[0] = acc_value%256;
        msg_acc[1] = acc_value/256;
    }
    else if(control_mode==2){//mode 2:Require pressure
        msg_acc[0] = pressure_value%256;
        msg_acc[1] = pressure_value/256;
    }
    else if(control_mode==3){//mode 3:Require acc
        msg_acc[0] = acc_value%256;
        msg_acc[1] = acc_value/256;
    }
    msg_acc[7] = loop_number%16;
    loop_number ++;
    return msg_acc;
}
*/
int * Communication::Con2CAN_acc_pedal()
{
    static int msg_acc_pedal[8] = {0};
    msg_acc_pedal[0] = 0xA1;
    msg_acc_pedal[1] = *(CAN_acc_pedal_msg + 1);
    msg_acc_pedal[4] = *(CAN_acc_pedal_msg + 4);
    return msg_acc_pedal;
}

int * Communication::Con2CAN_brake()
{
    static int msg_brake[8] = {0};
    msg_brake[0] = 0xA2;
    msg_brake[1] = *(CAN_brake_msg);
    msg_brake[2] = *(CAN_brake_msg+1);
    msg_brake[3] = *(CAN_brake_msg+2);
    msg_brake[4] = *(CAN_brake_msg+3);
    msg_brake[5] = *(CAN_brake_msg+4);
    msg_brake[6] = *(CAN_brake_msg+5);
    return msg_brake;
}

int * Communication::Con2CAN_steering_wheel()
{
    static int msg_steering_wheel[8] = {0};
    msg_steering_wheel[0] = 0xA3;
    msg_steering_wheel[1] = *(CAN_steering_wheel_msg);
    msg_steering_wheel[2] = *(CAN_steering_wheel_msg + 1);
    msg_steering_wheel[3] = *(CAN_steering_wheel_msg + 2);
    msg_steering_wheel[4] = *(CAN_steering_wheel_msg + 3);
    msg_steering_wheel[5] = *(CAN_steering_wheel_msg + 5);
    msg_steering_wheel[7] = *(CAN_steering_wheel_msg + 7);
    return msg_steering_wheel;
}

int * Communication::Con2CAN_wheel()
{
    static int msg_wheel[8] = {0};
    msg_wheel[0] = 0xA4;
    msg_wheel[1] = *(CAN_wheel_msg );
    msg_wheel[2] = *(CAN_wheel_msg + 1);
    msg_wheel[3] = *(CAN_wheel_msg + 2);
    return msg_wheel;
}

int * Communication::Con2CAN_la_yr()
{
    static int msg_la_yr[8] = {0};
    msg_la_yr[0] = 0xA5;
    msg_la_yr[1] = *(CAN_la_yr_msg );
    msg_la_yr[2] = *(CAN_la_yr_msg + 1);
    msg_la_yr[6] = *(CAN_la_yr_msg + 6);
    msg_la_yr[7] = *(CAN_la_yr_msg + 7);
    return msg_la_yr;
}

int * Communication::Con2CAN_gear_position()
{
    static int msg_gear_position[8] = {0};
    msg_gear_position[0] = 0xA6;
    msg_gear_position[1] = *(CAN_gear_position_msg);
    msg_gear_position[5] = *(CAN_gear_position_msg + 4);
    return msg_gear_position;
}

int * Communication::Con2CAN_pedal_angle()
{
    static int msg_pedal_angle[8] = {0};
    msg_pedal_angle[0] = 0xA7;
    msg_pedal_angle[1] = *(CAN_pedal_angle_msg );
    msg_pedal_angle[2] = *(CAN_pedal_angle_msg + 1);
    msg_pedal_angle[6] = *(CAN_pedal_angle_msg + 6);
    msg_pedal_angle[7] = *(CAN_pedal_angle_msg + 7);
    return msg_pedal_angle;
}
//Convert acc information to CANmsg
/************************************************************************************************/


/********************************Convert CAN message to value************************************/
/*
void Communication::CAN2Val_acc(int *CANmsg_acc,int msg_length){
    Follower_acceleration = CANmsg_acc[0] + CANmsg_acc[1];//TODO:resolution
}
//Convert CANmsg to follower acc value

void Communication::CAN2Val_UWB_position(int*CANmsg_UWB,int msg_length){
    //cout << "UWB_position" << endl;
    UWB_distance = 256 * CANmsg_UWB[3] + CANmsg_UWB[2];
    if (CANmsg_UWB[5] >= 0x80)
    {
        UWB_fangwei = 256 * CANmsg_UWB[5] + CANmsg_UWB[4] - 65536;
    }
    else {
        UWB_fangwei = 256 * CANmsg_UWB[5] + CANmsg_UWB[4];
    }
    if (CANmsg_UWB[7] >= 0x80)
    {
        UWB_zitai = 256 * CANmsg_UWB[7] + CANmsg_UWB[6] - 65536;
    }
    else {
        UWB_zitai = 256 * CANmsg_UWB[7] + CANmsg_UWB[6];
    }

}
//Convrt CANmsg to UWB position value

void Communication::CAN2Val_UWB_leaderstate(int*CANmsg_UWB_state,int msg_length){
    //CANmsg_UWB_state[8] TODO: convert CAN message to value
}
//Convert CANmsg to leader state value

void Communication::CAN2Val_speed(int*CANmsg_speed,int msg_length){
    Follower_velocity = CANmsg_speed[6] + CANmsg_speed[7] * 256;
}

void Communication::CAN2Val_acc_pedal(int*CANmsg_acc_pedal,int msg_length){
    Leader_ACC_pedal_position = CANmsg_acc_pedal[1];
    Leader_Remote_position = CANmsg_acc_pedal[4];
}

void Communication::CAN2Val_brake(int*CANmsg_brake,int msg_length){
    Leader_Brake_pedal_position = CANmsg_brake[1];
    if(CANmsg_brake[3] >= 0x80)
        Leader_Actual_acc = (CANmsg_brake[3] * 256 + CANmsg_brake[2]) - 0xFFFF - 1;
    else
        Leader_Actual_acc = (CANmsg_brake[3] * 256 + CANmsg_brake[2]);
    Leader_Speed = (CANmsg_brake[5] * 256 + CANmsg_brake[4]);
    Leader_Pressure = CANmsg_brake[6];
}

void Communication::CAN2Val_steering_wheel(int*CANmsg_steering_wheel,int msg_length){
    if(CANmsg_steering_wheel[2] >= 0x80)
        Leader_Steering_wheel_angle = (CANmsg_steering_wheel[2] * 256 + CANmsg_steering_wheel[1]) - 0xFFFF - 1;
    else
        Leader_Steering_wheel_angle = (CANmsg_steering_wheel[2] * 256 + CANmsg_steering_wheel[1]);
    Leader_Steering_wheel_speed = CANmsg_steering_wheel[3];
    Leader_Steering_wheel_state = CANmsg_steering_wheel[4];
    Leader_Count = CANmsg_steering_wheel[5];
    Leader_Check = CANmsg_steering_wheel[7];
}


void Communication::CAN2Val_wheel(int*CANmsg_wheel_speed,int msg_length){
    Leader_Wheel_speed = CANmsg_wheel_speed[3] * (256 * 256) + CANmsg_wheel_speed[2] * 256 + CANmsg_wheel_speed[1];
}

void Communication::CAN2Val_la_yr(int*CANmsg_la_yr,int msg_length){
    Leader_La_acc = CANmsg_la_yr[1] + CANmsg_la_yr[2] * 256;
    Leader_Yr_speed = CANmsg_la_yr[1] + CANmsg_la_yr[2] * 256;
}

void Communication::CAN2Val_gear_position(int*CANmsg_gear_position,int msg_length){
    Leader_Target_gear = CANmsg_gear_position[1];
    Leader_Current_gear = CANmsg_gear_position[5];
}

void Communication::CAN2Val_pedal_angle(int*CANmsg_pedal_angle,int msg_length){
    Leader_Acc_pedal = CANmsg_pedal_angle[1] + CANmsg_pedal_angle[2] * 256;
    Leader_Brake_pedal = CANmsg_pedal_angle[6] + CANmsg_pedal_angle[7] * 256;
}
*/
//Convert CANmsg to follower speed
/************************************************************************************************/
