#ifndef _PACKET_H_
#define _PACKET_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define HEADER_CHR ((char)0xAA)
#define ENDER_CHR  ((char)0xAB)
#define HEADER_LEN 1

namespace Neo_Packet {

    enum PacketType {
        ROBOTSTATUS = 0,
        MAPDATAS,
        SENDGOAL,
        CANCELGOAL,
        ROSCONTROL,
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
