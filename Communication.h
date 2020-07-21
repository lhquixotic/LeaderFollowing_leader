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
#define VEHICLE_BRAKE_ID 0x18F02501s
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



//TODO:Vehicle speed id is?

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
    [[noreturn]] static void CAN0_update(); //send all signals
    //static void CAN1_update();

    static void CAN_send(int *message_ptr,int id,int msg_length,bool EFF, int CAN_channel);

    [[noreturn]] static void CAN_receive(int id,int msg_length,bool EFF,int CAN_channel);
    static int * CAN_get_msg(int id,bool EFF,int CAN_channel);

private:
    //CAN_send func
    static int socket_word;
    static int nbytes;
    static struct sockaddr_can addr;
    static struct ifreq ifr;
    static struct can_frame frame[2];
    //CAN_steer_msg
    //static int * Con2CAN_steer(int steer_enable,int steer_angle,int steer_velocity);
    //static int * Con2CAN_acc(int control_mode, int acc_value, int pressure_value);
    static int * Con2CAN_acc_pedal();
    static int * Con2CAN_brake();
    static int * Con2CAN_steering_wheel();
    static int * Con2CAN_wheel();
    static int * Con2CAN_la_yr();
    static int * Con2CAN_gear_position();
    static int * Con2CAN_pedal_angle();
    /*
    //CAN Receive
    static void CAN2Val_acc(int *message_ptr,int msg_length);
    static void CAN2Val_UWB_position(int *message_ptr,int msg_length);
    static void CAN2Val_UWB_leaderstate(int *message_ptr,int msg_length);
    static void CAN2Val_speed(int *message_ptr,int msg_length);
    static void CAN2Val_acc_pedal(int *message_ptr,int msg_length);
    static void CAN2Val_brake(int *message_ptr,int msg_length);
    static void CAN2Val_steering_wheel(int *message_ptr,int msg_length);
    static void CAN2Val_wheel(int *message_ptr,int msg_length);
    static void CAN2Val_la_yr(int *message_ptr,int msg_length);
    static void CAN2Val_gear_position(int *message_ptr,int msg_length);
    static void CAN2Val_pedal_angle(int *message_ptr,int msg_length);
     */
};

