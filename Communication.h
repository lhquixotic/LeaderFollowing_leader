//
// Created by nvidia on 20-6-23.
//

#ifndef LEADERFOLLOWING_COMMUNICATION_H
#define LEADERFOLLOWING_COMMUNICATION_H

#endif //LEADERFOLLOWING_COMMUNICATION_H

#define CAN_EFF_FLAG 0x80000000U //Extended Frame Mark
#define CAN_EFF_MASK 0x1FFFFFFFU //Extended Frame format
//CAN receive

#define VEHICLE_ACC_PEDAL_ID 0x0CF00300
#define VEHICLE_ACC_PEDAL_MSG VEHICLE_ACC_PEDAL_ID,8,1,1
#define VEHICLE_BRAKE_ID 0x18F02501
#define VEHICLE_BRAKE_MSG VEHICLE_BRAKE_ID,8,1,1
#define VEHICLE_STEERING_WHEEL_ID 0x18F01D48
#define VEHICLE_STEERING_WHEEL_MSG VEHICLE_STEERING_WHEEL_ID,8,1,1
#define VEHICLE_WHEEL_ID 0x18F0010B
#define VEHICLE_WHEEL_MSG VEHICLE_WHEEL_ID,8,1,1
#define VEHICLE_LA_YR_ID 0x18F02505
#define VEHICLE_LA_YR_MSG VEHICLE_LA_YR_ID,8,1,1
#define VEHICLE_GEAR_POSITION_ID 0x18F00503
#define VEHICLE_GEAR_POSITION_MSG VEHICLE_GEAR_POSITION_ID,8,1,1
#define VEHICLE_PEDAL_ANGLE_ID 0x18F02502
#define VEHICLE_PEDAL_ANGLE_MSG VEHICLE_PEDAL_ANGLE_ID,8,1,1

//CAN send

#define LEADER_ID  0x03100000
#define LEADER_MSG LEADER_ID,8,1,1


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <iostream>
#include <string.h>
#include <string>
#include <mutex>
 
class Communication{
public:

    void CAN_retrans(int id_origin,int msg_length,bool EFF,int CAN_channel);
private:

    void CAN_send(int *message_ptr,int id,int msg_length,bool EFF, int CAN_channel);
    int * CAN_get_msg(int id,bool EFF,int CAN_channel);
    int * MsgConvert(int id_origin, int *msg_ptr);
};

