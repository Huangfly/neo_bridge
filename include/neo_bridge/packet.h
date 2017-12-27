#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define HEAD_CHR 0x04
//#define END_CHR  0x05
//#define H_E_LEN 2

#define HEAD_CHR 0xAA
#define END_CHR  0xAB
#define HEAD_LEN 1

///////////////////////////////////////
//
#define PACK_HEARD		0 //
#define PACK_LOGIN		1 //
#define PACK_MAP        2 //
#define PACK_GOAL		3 //
#define PACK_CANCELGOAL 4 //
#define PACK_NODECTL	5 //
#define PACK_NAVIGATION	6 //
#define PACK_LOCATION	7 //

//
typedef struct packet_head 
{
        int size;// header len + data len + sum len( 1 byte )
        int funcId;
        int msg_code;//00 successed   FF fail
		int trans_id;
}P_HEAD;
/*
//
typedef struct HeadData_t
{
	int funcId;
	bool isdeal;
	int size;
}P_HEAD;*/

//
typedef struct packet_login 
{
	char name[20];
	char pwd[10];
}P_LOGIN;

//
typedef struct ack_login
{
	int user_id;
	int login_ret;//
}A_LOGIN;

//
typedef struct ack_channel
{
	int t_count;
	int channel_id;
	char name[20];
}A_CHANNEL;

//
typedef struct ack_type
{
	int t_count;
	int channel_id;
	int type_id;
	char name[20];
}A_TYPE;

//
typedef struct ack_region
{
	int t_count;
	int channel_id;
	int region_id;
	char name[20];
}A_REGION;

//
typedef struct pack_videolist 
{
	int page;//
	int channel_id;//
	int type_id;//
	int region_id;//
}P_VIDEO;

//
typedef struct ack_video
{
	int count;
	int video_id;
	int channel_id;
	int type_id;
	int region_id;
	char name[20];
	int playtimes;
	char is_new;
}A_VIDEO;

//
typedef struct packet_play
{
	int user_id;
	int video_id;
}P_PLAY;
//
typedef struct ack_play
{
	int record_id;
	int user_id;
	int video_id;
	int play_times;
	char video_name[20];
	char time[24];
}A_PLAY;

//
typedef struct packet_playtime
{
	int record_id;
	int end_time;
}P_PTIME;


void DEBUG_LOG(unsigned char *data,int len);
class CPacketStream
{
public:
        void Packet(unsigned char buf[1024], int *Size, int funcId, void *pData, int Len, int trans_id, int msg_code = 0);
	int GetPack(char *buf, int nRead, char *pack, int *Len, int *funcId);
private:
};

#endif
