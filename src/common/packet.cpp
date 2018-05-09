#include <neo_bridge/packet.h>
#include "packet.h"

void DEBUG_LOG(unsigned char *data,int len)
{
    for(int i = 0; i < len ; i++)
    {
        printf("%02X ",data[i]);
    }
    printf("\n");
}

static unsigned char calcSum(unsigned char *data, int len)
{
    unsigned char sum = 0;
    if(data == NULL) return sum;
    for(int i = 0; i < len; i++)
    {
        sum += data[i];
    }
    return sum;
}

//0 OK   -1 fail
static unsigned char checkSum(unsigned char *data, int len)
{
    unsigned char sum = 0;
    int i;
    if(data == NULL) return 1;
    for(i = 0; i < len; i++)
    {
        sum += data[i];
    }
    if(sum == data[i]) return 0;
    return 1;
}

void CPacketStream::Packet(unsigned char buf[1024], int *Size, int function_id, void *pData, int Len, int trans_id, int device_id /* = 0 */)
{
    Neo_Packet::HEAD packet_head = {0};
    int count = 0;

    //header
    buf[0] = HEADER_CHR;
    count += HEADER_LEN;

    //header struct
	packet_head.function_id = function_id;
    count += 4;

    packet_head.size = sizeof(Neo_Packet::HEAD) + Len; // header struct + data + sum
    count += 4;

    packet_head.ref = trans_id;
    count += 4;

	packet_head.device_id = device_id;
    count += 4;


    memcpy(buf + HEADER_LEN, &packet_head, sizeof(Neo_Packet::HEAD));

    //body data
	if (pData != NULL && Len > 0)
	{
        memcpy(buf + HEADER_LEN + sizeof(Neo_Packet::HEAD), pData, Len);
        count += Len;
    }

    //sum
    buf[count] = calcSum(buf + HEADER_LEN, (packet_head.size));
    count += 1;

    //ender
    buf[count] = ENDER_CHR;
    count += HEADER_LEN;

    *Size = count;
}

int CPacketStream::GetPack(char *buf, int nRead, char *pack, int *Len, int *function_id)
{
	int i = 0;
    Neo_Packet::HEAD head = {0};
	char head_str[2] = {HEADER_CHR, HEADER_CHR};
	char end_str[2] = {ENDER_CHR, ENDER_CHR};

	for (i = 0; i < nRead; i++)
	{
        // header fail
		if (*(buf + i) == HEADER_CHR)
		{
            //if (*(buf + i + 1) == HEAD_CHR)
			{
				//
                memcpy(&head, buf + i + HEADER_LEN, sizeof(Neo_Packet::HEAD));
                if (head.size > (nRead - i) // half packet
                        || head.size < 0)// fail packet ID
				{
					continue;
				}

				//
                if (memcmp(buf + i + head.size - HEADER_LEN, end_str, HEADER_LEN) == 0)
				{
					memcpy(pack, buf + i, head.size);
					*Len = head.size;
					*function_id = head.function_id;
					return i;
				}
				else//
				{
					continue;
				}
			}
		}
	}

	*Len = 0;
	return i;
}
