//
// Created by nvidia on 20-6-21.
//
#include "Leader_following.h"

using namespace std;

void Communication::CAN_retrans(int id_origin,int msg_length,bool EFF,int CAN_channel){
    int *CANmsg_origin;
    for(;;){
        CANmsg_origin = Communication::CAN_get_msg(id_origin,EFF,CAN_channel);
        Communication::CAN_send(MsgConvert(id_origin,CANmsg_origin),LEADER_MSG);
    }
}

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

int * Communication::MsgConvert(int id_origin, int *msg_ptr){
    static int msg_retrans[8] = {0};
    if(id_origin == VEHICLE_ACC_PEDAL_ID)
        msg_retrans[0] = 0xA1;
    else if(id_origin == VEHICLE_BRAKE_ID)
        msg_retrans[0] = 0xA2;
    else if(id_origin == VEHICLE_STEERING_WHEEL_ID)
        msg_retrans[0] = 0xA3;
    else if(id_origin == VEHICLE_WHEEL_ID)
        msg_retrans[0] = 0xA4;
    else if(id_origin == VEHICLE_LA_YR_ID)
        msg_retrans[0] = 0xA5;
    else if(id_origin == VEHICLE_GEAR_POSITION_ID)
        msg_retrans[0] = 0xA6;
    else if(id_origin == VEHICLE_PEDAL_ANGLE_ID)
        msg_retrans[0] = 0xA7;
    msg_retrans[1] = *(msg_ptr);
    msg_retrans[2] = *(msg_ptr + 1);
    msg_retrans[3] = *(msg_ptr + 2);
    msg_retrans[4] = *(msg_ptr + 3);
    msg_retrans[5] = *(msg_ptr + 4);
    msg_retrans[6] = *(msg_ptr + 5);
    msg_retrans[7] = *(msg_ptr + 6);
    return msg_retrans;
}