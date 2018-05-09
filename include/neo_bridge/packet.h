#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define HEAD_CHR 0x04
//#define END_CHR  0x05
//#define H_E_LEN 2

#define HEADER_CHR ((char)0xAA)
#define ENDER_CHR  ((char)0xAB)
#define HEADER_LEN 1

#define PACK_HEARD		0
#define PACK_LOGIN		1
#define PACK_MAP        2
#define PACK_GOAL		3
#define PACK_CANCELGOAL 4
#define PACK_NODECTL	5
#define PACK_NAVIGATION	6
#define PACK_LOCATION	7
#define PACK_CMDVEL     8
#define PACK_LIDAR		9
#define PACK_LOADMAP    10
#define PACK_GLOBALPATH	11


namespace Neo_Packet {

    enum PacketType {
        ROBOTSTATUS = 0,
        MAPDATAS,
        SENDGOAL,
        CANCELGOAL,
        ROSCONTROL,
        LOCALIZATION,
        CMDVEL,
        LIDARDATAS,
        MAPUPLOAD,
        GLOBALPLAN,
        FORWARDTHENINTERPOLATE
    };

	typedef struct {
		int size;
		int function_id;
		int device_id;
		int ref;
	} HEAD;
}

void DEBUG_LOG(unsigned char *data,int len);
class CPacketStream
{
public:
	void Packet(unsigned char buf[1024], int *Size, int function_id, void *pData, int Len, int trans_id, int device_id = 0);
	int GetPack(char *buf, int nRead, char *pack, int *Len, int *function_id);
private:
};

#endif
