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

void CPacketStream::Packet(unsigned char buf[1024], int *Size, int funcId, void *pData, int Len, int trans_id, int msg_code /* = 0 */)
{
	P_HEAD packet_head = {0};
    int count = 0;

    //header
    buf[0] = HEAD_CHR;
    count += HEAD_LEN;

    //header struct
	packet_head.funcId = funcId;
    count += 4;

    packet_head.size = sizeof(P_HEAD) + Len + 1; // header struct + data + sum
    count += 4;

    packet_head.trans_id = trans_id;
    count += 4;

	packet_head.msg_code = msg_code;
    count += 4;


    memcpy(buf + HEAD_LEN, &packet_head, sizeof(P_HEAD));

    //body data
	if (pData != NULL && Len > 0)
	{
        memcpy(buf + HEAD_LEN + sizeof(P_HEAD), pData, Len);
        count += Len;
    }

    //sum
    buf[count] = calcSum(buf + HEAD_LEN, (packet_head.size - 1));
    count += 1;

    //ender
    buf[count] = END_CHR;
    count += HEAD_LEN;

    *Size = count;
}

int CPacketStream::GetPack(char *buf, int nRead, char *pack, int *Len, int *funcId)
{
	int i = 0;
	P_HEAD head = {0};
	char head_str[2] = {HEAD_CHR, HEAD_CHR};
	char end_str[2] = {END_CHR, END_CHR};

	for (i = 0; i < nRead; i++)
	{
        // header fail
		if (*(buf + i) == HEAD_CHR)
		{
            //if (*(buf + i + 1) == HEAD_CHR)
			{
				//
                memcpy(&head, buf + i + HEAD_LEN, sizeof(P_HEAD));
                if (head.size > (nRead - i) // half packet
                        || head.size < 0)// fail packet ID
				{
					continue;
				}
				//
                if (memcmp(buf + i + head.size - HEAD_LEN, end_str, HEAD_LEN) == 0)
				{
					memcpy(pack, buf + i, head.size);
					*Len = head.size;
					*funcId = head.funcId;
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
