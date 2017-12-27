//
// Created by huang on 17-12-5.
//
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>  /* netdb is necessary for struct hostent */


#define PORT 8888  /* server port */

#define MAXDATASIZE 600


typedef struct packet_head
{
    int size;// header len + data len + sum len( 1 byte )
    int funcId;
    int msg_code;//00 successed   FF fail
    int trans_id;
}P_HEAD;


typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
}MAP_PACKAGE_POP;

typedef struct {
    unsigned int package_sum;// map package total sum.
    unsigned int package_num;// map package num.
    unsigned int size;
    unsigned int width;
    unsigned int hight;
    char data[512];
}MAP_PACKAGE_ACK;

int main(int argc, char *argv[])
{

    popen("bash /home/huang/stage_ws/slam.sh","w");
    while(1)
    {
        char ch = getchar();
        printf("-----------Input %c-----\n",ch);
        if(ch == 'k'){
            system("rosnode kill /slam_gmapping");
            printf("end----------------------\n");
            break;
        }
    }

/*    int sockfd, num;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in server;

    if (argc != 2)
    {
        printf("Usage: %s <IP Address>\n",argv[0]);
        exit(1);
    }

    if((he=gethostbyname(argv[1]))==NULL)
    {
        printf("gethostbyname() error\n");
        exit(1);
    }

    if((sockfd=socket(AF_INET,SOCK_STREAM, 0))==-1)
    {
        printf("socket() error\n");
        exit(1);
    }
    bzero(&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr *)he->h_addr);
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server))==-1)
    {
        printf("connect() error\n");
        exit(1);
    }
    unsigned char str[50] = {0};
    unsigned char ack[600] = {0};
    P_HEAD *head = (P_HEAD*)(str+1);
    MAP_PACKAGE_POP *map_pop = (MAP_PACKAGE_POP*)(str+1+16);
    P_HEAD *ackhead = (P_HEAD*)(ack+1);
    MAP_PACKAGE_ACK *map_ack = (MAP_PACKAGE_ACK*)(ack+1+16);
    head->funcId = 2;
    head->size = 16+8;
    map_pop->package_num = 0;
    map_pop->package_sum = 0;
    str[0] = 0xAA;
    str[1+16+8+1]=0xAB;

    do{
        //sleep(1);
        //printf("send : \n id:%d  size:%d\n",head->funcId,head->size);
        printf("sum:%d num:%d\n",map_pop->package_sum,map_pop->package_num);
        for (int i = 0; i < 25; ++i) {
            printf("%02X ",str[i]);
        }
        printf("\n");
        if((num=send(sockfd,str,head->size+9,0))==-1){
            printf("send() error\n");
            exit(1);
        }

        //printf("wait ack...\n");
        int ret = 0;

            if((ret=recv(sockfd,ack,sizeof(MAP_PACKAGE_ACK)+19,0)) == -1) {
                printf("recv() error\n");
                exit(1);
            }
        if(ackhead->funcId == 2 )
        {
            printf("ack sum:%d num:%d\n",map_ack->package_sum,map_ack->package_num);
            map_pop->package_sum = map_ack->package_sum;
            map_pop->package_num = map_ack->package_num;
            num = 0;
            if (map_pop->package_sum <= map_pop->package_num)
            {
                break;
            }
        }
        else
        {
            printf("ack fail.\n");
            exit(1);
        }

    }while(map_pop->package_sum != 0 && map_pop->package_sum >= map_ack->package_num);


    //buf[num-1]='\0';
    printf("server message: %s\n",buf);
    close(sockfd);*/
    return 0;
}
